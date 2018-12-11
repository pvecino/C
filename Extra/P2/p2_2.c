#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  int foriginal, read_bytes, fcopia;
  char buffer[1024];
  foriginal = open("origen.txt", O_RDONLY);
  fcopia = open("destino.txt", O_WRONLY | O_CREAT, S_IRWXU);
  if ((foriginal == -1) || (fcopia == -1)) {
    perror("Error al abrir el fichero");
    exit(EXIT_FAILURE);
  }
  do {
    read_bytes = read(foriginal, buffer, sizeof(buffer));
    write(fcopia, buffer, read_bytes);
  } while(read_bytes > 0);
  printf("Archivo copiado correctamente\n");
  exit(EXIT_SUCCESS);
}
