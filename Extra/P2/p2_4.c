#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char const *argv[]) {
  int read_bytes, i;
  char buffer[80];
  printf("Escribe una frase a repetir(Enter para terminar): \n");
  read_bytes = read(0, buffer, sizeof(buffer));
  printf("\nRepetición de la frase: \n");
  for (i = 0; i < 3; i++) {
    write(1, buffer, read_bytes);
  }
  exit(EXIT_FAILURE);
}
