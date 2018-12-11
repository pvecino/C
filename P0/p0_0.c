#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
int main (int argc, char *argv[])
{
  pid_t pid=0;
  int i, n, a = 1;
  if (argc != 2)
  {
    fprintf(stderr, "Uso: %s procesos\n", argv[0]);
    return 1;
  }
  n = atoi(argv[1]);
  for (i = 1; i < n; i++) {
    pid = fork();
    if (pid == 0) {
      a += i;/*a=a+i*/
      break;
    }else if (pid == -1){
      perror("Falla el fork");
    }
  }
  fprintf(stderr, "i:%d Proceso ID:%d Padre ID:%d Hijo ID:%d a=%d\n", i, getpid(), getppid(), pid, a);
  return 0;
}
