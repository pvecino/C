#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
//********************************************
//Está mal-> no imprimi bien los comentarios que estan en la misma linea que operacion
//*******************************************
int main(int argc, char const *argv[]) {
  int fd, fr, read_bytes = 1, i, pipe_bc_in[2], pipe_bc_out[2], status;
  char buffer[1];
  char operacion[80], comentario[80], resultado[80], operando1[3], operando2[3], operador[3];
  pid_t pid;
  fd = open("operaciones_texto.txt", O_RDONLY);
  fr = open("resultados.txt", O_WRONLY |O_CREAT, S_IRWXU);
  if ((fd == -1) || (fr==-1)) {
    perror("Error al abrir el archivo");
    exit(EXIT_FAILURE);
  }
  pipe(pipe_bc_in);
  pipe(pipe_bc_out);
  do {
    //en el bucle voy a mandar cada linea al hijo
    for (i = 0; i < sizeof(resultado); i++) {
      operacion[i] = '\0';
      comentario[i] ='\0';
      resultado[i] ='\0';
    }
    for (i = 0; i < sizeof(operador); i++) {
      operando1[i] = '\0';
      operando2[i] ='\0';
      operador[i] ='\0';
    }
    for (i = 0; i < sizeof(resultado); i++) {
      read_bytes = read(fd, buffer, 1);
      //alamaceno las operaciones
      if ((buffer[0]!= '\n') && (buffer[0] != '/')) {
        //he tenido que quitar buffer[0]!=' ') xq hay espacios en operacion
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
      //***********************************************************************
      //Aqui tengo que procesar la linea y luego volver a juntarla
      sscanf(operacion, "%s %s %s", operando1, operador, operando2);
      if (operador[0]=='A') {
        operador[0]='+';
        operador[1]='\0';
        operador[2]='\0';
      } else if (operador[0]=='M') {
        operador[0]='*';
        operador[1]='\0';
        operador[2]='\0';
      } else if (operador[0]=='D') {
        operador[0]='/';
        operador[1]='\0';
        operador[2]='\0';
      }else if (operador[0]=='S') {
        operador[0]='-';
        operador[1]='\0';
        operador[2]='\0';
      }
      //**********************************************************************
      pid = fork();
      if (pid == 0) {
        printf("Soy el hijo con ID: %d y mi padre tiene el ID: %d\n", getpid(), getppid());
        printf("Ejecuto la operacion: %s %s %s\n", operando1, operador, operando2);
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
        //estos close sobran porque sino me da error en la lectura de la entrada estandar
        //close(pipe_bc_in[0]);
        //close(pipe_bc_out[1]);
        //***********************************************************************+

        write(pipe_bc_in[1], operando1, strlen(operando1));
        write(pipe_bc_in[1], operador, strlen(operador));
        write(pipe_bc_in[1], operando2, strlen(operando2));
        write(pipe_bc_in[1], "\n", 1); //para indicar a bc que ha terminado la operacion

        write(fr, operando1, strlen(operando1));
        write(fr, operador, strlen(operador));
        write(fr, operando2, strlen(operando2));
        write(fr, "=", 1);

        //***********************************************************************
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
