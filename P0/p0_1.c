#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 1) {
    perror("Demasiados Argumentos");
    exit(EXIT_FAILURE);
  }
  int n_sons = 5, i;
  int status, reload = 0;
  pid_t pid[n_sons];
  if (reload = 1) {
    for (;;){
      for (i=0; i < n_sons; i++) {
        pid[i]=fork();
        if (pid[i]==0) {
          /*printf("Hijo %d : %d\n", i, pid[i]);*/
          break;
        }else if (pid[i]==-1) {
          perror("Fallo en fork");
          exit(EXIT_FAILURE);
        }
      }
  /*HIjo que ejecuta xload*/
      if (pid[0]==0) {
        execlp("xload", "xload", NULL);
        perror("Error en hijo de xload");
        exit(EXIT_FAILURE);
  /*HIjo que ejecuta xeyes*/
      }else if (pid[1]==0){
        execlp("xeyes", "xeyes", NULL);
        perror("Error en hijo de xeyes");
        exit(EXIT_FAILURE);
  /*HIjo que ejecuta xlogo*/
      }else if (pid[2]==0){
        execlp("xlogo", "xlogo", NULL);
        perror("Error en hijo de xlogo");
        exit(EXIT_FAILURE);
    /*HIJO QUE EJECUTA XCALC*/
      }else if (pid[3]==0){
        execlp("xcalc", "xcalc", NULL);
        perror("Error en hijo de xcalc");
        exit(EXIT_FAILURE);
      }else if (pid[4]==0){
        execlp("xclock", "xclock","-update", "1", NULL);
        perror("Error en hijo de xclock");
        exit(EXIT_FAILURE);
      }else{/*PADRE*/
        reload = 0;
        printf("Para terminar el programa Ctrl+C %d\n", getppid());
        wait(&status);
        reload = 1;
      }
    }
  }
  exit(EXIT_SUCCESS);
}
