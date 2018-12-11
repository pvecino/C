#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {
  pid_t pid;
  int status;
  pid = fork();
  if (pid == -1) {
    perror("Error en el fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    printf("Soy el hijo con ID: %d y mi padre tiene el ID: %d\n", getpid(), getppid());
    execlp("ls", "ls", "-l", NULL);
    perror("Error al ejecutar ls");
    exit(EXIT_FAILURE);
  } else {
    wait(&status);
    printf("Hijo terminado con éxito\n");
  }
  exit(EXIT_SUCCESS);
}
