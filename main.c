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
  int n;

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

char *uno(char jugador_actual[4][10]){
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
    if (!cont){
      return jugador_actual[i];
    }
    if (cont==1){
      printf("El jugador %s tiene 1 carta en su mano\n",jugador_actual[i]);
    }

    closedir(mano);
  }
  return "no";
}

char* mostrar_discard(char *discard){
  int cont=1;
  char *carta,*aux2,wea[30];
  DIR* disc;
  struct dirent *aux;
  disc = opendir(discard);
  printf("\nDISCARD:\n");
  while(aux = readdir(disc)){
    carta = (aux->d_name);
    //printf("carta es %s\n", carta);
    if (carta[0]!='.'){
      aux2 = strcpy(wea,carta);
      strtok(aux2,".()");
      printf("%d. %s\n",cont,aux2);
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

char * turno(char jugadores[4][10],int actual, char* color, char *retorno){
  int opcion, opcion2, n_carta;
  char *disc, *carta_jugar,copia_carta[33], colores[4][10] = {"azul","rojo","verde","amarillo"};
  char *efecto, *colour;
  printf("JUGADOR ACTUAL: %s\n", jugadores[actual]);
  uno(jugadores);
  disc=mostrar_discard("discard");
  n_carta = mostrar_mano(jugadores[actual]);
  printf("\n¿Que desea hacer? \n");
  printf("1. Sacar carta \n2. Jugar carta \n3. Salir\n");
  opcion = comprobar_opcion(4);
  if (opcion == 1){
    sacar_carta(jugadores[actual],1);
    //limpiar();
    printf("El jugador %s ha sacado 1 carta \n\n", jugadores[actual]);
    strtok(disc,".()");
    sprintf(retorno,"%d|%s|%s",actual,disc,"sc");
    return retorno;
  }else if(opcion == 2){
    //limpiar();
    printf("\n¿Que carta desea jugar? \n");
    opcion2=comprobar_opcion(n_carta-1);
    carta_jugar=obtener_carta(jugadores[actual],opcion2);
    strcpy(copia_carta,carta_jugar);
    strtok(carta_jugar,".()");


    efecto = jugar_carta(jugadores[actual],copia_carta, disc, color);
    //if (!strcmp(efecto,"Reversa")) orden *= -1;
    if (!strcmp(efecto,"Salto"))
      sprintf(retorno,"%d|%s|%s",actual,carta_jugar,"si");
    else if (!strcmp(efecto,"+2")) {
      sprintf(retorno,"%d|%s|%s",actual,carta_jugar,"si");
    }
    else if (!strcmp(efecto,"Color")){
      printf("\n¿A que color desea cambiar?\n1. Azul\n2. Rojo\n3. Verde\n4. Amarillo\n");
      opcion2 = comprobar_opcion(5);
      strcpy(colour,colores[opcion2-1]);
      sprintf(retorno,"%d|%s %s|%s",actual,efecto,colour,"no");
    }
    else if (!strcmp(efecto,"+4")){
      printf("¿A que color desea cambiar?\n1. Azul\n2. Rojo\n3. Verde\n4. Amarillo\n");
      opcion2 = comprobar_opcion(5);
      strcpy(colour,colores[opcion2-1]);

      sprintf(retorno,"%d|%s %s|%s",actual,efecto,colour,"si");
    }
    else{
      strtok(carta_jugar,".()");
      sprintf(retorno,"%d|%s|%s",actual,carta_jugar,"no");
    }
    return  retorno;
  }else if(opcion == 3){
    return "FIN";
  }
}

int main(void){
  pid_t p=1;
  int pipes[4][2],i;
  char jugadores[4][10];
  char *efecto,*unop;
  char *aux;
  bool ciclo = true, hola =true;
  int jugador=0, orden = 1;

  char *wea,*skip,*carta, *color,enviar[30];
  int hijo,vecino,sentido=1;
  char toitriste[30],aux6[30];

  limpiar();
  printf("¡BIENVENIDO AL UNO!\n");
  srand(time(NULL));

  strcpy(unop,"no");
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
  sacar_carta("discard",1);

  for(int j=0;j<4;j++) pipe(pipes[j]);
  for(i=0;i<3;i++) {
    if (p>0) {
      p = fork();
    }
    if (!p) break;
  }
    if (p==0){
      hijo = i;
      if (!hijo && hola){
        efecto = mostrar_discard("discard");
        //limpiar();
        strtok(efecto,".()");
        sprintf(enviar,"%d|%s|no",3,efecto);
        write(pipes[3][1],enviar,sizeof(char)*30);
        hola = false;
        limpiar();
      }
      while(en_mazo && !strcmp(unop,"no")){
      if (read(pipes[hijo][0],toitriste,sizeof(char)*30)!=-1){
        limpiar();
        wea = strcpy(aux6,toitriste);
        vecino = atoi(strtok(wea,"|"));
        carta = strtok(NULL,"|");
        skip = strtok(NULL,"|");
        if (!hijo){
          if (vecino==3 && sentido<0) sentido = 1;
          if (vecino==1 && sentido>0) sentido = -1;
        }else{
          if (vecino>hijo && sentido>0) sentido=-1;
          else if (vecino<hijo && sentido<0) sentido=1;
        }
      if (!strcmp(skip,"si")){
        strtok(carta," ");
        wea = strtok(NULL, " ");
        if (!strcmp(carta,"+2")){
          printf("%s HA JUGADO %s Y TE HA CANCELADO EL TURNO\n",jugadores[vecino],carta);
          printf("HAS TENIDO QUE SACAR 2 CARTAS :C\n");
          sacar_carta(jugadores[hijo],2);
        }else if (!strcmp(carta,"+4")){
          printf("%s HA JUGADO %s Y TE HA CANCELADO EL TURNO\n",jugadores[vecino],carta);
          printf("HAS TENIDO QUE SACAR 4 CARTAS :C\n");
          printf("%s HA CAMBIADO EL COLOR A %s\n",jugadores[vecino],wea);
          sacar_carta(jugadores[hijo],4);
        }else
          printf("%s HA JUGADO %s %s Y TE HA CANCELADO EL TURNO\n",jugadores[vecino],carta,wea);

        printf("PRESIONE ALGO...\n");
        scanf("%d\n");

        sprintf(enviar,"%d|%s %s|%s",hijo,carta,wea,"ss");
        if (!hijo && sentido<0){
          printf("ENVIADO\t%s\n",wea);
          sleep(5);
          write(pipes[3][1],enviar,sizeof(char)*30);}
        else{
          write(pipes[hijo+sentido][1],enviar,sizeof(char)*30);
          printf("ENVIADO\t%s\n",wea);
          sleep(5);}

      }else{
        if (!strcmp(skip,"sc")){
          printf("EL JUGADOR %s HA SACADO CARTA\n",jugadores[vecino]);
        }
        else if (!strcmp(skip,"ss")){
          printf("EL JUGADOR %s HA SIDO SALTADO\n",jugadores[vecino]);
        }
        else{
          printf("EL JUGADOR %s HA JUGADO %s\n",jugadores[vecino],carta);
        }
        strtok(carta," ");
        color = strtok(NULL," ");
        aux = turno(jugadores,hijo,color,wea);
        if (!strcmp(wea,"FIN")){
          ciclo = false;
          //limpiar();
        }
        else{
          if (!hijo && sentido<0){
            printf("ENVIADO\t%s\n",aux);
            sleep(5);
            write(pipes[3][1],aux,sizeof(char)*30);}
          else{
            write(pipes[hijo+sentido][1],aux,sizeof(char)*30);}
        }
      }
    }
    unop = uno(jugadores);
      }
    }
  else{
    sleep(1);
    while(en_mazo && !strcmp(unop,"no")){
    if(read(pipes[3][0],toitriste,sizeof(char)*30)!=-1){
      limpiar();
      wea = strcpy(aux6,toitriste);
      vecino = atoi(strtok(wea,"|"));
      carta = strtok(NULL,"|");
      skip = strtok(NULL,"|");
      if (!strcmp(skip,"si")){
        if (!hola){
        printf("%s HA JUGADO %s Y TE HA CANCELADO EL TURNO\n",jugadores[vecino],carta);
        strtok(carta," ");
        wea = strtok(NULL, " ");
        if (!strcmp(carta,"+2")){
          printf("HAS TENIDO QUE SACAR 2 CARTAS :C\n");
          sacar_carta(jugadores[3],2);
        }if (!strcmp(carta,"+4")){
          printf("HAS TENIDO QUE SACAR 4 CARTAS :C\n");
          printf("%s HA CAMBIADO EL COLOR A %s\n",jugadores[vecino],wea);
          sacar_carta(jugadores[3],4);
        }
        sleep(2);

        sprintf(enviar,"%d|%s %s|%s",3,carta,wea,"ss");
        if (!vecino)
          write(pipes[2][1],enviar,sizeof(char)*30);
        else
        {printf("ENVIADO\t%s\n",enviar);
        sleep(5);
          write(pipes[0][1],enviar,sizeof(char)*30);}
        }hola = false;
      }else{
        if (!hola){
        if (!strcmp(skip,"sc")){
          printf("EL JUGADOR %s HA SACADO CARTA\n",jugadores[vecino]);
        }else if (!strcmp(skip,"ss")){
          printf("EL JUGADOR %s HA SIDO SALTADO\n",jugadores[vecino]);
        }
        else{
          printf("EL JUGADOR %s HA JUGADO %s\n",jugadores[vecino],carta);
        }
      }hola=false;
        strtok(carta," ");
        color = strtok(NULL," ");
        wea = turno(jugadores,3,color,wea);
        printf("%s\n", wea );
        if (!strcmp(wea,"FIN")){
          ciclo = false;
          //limpiar();
        }
        else{
          if (!vecino)
            write(pipes[2][1],wea,sizeof(char)*30);
          else{
            write(pipes[0][1],wea,sizeof(char)*30);

            }
        }
      }
    }
    unop = uno(jugadores);
    }
  }

  borrar(jugadores);
  return 0;
}
