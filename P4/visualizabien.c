#include <stdio.h>
#include <unistd.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int main(int argc, char const *argv[]) {
  // Antes de nada he de controlar los argumentos
  if (argc != 1) {
    perror("./visualizabien < imagen.formato.gz" );
    exit(EXIT_FAILURE);
  }
  // variables de herramientas
  int pipe_zcat_in[2], pipe_zcat_out[2], pipe_xview[2], pipe_wc[2], i, read_bytes;
  char buffer[1024];
  pid_t pid[Nsons]; //voy a crear 3 hijos (zcat, xview, wc)
  //variables de control
  int epipezcat_out, epipezcat_in, epipexview, epipewc, eimg, Nsons = 3;

  //creo las tuberias: sólo dos ya que como es copy on write,
  //los hijos heredan de los padres los argumentos y no hace falta pasarselo
  // por otra tuberia
  epipezcat_out = pipe(pipe_zcat_out);
  epipewc = pipe(pipe_wc);
  epipexview = pipe(pipe_xview);
  //epipezcat_in =pipe(pipe_zcat_in);
  if ((epipewc ==-1) || (epipezcat_out ==-1) || (epipexview == -1) || (epipezcat_in == -1)) {
    perror("Error al crear tuberias");
    exit(EXIT_FAILURE);
  }
  //creo mis tres hijos
  for (i = 0; i < Nsons; i++) {
    pid[i] = fork();
    if (pid[i]==-1) {
      perror("Error en la creación del hijo");
      exit(EXIT_FAILURE);
    } else if(pid[i] == 0) {
      printf("Soy un hijo con pid: %d e ID:%d y el ID de mi padre es: %d\n", pid[i], getpid(), getppid());
      break;
    }
  }

  if (pid[0]==0) {
    //soy el hijo que hace zcat-> cierro mi salida estandar y la redirijo a pipe_zcat_out[1]
    close(pipe_wc[0]);
    close(pipe_wc[1]);
    close(pipe_xview[0]);
    close(pipe_xview[1]);

    //close(0); //copy on write esto no hace falta
    //dup(pipe_zcat_in[0]):
    //close(pipe_zcat_in[0]):
    //close(pipe_zcat_in[1]);
    close(pipe_zcat_out[0]);
    close(1);
    dup(pipe_zcat_out[1]);
    close(pipe_zcat_out[1]);

    execlp("zcat", "zcat", 0, NULL);//leo de cero porque tengo una redireccion
    perror("Error al ejecutar zcat");
    exit(EXIT_FAILURE);
  } else if (pid[1] == 0) {
    //soy el hijo que ejecuta xview
    //close(pipe_zcat_in[0]);
    //close(pipe_zcat_in[1]);
    close(pipe_zcat_out[0]);
    close(pipe_zcat_out[1]);
    close(pipe_wc[0]);
    close(pipe_wc[1]);

    close(pipe_xview[1]);
    close(0);
    dup(pipe_xview[0]);
    close(pipe_xview[0]);
    execlp("xview", "xview", "stdin", NULL);
    perror("Error al ejecutar xview");
    exit(EXIT_FAILURE);
  } else if (pid[2]==0) {
    // soy el hijo que ejecuta wc
    //close(pipe_zcat_in[0]);
    //close(pipe_zcat_in[1]);
    close(pipe_zcat_out[0]);
    close(pipe_zcat_out[1]);
    close(pipe_xview[0]);
    close(pipe_xview[1]);

    close(pipe_wc[1]);
    close(0);
    dup(pipe_wc[0]);
    close(pipe_wc[0]);
    execlp("wc", "wc", "-c", NULL);
    perror("Error al ejecutar wc");
    exit(EXIT_FAILURE);
  } else {
    //padre
    //close(pipe_zcat_in[0]);
    close(pipe_wc[0]);
    close(pipe_xview[0]);
    close(pipe_zcat_out[1]);
    //para utilizar la cuarta tuberia deberia diferenciar los read_bytes.
    //do {
    //  read_bytes2 = read(0, buffer, sizeof(buffer));
  //    printf("%d\n", read_bytes2);
  //    write(pipe_zcat_in[1], buffer, read_bytes2);

    do {
      read_bytes = read(pipe_zcat_out[0], buffer, sizeof(buffer));
      write(pipe_xview[1], buffer, read_bytes);
      write(pipe_wc[1], buffer, read_bytes);
    } while(read_bytes > 0);
//  } while(read_bytes2 > 0);
  }

  exit(EXIT_SUCCESS);
}
