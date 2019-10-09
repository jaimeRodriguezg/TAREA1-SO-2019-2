#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{

    pid_t   hijo;
    int  estado_hijo;
    int fd1[4][2];


    //iniciar la tuberia
    if(pipe(fd1[0])==-1 )
    {
        perror("pipe");
        exit(-1);
    }

    //crear un proceso hijo
    if ((hijo =fork()) == -1)
    {
        perror("fork");
        exit(-1);
    }
    if (hijo==0) //contexto para el hijo
    {
        char leer[100];


        while(strcmp(leer,"FIN")!=0)
        {
            //leer los nombres de los usuarios
            if(read(fd1[0][0], leer, sizeof(char)*100)!=-1)
            {
                //hacer tiempo

                //mensaje para el usuario
                printf("**hola papa** SE HA TERMINADO EL PROCESO PARA -->(%s)\n", leer);
            }
        }
        exit(0);

    }
    else //contexto para el padre
    {
        char  escribir[100];

        //mientras no se introuzca la palabra FIN
        while(strcmp(escribir,"FIN")!=0)
        {
            //introducir los nombres de los usuarios
            printf("\nIntroduzca un usuario = ");
            if(scanf("%s",&escribir)!=0)
            {
                //mandarle el nombre del usuario al hijo
                write(fd1[0][1],escribir,100);//
            }
        }

        sleep(2);
    }

    printf("\n");
    exit(0);
}
