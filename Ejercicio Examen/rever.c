#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[]) {
   // variables de uso
   int pipe_wc[2], pipe_rev[2], frev[2], i, Nsons =2, read_bytes;
   char buffer[1024];
   pid_t pid[Nsons]; // voy a crear dos hijos
   // variables de control
   int epipe_wc, epipe_rev;
   if (argc!=2) {
      perror("./rever 'nombre fichero revertido' < 'fichero a revertir'");
      exit(EXIT_FAILURE);
   }
   epipe_wc=pipe(pipe_wc);
   epipe_rev=pipe(pipe_rev);
   if ((epipe_wc==-1) || (epipe_rev==-1)) {
      perror("Error al crear tuberias");
      exit(EXIT_FAILURE);
   }
   //los hijos lo tiene que heredar abierto
   frev[1]= open(argv[1], O_WRONLY | O_CREAT, S_IRWXU);
   //**************************************
   //aqui habia un fallo: if(foriginal==-1){-> se me habia olvidado cambiarlo
   //**************************************
   if (frev[1] ==-1) {
      perror("Error al abrir/crear los archivos");
      exit(EXIT_FAILURE);
   }
   for (i = 0; i < Nsons; i++) {
      pid[i]=fork();
      if (pid[i]==-1) {
         perror("Error al crear los hijos");
         exit(EXIT_FAILURE);
      } else if (pid[i]==0) {
         printf("Soy el hijo con ID: %d y mi padre tiene un ID: %d\n", getpid(), getppid());
         break;
      }
   }
   if (pid[0]==0) {
      //este es el hijo que va a realizar rev, entra por la entrada estandar
      //el fichero a revertir y su salida estandar la vuelco en el fichero
      //puedo hacer esto, porque los hijos lo heredan abierto.
      close(1);
      dup(frev[1]);
      close(frev[1]);

      close(0);
      dup(pipe_rev[0]);
      close(pipe_rev[0]);

      //cierro las tuberias que no voy a usar
      close(pipe_rev[1]);
      close(pipe_wc[1]);
      close(pipe_wc[0]);

      execlp("rev", "rev", NULL);
      perror("Error al ejecutar rev");
      exit(EXIT_FAILURE);
   } else if (pid[1]==0) {
      //Este es el hijo que va a hacer wc
      //voy a leer de mi entrada estandar el fichero, me lo pasa el padre por
      //la tuberia
      close(0);
      dup(pipe_wc[0]);
      close(pipe_wc[0]);

      //cierro las tuberias que no voy a usar
      close(pipe_wc[1]);
      close(pipe_rev[1]);
      close(pipe_rev[0]);

      execlp("wc", "wc", "-w", NULL);
      perror("Error al ejecutar wc");
      exit(EXIT_FAILURE);
   } else {
      close(pipe_wc[0]);
      close(pipe_rev[0]);
      //mientras leo de la entrada estandar voy escribiendo en los dos extremos
      //de las tuberias de los hijos
      do {
         read_bytes= read(0, buffer, sizeof(buffer));
         if (read_bytes==-1) {
            perror("Error con el fichero de entrada");
            exit(EXIT_FAILURE);
         }
         write(pipe_rev[1], buffer, read_bytes);
         write(pipe_wc[1], buffer, read_bytes);
      } while(read_bytes>0);
   }
   exit(EXIT_SUCCESS);
}
