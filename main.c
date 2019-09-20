#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
srand(time(NULL));
//hacer un numero aleatorio entre 0 y en_mazo, de esta manera, recorro las cartas
//(readdir) del mazo hasta el numero, y saco esa carta para el jugador
int en_mazo = 106;

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

//Funcion sacar_carta: mueve n carta(s) aleatoria(s) desde el mazo a la mano de jugador
void sacar_carta(char *jugador,int n){
  char *carta,aux1[30],aux2[30];
  int i=0,pos,cont;
  DIR *dir;
  struct dirent *temp;
  while (i<n){
    dir = opendir("mazo");
    cont = 0;
    pos = rand() % en_mazo;
    while (cont<pos){
      temp=readdir(dir);
      carta = temp->d_name;
      if (carta[0]!='.') cont++;
    }
    temp = readdir(dir);
    snprintf(aux1,sizeof(aux1),"mazo/%s",temp->d_name);
    snprintf(aux2,sizeof(aux2),"%s/%s",jugador,temp->d_name);
    rename(aux1,aux2);
    i++;
    en_mazo--;
    closedir(dir);
  }
}

//Funcion jugar_carta: revisa que la carta jugada por el jugador sea correcto jugarla
//ya sea siendo negra, o coincidiendo en color o tipo. En caso de no serla, el jugador
//debe sacar una carta.
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
    if (strcmp(tipo,tipo_d) && strcmp(color,color_d)) sacar_carta(jugador,1);
  }
}

int main(void){
  char jugadores[4][10];
  char *aux;/*

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

  return 0;
}
