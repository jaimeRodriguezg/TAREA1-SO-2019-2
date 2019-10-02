#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include<stdbool.h>

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
    dir = opendir("mazo"); //Abre directorio mazo
    cont = 0;
    pos = rand() % en_mazo; //genera numero aleatorio del total de valores que hay en el mazo
    while (cont<pos){
      temp=readdir(dir);
      carta = temp->d_name; //carta guarda el nombre de un archivo
      if (carta[0]!='.') cont++;
    }
    temp = readdir(dir);
    snprintf(aux1,sizeof(aux1),"mazo/%s",temp->d_name);
    snprintf(aux2,sizeof(aux2),"%s/%s",jugador,temp->d_name);
    rename(aux1,aux2); //mueve el archivo del aux1 al aux2
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
int comprobar_opcion(int max){
  int n;
  bool ciclo = true;
  while(ciclo){
    scanf("%d",&n);
    if (n > max || n <= 0){
      printf("Ingrese una opcion dentro del rango \n");
    }else {
      ciclo = false;
      return n;
    }
  }
}
  int cambiar_jugador(char jugador_actual[4][10], int jugador){
    int opcion;
    printf("el jugador actual es %s \n", &jugador_actual[jugador]);
    printf("Eliga el jugador: \n");
    for (int i = 0; i < 4; i++){
      printf("%d %s\n",i+1,&jugador_actual[i]);
    }
    opcion=comprobar_opcion(4);
    jugador=opcion-1;
}

int main(void){
  srand(time(NULL));
  char jugadores[4][10];
  char *aux;
  bool ciclo = true;
  int jugador=0;

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

  while(ciclo){
    int opcion;
    printf("El jugador actual es %s\n", jugadores[jugador]);
    printf("Desee la accion a realizar: \n");
    printf("1. Sacar carta \n2. Jugar carta \n3. Cambiar de jugador\n4. Salir\n");
    opcion = comprobar_opcion(5);
    if (opcion == 1){
      sacar_carta(jugadores[jugador],1);
      printf("El jugador %s ha sacado 1 carta \n", jugadores[jugador]);
    }if(opcion == 2){
      char var1[] = "3 amarillo";
      char var2[] = "3 rojo";
      jugar_carta(jugadores[jugador],var1, var2);
      printf("El jugador %s ha jugado \n",jugadores[jugador]);
    }if(opcion == 3){
      jugador=cambiar_jugador(jugadores,jugador);
    }if(opcion == 4){
      ciclo = false;
    }
  }

  //sacar_carta("jaime",2);

  //jugar_carta("jaime",var1,var2);

  return 0;
}
