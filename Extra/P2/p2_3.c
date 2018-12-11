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
  if (argc != 3) {
    perror("./p2_3 <Archivo Origen.txt> <Archivo Destino.txt>");
  }
  foriginal = open(argv[1], O_RDONLY);
  fcopia = open(argv[2], O_WRONLY | O_CREAT, S_IRWXU);
  if ((foriginal == -1) || (fcopia == -1)) {
    perror("Error al abrir el fichero");
    exit(EXIT_FAILURE);
  }
  printf("Argc: %d, Argv[]: %s | %s | %s\n", argc, argv[0], argv[1], argv[2]);
  do {
    read_bytes = read(foriginal, buffer, sizeof(buffer));
    write(fcopia, buffer, read_bytes);
  } while(read_bytes > 0);
  printf("Archivo copiado correctamente\n");
  exit(EXIT_SUCCESS);
}
