#include "macros.h"

int main(){
  key_t clave; 
  int idmc, i; 
  char tipo[TAM_TIPO];
  char fichEscritura[TAM_FCHESCR];
  FILE *dfich;
  HILO hilos[NUM_TIPO_PROD];
  //Hilos
  pthread_attr_t attr;
  pthread_t thid[NUM_TIPO_PROD];

  //Iniciamos el atributo del hilo como no independiente
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  //Eliminamos todos los semáforos
  sem_unlink("carne");
  sem_unlink("pescado");
  sem_unlink("fruta");
  sem_unlink("bebida"); 

  //Creación de la carpeta para la actualización de valores
  if(mkdir("./actualización", 0700)==0)
  {
    fprintf(stdout,"Cambios guardados en la carpeta ./actualización\n");
  } else {
    fprintf(stderr,"Carpeta creada anteriormente.\n");
  }
  
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
    hilos[i].dfich=NULL;
    if((idmc=shmget(clave,TAM_MC*sizeof(PRODUCTO),0))==-1)
    {
      printf("No hay segmento de memoria creado.\n");
    } else {
      sprintf(fichEscritura,"./actualización/%s",tipo);
      if ((dfich=fopen(fichEscritura, "w"))==NULL)
      {
        fprintf(stderr, "No ha podido crearse el fichero %s\n",tipo);
      } else {
        hilos[i].idmc=idmc;
        hilos[i].dfich=dfich;
            //Creamos el hilo correspondiente a cada memoria
        pthread_create(&thid[i], &attr, escribefichero, &hilos[i]);
      }    
    }
  }
  pthread_attr_destroy(&attr);    

  //Bucle para cerrar los ficheros de lectura y espera a que termine el hilo
  for (i = 0; i < NUM_TIPO_PROD; i++)
  {
    if (hilos[i].dfich!=NULL) 
    {
      pthread_join(thid[i], NULL);
      //Eliminación de la memoria compartida
      shmctl(hilos[i].idmc,IPC_RMID,NULL);
      fclose(hilos[i].dfich);
    }
  }

  return 0;
}

void *escribefichero(void *datos)
{
  HILO *hilo=(HILO *)datos;
  int i;
  //Linea de lectura del fichero
  char linea[TAM_LECTURA];
  //Puntero para el mapeado de direcciones
  PRODUCTO *seg = NULL;
  //Mapeado de la dirección de memoria
  if (hilo->idmc != -1)
  {
    if((seg=shmat(hilo->idmc,NULL,0))== (PRODUCTO *)-1) 
      printf("Error al mapear el segmento.\n"); 
    else 
    {
      //Leemos hasta que termine el fichero
      for(i=0; i<TAM_MC; i++)
      {
        //Divide los parámetros de la línea
        sprintf(linea, "%s %d %.2f\n",seg[i].nombre, seg[i].cantidad, seg[i].precio);
        //Lee una línea del fichero
        fputs(linea,hilo->dfich); 
      }
      shmdt(seg);
    }
  }
  pthread_exit(NULL);
}
