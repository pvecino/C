#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  int fd, read_bytes;
  char buffer[1024];
  fd = open("p2_1.txt", O_RDONLY);
  if (fd == -1) {
    perror("Error al abrir el fichero");
    exit(EXIT_FAILURE);
  }
  do {
    read_bytes = read(fd, buffer, sizeof(buffer));
    write(1, buffer, read_bytes);
  } while(read_bytes > 0);
  exit(EXIT_SUCCESS);
}
