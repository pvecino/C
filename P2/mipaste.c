#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  int pipe_to_sort[2], pipe_to_paste[2], fd, read_bytes, i;
  int epipe_paste, epipe_sort, efd1, efd2;
  char buffer[512];
  pid_t pid[2];//Voy a crear dos hijos
  if (argc != 4) {
    perror("./mipaste <fich1(debe exisistir)> <fich21(debe exisistir)> <fich3>");
    exit(EXIT_FAILURE);
  }
//creo las tuberias, para que las hereden los hijos abiertas
  epipe_sort = pipe(pipe_to_sort);
  epipe_paste = pipe(pipe_to_paste);
  if ((epipe_paste ==-1) || (epipe_sort ==-1)) {
    perror("Error al crear pipe_to_paste");
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < 2; i++) {//Creacion de los dos hijos
    pid[i]=fork();
    if (pid[i]==0) {
      printf("PID HIJO:%d; ID HIJO: %d; ID PADRE: %d\n", pid[i], getpid(), getppid());
      break;
    }else if (pid[i]==-1) {
      perror("Falla en el fork");
      exit(EXIT_FAILURE);
    }
  }
  efd1 = open(argv[1], O_RDONLY);
  efd2 = open(argv[2], O_RDONLY);
  if ((efd1 == -1) || (efd2 ==-1)) {
    perror("Error al abrir el fichero 1");
    exit(EXIT_FAILURE);
  }
  close(efd1);
  close(efd2);
  //Hijo1: es el que va a realizar el paste, no hace falta que se comunique
  //con el padre para coger los archivos-> copy on write
  if (pid[0]==0) {
    //cierro la tuberia pipe_to_sort
    close(pipe_to_sort[0]);
    close(pipe_to_sort[1]);
    //cierro la escritura(0) del pipe porque solo voy a mostrar(1) en ella.
    close(pipe_to_paste[0]);
    //hago la redireccion de la salida estandar hacia la salida de la pipe
    close(1);
    dup(pipe_to_paste[1]);
    close(pipe_to_paste[1]);
    //tengo que comprobar si argv[1-2] existen o sino crear fich2
    execlp("paste", "paste", argv[1], argv[2], NULL);
    perror("Error en el paste");
    exit(EXIT_FAILURE);
    //hijo 2: realizo el sort, el cual solo lee de la salida de la pipe_to_sort
    //y escribo en el pipe_to_paste para escribir en el fichero 3
  } else if (pid[1]==0) {
    //cierro la salida de pipe_to_sort porque voy a recibir de pipe_to_sort(0)
    close(pipe_to_paste[1]);
    //cierro la entrada estandar y la redirijo a la pipe_to_paste(0)
    close(0);
    dup(pipe_to_paste[0]);
    close(pipe_to_paste[0]);
    //cierro pipe_to_paste(0) escritura porque no la voy a utilizar
    close(pipe_to_sort[0]);
    // redirijo la salida estaNdar a la tuberia
    close(1);
    dup(pipe_to_sort[1]);
    close(pipe_to_sort[1]);
    execlp("sort", "sort", NULL);
    perror("Error em el sort");
    exit(EXIT_FAILURE);
  } else {
    //soy el padre que voy a recibir de mi hijo 2 por la tuberia pipe_to_paste
    //las demas las cierro
    close(pipe_to_paste[0]);
    close(pipe_to_paste[1]);
    //solo necesito la entrada de la tuberia para escribir en el fichero 3
    close(pipe_to_sort[1]);
    // este fichero no hace falta que lo hereden los hijos ya que solo voy a poner el resultado.
    fd = open(argv[3], O_WRONLY | O_CREAT, S_IRWXU);
    if (fd ==-1) {
      perror("Error al abrir/crear fichero 3");
      exit(EXIT_FAILURE);
    }
    do {
      read_bytes = read(pipe_to_sort[0], buffer, sizeof(buffer));
      write(fd, buffer, read_bytes);
    } while(read_bytes > 0);
    close(fd);
  }
  exit(EXIT_SUCCESS);
}
