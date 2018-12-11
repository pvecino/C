#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(int argc, char const *argv[]) {
  int i, pipe_bc_in[2], pipe_bc_out[2], status, read_bytes;
  char operacion[80], resultado[80], buffer[1];
  pid_t pid;
  for (i = 0; i < sizeof(resultado); i++) {
    operacion[i]='\0';
    resultado[i]='\0';
  }
  pipe(pipe_bc_in);
  pipe(pipe_bc_out);
  printf("Ingresa la operacion a realizar: ");
  scanf("%s", operacion);
  pid = fork();
  if (pid == 0) {
    printf("Soy el hijo con ID: %d y mi padre tiene el ID: %d\n", getpid(), getppid());
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
    close(pipe_bc_in[0]);
    close(pipe_bc_out[1]);

    write(pipe_bc_in[1], operacion, strlen(operacion));
    write(pipe_bc_in[1], "\n", 1); //para indicar a bc que ha terminado la operacion

    read_bytes = read(pipe_bc_out[0], resultado, sizeof(resultado));
    write(1, resultado, read_bytes);
  }

  exit(EXIT_SUCCESS);
}
