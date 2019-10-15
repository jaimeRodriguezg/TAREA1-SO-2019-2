#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
  char* wea,*mensaje,*skip,hola[]="1|+2 azul|si";
  wea = strtok(hola,"|");
  printf("%s\n",wea);
  mensaje = strtok(NULL,"|");
  printf("%s\n",mensaje);
  skip = strtok(NULL,"|");
  printf("%s\n", skip);
  return 0;
}
