#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char const *argv[]) {
  int forigen, i, fdestino, read_bytes;
  char origen[30], destino[30], buffer[1024];
  for (i = 0; i < sizeof(origen); i++) {
    origen[i] ='\0';
    destino[i] ='\0';
  }
  printf("Escribe nombre fichero origen: ");
  scanf("%s", origen);
  printf("Escribe nombre fichero destino: ");
  scanf("%s", destino);
  forigen = open(origen, O_RDONLY);
  fdestino = open(destino, O_WRONLY | O_CREAT, S_IRWXU);
  if ((forigen == -1) || (fdestino == -1)) {
    perror("Error al abrir el archivo");
    exit(EXIT_FAILURE);
  }
  do {
    read_bytes = read(forigen, buffer, sizeof(buffer));
    write(fdestino, buffer, read_bytes);
  } while(read_bytes > 0);
  printf("Archivo copiado.\n");
  exit(EXIT_SUCCESS);
}
