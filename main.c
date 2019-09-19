#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void creacion_cartas(){
  char aux[30];
  char cartas[4][9] = {"azul","rojo","amarillo","verde"};
  char tipos[3][9] = {"+2","Reversa","Salto"};

  FILE *card;
  for(int i=0;i<4;i++) {
    snprintf(aux,sizeof(aux),"mazo/0 %s.txt",cartas[i]);
    card = fopen(aux,"w");
    fclose(card);
    for (int j=1;j<10;j++){
      snprintf(aux,sizeof(aux),"mazo/%d %s.txt",j,cartas[i]);
      card = fopen(aux,"w");
      fclose(card);
      snprintf(aux,sizeof(aux),"mazo/%d %s(1).txt",j,cartas[i]);
      card = fopen(aux,"w");
      fclose(card);
    }
    for (int j=0;j<3;j++){
      snprintf(aux,sizeof(aux),"mazo/%s %s.txt",tipos[j],cartas[i]);
      card = fopen(aux,"w");
      fclose(card);
      snprintf(aux,sizeof(aux),"mazo/%s %s(1).txt",tipos[j],cartas[i]);
      card = fopen(aux,"w");
      fclose(card);
    }
  }
  for (int i=0;i<4;i++){
    if (i==0) strcpy(aux,"mazo/Cambio color.txt");
    else  snprintf(aux,sizeof(aux),"mazo/Cambio color(%d).txt",i);
    card = fopen(aux,"w");
    fclose(card);
    if (i==0) strcpy(aux,"mazo/+4.txt");
    else  snprintf(aux,sizeof(aux),"mazo/+4(%d).txt",i);
    card = fopen(aux,"w");
    fclose(card);
  }
}

int main(void){
  char *aux;
  char jugadores[4][10];

  //Creacion de directorios
  mkdir("mazo",0700);
  mkdir("pila",0700);
  for (int i=0;i<4;i++){
    printf("Jugador %d: ",i+1);
    scanf("%s",jugadores[i]);
  }
  for (int i=0;i<4;i++) {
    mkdir(jugadores[i],0700);
  }
  creacion_cartas();
  return 0;
}
