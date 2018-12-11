#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  int fd, read_bytes = 1, i, pid, status;
  char buffer[1];
  char linea[80], argumento[80], comando[80];
  if (argc != 2) {
    perror("./p4_1 <fichero.txt con comando y un argumento>");
    exit(EXIT_FAILURE);
  }
  fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("Error al abrir el archivo");
    exit(EXIT_FAILURE);
  }
  do {
    //en el bucle voy a mandar cada linea al hijo, por cada linea un hijo
    for (i = 0; i < sizeof(linea); i++) {
      linea[i]='\0';
      comando[i] = '\0';
      argumento[i] ='\0';
    }

    for (i = 0; i < sizeof(linea); i++) { //leo hasta fin de linea
      read_bytes = read(fd, buffer, 1);
      if (buffer[0]!= '\n') {
        linea[i] = buffer[0];
      }else{
        break;
      }
    }
    sscanf(linea, "%s %s", comando, argumento);
    //por cada línea leida un hijo
    pid = fork();
    if ((pid == 0) && (read_bytes > 0)) {
      printf("Soy el hijo con ID: %d y mi padre tiene el ID: %d\n", getpid(), getppid());
      //No necesito tuberias porque hereda las variables y las ejecuta.
      execlp(comando, comando, argumento ,NULL);
      printf("Error al ejecutar el comando\n" );
      exit(EXIT_FAILURE);
    }else{
      wait(&status);
      printf("Hijo acabado con éxito\n");
    }

  } while(read_bytes > 0);
  exit(EXIT_SUCCESS);
}
