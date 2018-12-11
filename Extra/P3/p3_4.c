#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  int read_bytes;
  char buffer[1024];
  printf("Este programa funciona como tee sin argumentos (Crtl +C para terminar)\n");
  do {
    read_bytes = read(0, buffer, sizeof(buffer));
    write(1, buffer, read_bytes);
  } while(read_bytes > 0);
  return 0;
}
