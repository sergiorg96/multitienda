#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h> 

#define NUM_TIPO_PROD 4
#define CARNE 1
#define PESCADO 2
#define FRUTA 3
#define BEBIDAS 4
#define TAM_TIPO 20
#define TAM_STR 15
#define TAM_MC 5

typedef struct{
  char nombre[TAM_STR];
  int cantidad;
  float precio;
} PRODUCTO;

int main(){
  key_t clave; 
  int idmc, i;
  int *seg = NULL; 
  char tipo[TAM_TIPO];

  //Eliminamos todos los semáforos
  sem_unlink("carne");
  sem_unlink("pescado");
  sem_unlink("fruta");
  sem_unlink("bebida"); 
  
  for (i=0; i<NUM_TIPO_PROD; i++)
  {
      //Elige el tipo de alimento para abrir su fichero
    switch(i)
    {
      case CARNE-1:
      strcpy(tipo,"carnes.txt");
      break;
      case PESCADO-1:
      strcpy(tipo,"pescados.txt");
      break;
      case FRUTA-1:
      strcpy(tipo,"frutas.txt");
      break;
      case BEBIDAS-1:
      strcpy(tipo,"bebidas.txt");
      break;
    }
    /* Generación de la clave con ftok*/
    clave=ftok(tipo,'R'); 

    if((idmc=shmget(clave,TAM_MC*sizeof(PRODUCTO),0))==-1)
    {
      printf("No hay segmento de memoria creado\n");
    }
    else
    {
      if((seg=shmat(idmc,NULL,0))== (int *)-1) 
        printf("Error al mapear el segmento\n"); 
      else
      {
        shmctl(idmc,IPC_RMID,NULL);
        shmdt(seg);
      }
    }
  }

 return 0;
}