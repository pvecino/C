#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
/*No me sale si pongo los argumentos*/
//cmp tetris.exe original.exe comprobar lineas

int main(int argc, char *argv[]) {
  /* Antes de ejecutar el código dar permisos a el fichero a modificar chomd 777 */

  int foriginal, read_bytes, fcopia, fmodificar;
  int bfind, nlifes, sizelifes;
  sizelifes = sizeof(nlifes);
  char lifes[sizelifes], buffer[2048];
  bfind = atoi(argv[2]);
  nlifes = atoi(argv[3]);

  if (argc == 4) {
    /* Aqui solo modficio el original */
    fmodificar = open(argv[1], O_RDWR);
    if (fmodificar ==-1) {
      perror("Error al abrir el archivo");
      exit(EXIT_FAILURE);
    }
    lseek(fmodificar, bfind, SEEK_SET);
    write(fmodificar, lifes, sizeof(nlifes));
    close(fmodificar);
  }else if (argc == 5) {
    /* Aqui primero copio todo a mi ot */
    foriginal = open(argv[1], O_RDWR);
    if (foriginal ==-1) {
      perror("Error al abrir el archivo");
      exit(EXIT_FAILURE);
    }
    fcopia = open(argv[4],O_RDWR | O_CREAT, S_IRWXU);
    if (fcopia ==-1) {
      perror("Error al abrir el archivo, voy a crearlo");
      exit(EXIT_FAILURE);
    }
    do {
      read_bytes = read(foriginal, buffer, sizeof(buffer));
      write(fcopia, buffer, read_bytes);
    } while(read_bytes > 0);
    lseek(fcopia, bfind, SEEK_SET);
    write(fcopia, lifes, sizeof(nlifes));
    close(fcopia);
    close(foriginal);
  }else{
    perror("tramposo.c <juego.exe> <byte de vidas> <vidas> <opt:mio.exe>");
    exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
