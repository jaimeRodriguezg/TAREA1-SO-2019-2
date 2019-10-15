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
    char wea[30]="h0la",*skip,*carta, leido[20],enviar[30];
    int hijo = i,vecino,sentido=1;
    while (strcmp(wea,"cha0")!=0){
      if (read(pipes[hijo][0],wea,sizeof(char)*30)!=-1){
        vecino = atoi(strtok(wea,"|"));
        carta = strtok(NULL,"|");
        skip = strtok(NULL,"|");
        printf("LA CONCHA - %d\t%d\n",hijo,vecino);
        if (!hijo){
          if (vecino==3 && sentido<0) sentido = 1;
          if (vecino==1 && sentido>0) sentido = -1;
          }else{
          if (vecino>hijo && sentido>0) sentido=-1;
          else if (vecino<hijo && sentido<0) sentido=1;
        }
        if (!strcmp(skip,"si")){
          sprintf(enviar,"%d|%s|%s",hijo,carta,"no");
          if (!hijo && sentido<0)
            write(pipes[3][1],enviar,sizeof(char)*30);
          else
            write(pipes[hijo+sentido][1],wea,sizeof(char)*30);
        }
        else{
          sprintf(enviar,"%d|%s|%s",hijo,carta,"no");
          if (!hijo && sentido<0)
            write(pipes[3][1],enviar,sizeof(char)*30);
          else
            write(pipes[hijo+sentido][1],enviar,sizeof(char)*30);
        }
      }

    }
    close(pipes[hijo][0]);
    close(pipes[hijo][1]);
    exit(0);
  }
  if (p>0){
    char wea[20] = "h0la", leido[30]="3|+2 rojo|no";
    int  n_1;
    write(pipes[3][1],leido,sizeof(char)*30 );
    while (strcmp(leido,"cha0")){
      printf("NONONON\n" );
      if ( read(pipes[3][0],leido,30) != -1){
        printf("MENSAJE RECIBIDO: %s\n",leido);
        printf("Soy papa, enviaré = ");
        if (scanf("%s",wea )!=0)
          printf("¿Direccion?\n1.Derecha\n2.Izquierda\n>> ");
          scanf("%d",&n_1);
          if (n_1==1) {
            write(pipes[0][1],leido,sizeof(char)*30);
          }
          else    write(pipes[2][1],leido,sizeof(char)*30);

          sleep(4);
        }
    }
    close(pipes[3][1]);
    close(pipes[3][0]);

  }
  return 0;
}
