#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  int fd, fr, read_bytes = 1, i, pipe_bc_in[2], pipe_bc_out[2];
  char buffer[1];
  char operacion[80], comentario[80], resultado[80];
  pid_t pid;
  fd = open("operaciones_simple.txt", O_RDONLY);
  fr = open("resultados.txt", O_WRONLY |O_CREAT, S_IRWXU);
  if ((fd == -1) || (fr==-1)) {
    perror("Error al abrir el archivo");
    exit(EXIT_FAILURE);
  }

  do {
    //para que en cada linea los hijos las hereden abiertas
    pipe(pipe_bc_in);
    pipe(pipe_bc_out);
    //en el bucle voy a mandar cada linea al hijo
    for (i = 0; i < sizeof(resultado); i++) {
      operacion[i] = '\0';
      comentario[i] ='\0';
      resultado[i] ='\0';
    }
    for (i = 0; i < sizeof(resultado); i++) {
      read_bytes = read(fd, buffer, 1);
      //alamaceno las operaciones
      if ((buffer[0]!= '\n') && (buffer[0] != '/') && (buffer[0]!=' ')) {
        //pongo buffer[0]!=' ') y buffer[0] != '/' xq cuando divido se puede equivocar.
        operacion[i] = buffer[0];
      }else if((buffer[0]!= '\n') && (buffer[0] == '/')){
        //alamaceno los comentarios
        for (i = 0; i < sizeof(resultado); i++) {
          read_bytes = read(fd, buffer, 1);
          if (buffer[0] != '/') {
            comentario[i]=buffer[0];
          } else {
            break; //he llegado al fin del comentario
          }
        }
      }else{
        break;//he llegado a fin de linea
      }
    }
    if (strlen(operacion)!=0) {//no entindo porque me pasa strlen(operacion)==0
      pid = fork();
      if (pid == 0) {
        printf("Soy el hijo con ID: %d y mi padre tiene el ID: %d\n", getpid(), getppid());
        printf("Ejecuto la operacion: %s\n", operacion);
        close(pipe_bc_in[1]);
        close(0);
        dup(pipe_bc_in[0]);
        close(pipe_bc_in[0]);

        close(pipe_bc_out[0]);
        close(1);
        dup(pipe_bc_out[1]);
        close(pipe_bc_in[1]);

        execlp("bc", "bc", NULL);
        perror("Error al ejecutar BC");
        exit(EXIT_FAILURE);
      } else if (pid == -1) {
        perror("Error en el fork");
        exit(EXIT_FAILURE);
      } else {
        //Puedo cerrar aqui estos extremos porque cada linea es un hijo y las hereda abiertas
        close(pipe_bc_in[0]);
        close(pipe_bc_out[1]);

        write(pipe_bc_in[1], operacion, strlen(operacion));
        write(fr, operacion, strlen(operacion));
        write(fr, "=", 1);
        write(pipe_bc_in[1], "\n", 1); //para indicar a bc que ha terminado la operacion

        read_bytes = read(pipe_bc_out[0], resultado, sizeof(resultado));
        write(fr, resultado, strlen(resultado));
      }
    }else if ((strlen(comentario)!=0) ) {
      write(fr, comentario, strlen(comentario));
      write(fr, "\n", 1);
    }
  } while(read_bytes > 0);//ejecuto el codigo hasta fin de fichero
  exit(EXIT_SUCCESS);
}
