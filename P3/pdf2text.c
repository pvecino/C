#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>


int main(int argc, char const *argv[]) {
  //compruebo argumentos, tienen que exisistir al menos dos arguamentos y ser impares.
  if ((argc<3) || (argc % 2 == 0)) {
    perror("./pdf2text.c <pdf1.pdf> <txt1.txt> [<pdfN.pdf> <txtN.txt>]");
    exit(EXIT_FAILURE);
  }
  pid_t pid;
  int pipe_pdf[2], err, Npdf, Ntxt, Nsons, i, j = 0, ftxt, epdf;
  int read_bytes;
  char buffer[1024];
  Nsons = (argc - 1 )/2;

  //Aqui empezaria un bucle que durará: (argc-1)/2-> numero de hijos.
  for (i = 0; i < Nsons; i++) {
    j++;//lo pongo aqui para depurar bien, pero se puede poner abajo y j=1
    Npdf = i + j;//1,3,5,7
    Ntxt = i + j +1; //2,4,6,8
    err = pipe(pipe_pdf);// los hijos heredan las tuberias
    if (err ==-1) {
      perror("Error en tuberia");
      exit(EXIT_FAILURE);
    }
    //intentamos abrir el pdf(argv[i]) y el texto(argv[i+1]) para que no halla errores.
    ftxt =open(argv[Ntxt], O_WRONLY | O_CREAT, S_IRWXU);
    if (ftxt ==-1) {
      perror("Error al abrir/crear el documento de TXT");
      exit(EXIT_FAILURE);
    }//no hace falta cerrarlo porque lo va a usar luego el padre
    epdf = open(argv[Npdf], O_RDONLY);
    if (epdf ==-1) {
      perror("Error al abrir el documento de PDF");
      exit(EXIT_FAILURE);
    }
    close(epdf);//no hace falta, porque lo va a abrir el hijo, por tanto cierro.

    pid = fork();
    if (pid ==-1) {
      perror("Hijo no creado");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      printf("Soy el Hijo, con pid %d e ID: %d y mi padre es: %d\n", pid, getpid(), getppid());
      close(pipe_pdf[0]);

      close(1);
      dup(pipe_pdf[1]);
      close(pipe_pdf[1]);
      execlp("pdftotext", "pdftotext", argv[Npdf], "-", NULL);
    } else {
      //Soy el padre y voy a lleer de la tuberia e imprimir en el txt y en salida estandar
      close(pipe_pdf[1]);//cierro esta parte de la tuberia porque no la voy a usar.
      do {
        read_bytes = read(pipe_pdf[0], buffer, sizeof(buffer));
        write(ftxt, buffer, read_bytes);
        write(1, buffer, read_bytes);
      } while(read_bytes > 0);

    }
  }
  exit(EXIT_SUCCESS);
}
