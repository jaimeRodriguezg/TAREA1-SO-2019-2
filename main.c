#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int en_mazo = 108;

void crear_cartas(){
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
    if (i==0) strcpy(aux,"mazo/Color.txt");
    else  snprintf(aux,sizeof(aux),"mazo/Color(%d).txt",i);
    card = fopen(aux,"w");
    fclose(card);
    if (i==0) strcpy(aux,"mazo/+4.txt");
    else  snprintf(aux,sizeof(aux),"mazo/+4(%d).txt",i);
    card = fopen(aux,"w");
    fclose(card);
  }
}

void jugar_carta(char *jugador,char *carta,char *discard){
  char aux1[30],aux2[33];
  snprintf(aux2,sizeof(aux2),"discard/%s.txt",discard);
  remove(aux2);

  snprintf(aux1,sizeof(aux1),"%s/%s.txt",jugador,carta);
  snprintf(aux2,sizeof(aux2),"discard/%s.txt",carta);
  rename(aux1,aux2);

  if (strcmp(carta,"Color") && strcmp(carta,"+4")){
    char *tipo,*color,*tipo_d,*color_d;
    tipo = strtok(carta," ");
    color = strtok(NULL," ");
    tipo_d = strtok(discard," ");
    color_d = strtok(NULL," ");
    if (strcmp(tipo,tipo_d) && strcmp(color,color_d)) printf("huevo");
  }
}

int main(void){
  char jugadores[4][10];/*
  char *aux;
  DIR *dir;
  struct dirent *carta_mano;*/

  //Creacion de directorios
  mkdir("mazo",0700);
  mkdir("discard",0700);
  for (int i=0;i<4;i++){
    printf("Jugador %d: ",i+1);
    scanf("%s",jugadores[i]);
  }
  for (int i=0;i<4;i++) {
    mkdir(jugadores[i],0700);
  }
  crear_cartas();

  //mover carta de directorio
  rename("mazo/0 azul.txt","b/0 azul.txt");
  rename("mazo/0 rojo.txt","discard/0 rojo.txt");

  //funcionamiento de readdir
  /*
  dir = opendir("mazo");
  if (!dir) exit(1);
  else{int i=1;
  while ( (carta_mano = readdir(dir)) ) {
    aux = carta_mano->d_name;
    if (aux[0]!='.') {
      aux = strtok(aux,".");
      printf("%d-%s\n",i,aux);
      i++;
    }
  }
  closedir(dir);}*/
  char wea[]="0 azul";
  char wea2[]="0 rojo";
  jugar_carta("b",wea,wea2);
  return 0;
}
