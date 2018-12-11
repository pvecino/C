#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char const *argv[]) {
  int sumando1, sumando2, resultado, pid;
  printf("Vamos a sumar dos valores: \n");
  printf("Introduzca primer sumando:");
  scanf("%d", &sumando1);
  printf("Introduzca segundo sumando:");
  scanf("%d", &sumando2);
  pid = fork();
  if (pid ==-1) {
    perror("Error en el fork");
    exit(EXIT_FAILURE);
  } else if (pid==0) {
    resultado = sumando1 + sumando2;
    _exit(resultado);
  } else {
    wait(&resultado);
    resultado = WEXITSTATUS(resultado);
    printf("%d + %d = %d\n", sumando1, sumando2, resultado);
  }
  exit(EXIT_SUCCESS);
}
