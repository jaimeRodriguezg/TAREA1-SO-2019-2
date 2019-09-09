#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void){
  char *aux;
  char jugadores[4][10];
  for (int i=0;i<4;i++){
    printf("Jugador %d: ",i);
    scanf("%s",&jugadores[i]);
  }
  for (int i=0;i<4;i++) {
    strcpy(aux,"mkdir ");
    strcat(aux, jugadores[i]);
    system(aux);
  }
  return 0;
}
