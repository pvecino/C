#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  pid_t pid[2];//voy a crear dos hijos
  int i;
  for (i = 0; i < 2; i++) {
    pid[i] = fork();
    if (pid[i]==-1) {
      //error
      perror("Error en el fork");
    } else if (pid[i] == 0) {
      //soy el hijos
      printf("SOy un hijo con ID: %d y mi padre tiene el ID: %d\n",getpid(), getppid());
      break;
    } else {
      printf("Soy el padre con ID: %d\n", getppid());
    }
  }
  exit(EXIT_SUCCESS);
}
