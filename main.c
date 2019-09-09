#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void){

  char jugadores[4][10];
  for (int i=0;i<4;i++){
    printf("Jugador %d: ",i);
    scanf("%s",&jugadores[i]);
  }
  for (int i=0;i<4;i++) printf("%s\n",jugadores[i]);
  return 0;
}
