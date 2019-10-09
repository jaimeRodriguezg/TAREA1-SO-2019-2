#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include<stdbool.h>

int main(int argc, char const *argv[]) {
  pid_t p=1;
  int pipes[4][2], jugador=0,i;
  int pipes2[4][2];
  for(int j=0;j<4;j++) {
    pipe(pipes[j]);
    pipe(pipes2[j]);
  }
  for(i=0;i<3;i++) {
    if (p>0) {
      p = fork();
    }
    if (!p) break;
  }

  if (p==0){
    char wea[20]="h0la", leido[20],enviado[20];
    int direccion = 1,n_1,n_2,hijo = i;
    while (strcmp(wea,"cha0")!=0){
      if (read(pipes[i][0],leido,sizeof(char)*20) != -1){
        printf("Soy el hijo %d y recibí un mensaje desde mi izquierda\n",hijo+1);
        strcpy(wea,leido);
        sleep(1);
        write(pipes[i+1][1],leido,20);
      }

      if(read(pipes2[i+1][0],leido,sizeof(char)*20) != -1){
        printf("Soy el hijo %d y recibí un mensaje desde mi derecha\n",hijo+1);
        strcpy(wea,leido);
        sleep(1);
        write(pipes2[i][1],leido,20);
      }
    }
    close(pipes[hijo][0]);
    close(pipes[hijo][1]);
    exit(0);
  }
  if (p>0){
    char wea[20] = "h0la", leido[20];
    int  n_1, n_2;
    while (strcmp(leido,"cha0")){
      //if (n_1 = read(pipes[3][0],leido,20) != -1){
      printf("Soy papa, enviaré = ");
      if (scanf("%s",leido )!=0)
        printf("¿Direccion?\n1.Derecha\n2.Izquierda\n>> ");
        scanf("%d",&n_1);
        if (n_1==1) {
          write(pipes[0][1],leido,20);
        }
        else    write(pipes2[3][1],leido,20);

        sleep(4);
      //}
    }
    close(pipes[3][1]);
    close(pipes[3][0]);

  }
  return 0;
}
