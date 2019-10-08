#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include<stdbool.h>
#ifdef _WIN32
  #include<windows.h>
#endif

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
char *jugar_carta(char *jugador,char *carta,char *discard, char* cambio_color){
  char aux1[30],aux2[33];
  char name[30],name_d[30];
  char *res;
  int n;/*
  strcpy(name,carta);
  strcpy(name_d,discard);
  n=comprobar_color(name,name_d);
  if(n==1){
    printf("Es valida la carta puede jugarla\n");
  }if(n==2){
    printf("puede jugarla porque la ultima carta fue un +2\n" );
  }else{
    printf("No es valida la carta no puede jugarla\n" );
  }
  */
  sprintf(aux2,"discard/%s",discard);
  remove(aux2);

  sprintf(aux1,"%s/%s",jugador,carta);
  sprintf(aux2,"discard/%s",carta);
  rename(aux1,aux2);

  strtok(carta,".()");
  strtok(discard,".()");
  //comprobar_color(name,name_d);
  if (strcmp(carta,"Color") && strcmp(carta,"+4")){
    char *tipo,*color,*tipo_d,*color_d,*colorf,*colorf_d;
    if (!strcmp(discard,"Color") || !strcmp(discard,"+4")){
      tipo = strtok(carta," ");
      color = strtok(NULL, " ");
      if (strcmp(color,cambio_color)) {
        printf("La carta jugada no coincide con el color indicado por el jugador anterior\n");
        sacar_carta(jugador,1);
      }
    }
    else{
      tipo = strtok(carta," ");
      color = strtok(NULL," ");
      tipo_d = strtok(discard," ");
      color_d = strtok(NULL," ");
      if (strcmp(tipo,tipo_d) && strcmp(color,color_d)) {
        printf("La carta ha sido mal jugada, %s debió sacar 1 carta\n",jugador);
        sacar_carta(jugador,1);
      }
    }
    return tipo;
  }
  else return carta;
}



int mostrar_mano(char* jugador){
  int cont=1;
  char *carta,*aux2;
  DIR* mano;
  struct dirent *aux;
  mano = opendir(jugador);
  printf("\nMANO:\n");
  while(aux = readdir(mano)){
    carta = (aux->d_name);
    if (carta[0]!='.'){
      aux2 = strtok(carta,".()");
      printf("%d. %s\n",cont,carta);
      cont++;
    }
  }
  closedir(mano);
  return cont;
}

void limpiar(){
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
  #endif
}

int comprobar_opcion(int max){
  int n;
  bool ciclo = true;
  while(ciclo){
    printf(">> ");
    scanf("%d",&n);
    if (n > max || n <= 0){
      printf("Ingrese una opcion dentro del rango \n");
    }else {
      ciclo = false;
      return n;
    }
  }
}

int cambiar_jugador(int jugador, int orden){
    if (!jugador && orden<0) jugador = 3;
    else if (jugador == 3 && orden >0) jugador = 0;
    else jugador += orden;

    return jugador;
}

void borrar(char jugadores[4][10]){
  DIR *dir;
  struct dirent *carta;
  char direccion[40], actual[10];
  for (int i=0;i<6;i++){
    if (i<4) {dir = opendir(jugadores[i]); strcpy(actual,jugadores[i]);}
    else if (i == 4) {dir = opendir("mazo"); strcpy(actual,"mazo");}
    else {dir = opendir("discard"); strcpy(actual, "discard");}

    while ((carta = readdir(dir)))
    {
        // build the path for each file in the folder
        sprintf(direccion, "%s/%s", actual, (carta->d_name));
        remove(direccion);
    }
    closedir(dir);
    rmdir(actual);
  }
}

void uno(char jugador_actual[4][10]){
  int cont,opcion;
  char *carta;
  DIR* mano;
  struct dirent *aux;
  for(int i=0; i < 4; i++){
    cont=0;
    mano = opendir(jugador_actual[i]);
    while(aux = readdir(mano)){
      carta = (aux->d_name);
      if (carta[0]!='.'){
        cont++;
      }
    }
    if (cont==1){
      printf("El jugador %s tiene 1 carta en su mano\n",jugador_actual[i]);
    }
    closedir(mano);
  }

}

char* mostrar_discard(char *discard){
  int cont=1;
  char *carta,*aux2;
  DIR* disc;
  struct dirent *aux;
  disc = opendir(discard);
  printf("\nDISCARD:\n");
  while(aux = readdir(disc)){
    carta = (aux->d_name);
    //printf("carta es %s\n", carta);
    if (carta[0]!='.'){
      //aux2 = strtok(carta,".()");
      printf("%d. %s\n",cont,carta);
      return carta;

      cont++;
    }
  }
  closedir(disc);


}

char* obtener_carta(char *jugador,int opcion){
   int cont=1;
   char *carta,*aux2;
   DIR* mano;
   struct dirent *aux;
   mano = opendir(jugador);
   while(aux = readdir(mano)){
     carta = (aux->d_name);
     if (carta[0]!='.'){
       //aux2 = strtok(carta,".()");
       if (cont == opcion){
         break;
       }
       cont++;
     }
   }
   closedir(mano);
   return carta;
}

int comprobar_color(char *carta,char *discard){
  char colores[4][9]={"rojo","amarillo","azul","verde"};
  char *token;
  char *aux;
  char *ret, *ret2;
  int comparar;

  token=strtok(carta,".()");
  aux=strtok(discard,".()");

  for (int i=0; i<4; i++){
    ret=strstr(token,colores[i]);
    ret2=strstr(aux,colores[i]);
    if(ret!= NULL && ret2  != NULL){
      return 1;
      break;

    }
  }
  token=strtok(carta," ");
  aux=strtok(discard," ");

  comparar=strcmp(aux,token);
  if (comparar==0){
    comparar=strcmp(aux,"+2");
    if(comparar==0){
      return 2; //significa que hay +2
    }
    return 1; // puede jugar porque son el mismo numero
  }
  return 0;
}

int main(void){
  limpiar();
  printf("¡BIENVENIDO AL UNO!\n");
  srand(time(NULL));
  char jugadores[4][10], colores[4][10] = {"azul","rojo","verde","amarillo"};
  char discard[]="discard", *efecto;
  char *aux,*disc,*carta_jugar,color[10];
  bool ciclo = true;
  int opcion,opcion2,jugador=0,n_carta, orden = 1;

  //Creacion de directorios
  mkdir("mazo",0700);
  mkdir("discard",0700);
  for (int i=0;i<4;i++){
    printf("Jugador %d: ",i+1);
    scanf("%s",jugadores[i]);
  }
  limpiar();
  crear_cartas();
  for (int i=0;i<4;i++) {
    mkdir(jugadores[i],0700);
    sacar_carta(jugadores[i],7);
  }
  sacar_carta(discard,1);

  while(en_mazo && ciclo){
    printf("Es el turno de %s\n", jugadores[jugador]);
    uno(jugadores);
    disc=mostrar_discard(discard);
    n_carta = mostrar_mano(jugadores[jugador]);
    printf("\n¿Que desea hacer? \n");
    printf("1. Sacar carta \n2. Jugar carta \n3. Salir\n");
    opcion = comprobar_opcion(4);
    if (opcion == 1){
      sacar_carta(jugadores[jugador],1);
      limpiar();
      printf("El jugador %s ha sacado 1 carta \n\n", jugadores[jugador]);
    }else if(opcion == 2){
      //limpiar();
      printf("\n¿Que carta desea jugar? \n");
      opcion2=comprobar_opcion(n_carta-1);
      carta_jugar=obtener_carta(jugadores[jugador],opcion2);

      efecto = jugar_carta(jugadores[jugador],carta_jugar, disc, color);
      if (!strcmp(efecto,"Reversa")) orden *= -1;
      else if (!strcmp(efecto,"Salto")) jugador = cambiar_jugador(jugador, orden);
      else if (!strcmp(efecto,"+2")) {
        jugador = cambiar_jugador(jugador,orden);
        printf("\n%s debe sacar 2 cartas y pierde su turno.\n",jugadores[jugador]);
        sacar_carta(jugadores[jugador],2);
      }
      else if (!strcmp(efecto,"Color")){
        printf("\n¿A que color desea cambiar?\n1. Azul\n2. Rojo\n3. Verde\n4. Amarillo\n");
        opcion2 = comprobar_opcion(5);
        strcpy(color,colores[opcion2-1]);
      }
      else if (!strcmp(efecto,"+4")){
        jugador = cambiar_jugador(jugador,orden);
        sacar_carta(jugadores[jugador],4);
        printf("¿A que color desea cambiar?\n1. Azul\n2. Rojo\n3. Verde\n4. Amarillo\n");
        opcion2 = comprobar_opcion(5);
        strcpy(color,colores[opcion2-1]);
      }
      //printf("El jugador %s ha jugado \n",jugadores[jugador]);
      //printf("el valor de la carta elegida es %d\n",n_carta );
    }else if(opcion == 3){
      ciclo = false;
      limpiar();
    }
  jugador = cambiar_jugador(jugador, orden);
  limpiar();
  }
  borrar(jugadores);
  return 0;
}
