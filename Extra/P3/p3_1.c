#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char const *argv[]) {
  int fentrada, fsalida, read_bytes;
  char buffer[1024];
  close(0);
  close(1);
  //cierro la entrada y salida estandar
  // fentrada y fsalida depende de los derechos que le des a los ficheros
  fentrada = open("origen.txt", O_RDONLY);
  fsalida = open("destino.txt", O_WRONLY | O_CREAT, S_IRWXU);
  if ((fentrada == -1)||(fsalida == -1)) {
    perror("Error al abrir/crear el archivo");
    exit(EXIT_FAILURE);
  }
  do {
    read_bytes = read(0, buffer, sizeof(buffer));
    write(1, buffer, read_bytes);
  } while(read_bytes > 0);
  return 0;
}
