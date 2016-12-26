#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <ctype.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
//#include <macros.h>

#define NUM_TIPO_PROD 4
#define TAM_LECTURA 100
#define TAM_TIPO 20
#define TAM_MC 5

#define CARNE 1
#define PESCADO 2
#define FRUTA 3
#define BEBIDAS 4
#define TAM_STR 15

typedef struct{
	char nombre[TAM_STR];
	int cantidad;
	float precio;
} PRODUCTO;

typedef struct{
	FILE *dfich;
	int idmc;	
}HILO;

void *leefichero(void *datos);

int creaMemoria(char *tipo);

int main()
{
	//Descriptor de fichero de lectura
	FILE *dfich;
	int i;
	//Tipo de producto
	char tipo[TAM_TIPO];
	//Tabla con la estructura de las memorias(ficheros de apertura e id de las memorias)
	HILO hilos[NUM_TIPO_PROD];
	//Semáforos de las distintas zonas de memoria
	sem_t *fruta, *pescado, *carne, *bebida;
	//Hilos
	pthread_attr_t attr;
	pthread_t thid[NUM_TIPO_PROD];

	//Iniciamos el atributo del hilo como no independiente
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	//Apertura de semáforos
	carne  =sem_open("carne",O_CREAT,0600,1);
	pescado=sem_open("pescado",O_CREAT,0600,1);
	fruta  =sem_open("fruta",O_CREAT,0600,1);
	bebida =sem_open("bebida",O_CREAT,0600,1);
    //Cerramos semáforos ya que no lo usa este código
	sem_close(carne);
	sem_close(pescado);
	sem_close(fruta);
	sem_close(bebida);
	//Bucle para lectura de ficheros y creación de memoria compartida	
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
		//Apertura del fichero de lectura
		if ((dfich=fopen(tipo, "r"))==NULL)
		{
			fprintf(stderr, "No ha podido abrirse el fichero %s \n",tipo);
		} else {
			//Creamos la memoria compartida de cada uno de los tipos
			hilos[i].idmc=creaMemoria(tipo);
			hilos[i].dfich=dfich;
			//Creamos el hilo correspondiente a cada memoria
			pthread_create(&thid[i], &attr, leefichero, &hilos[i]);
		}
	}
	//Bucle para cerrar los ficheros de lectura y espera a que termine el hilo
	for (i = 0; i < NUM_TIPO_PROD; i++)
	{
		if (hilos[i].dfich!=NULL) 
		{
			pthread_join(thid[i], NULL);
			fclose(hilos[i].dfich);
		}
	}

	return 0;
}

void *leefichero(void *datos)
{
	HILO *hilo=(HILO *)datos;
	char nombre[TAM_STR];
	int cantidad;
	int i=0;
	float precio;
	//Linea de lectura del fichero
	char linea[TAM_LECTURA];
	//Puntero para el mapeado de direcciones
	PRODUCTO *seg = NULL;
	//Mapeado de la dirección de memoria
	if (hilo->idmc != -1)
	{
		if((seg=shmat(hilo->idmc,NULL,0))== (PRODUCTO *)-1) 
			printf("Error al mapear el segmento\n"); 
		else 
		{
			//Leemos hasta que termine el fichero
			while(feof(hilo->dfich)==0)
			{
				//Lee una línea del fichero
				fgets(linea,TAM_LECTURA,hilo->dfich);
				//Divide los parámetros de la línea
				sscanf(linea, "%s %d %f\n",nombre, &cantidad, &precio);
				//Los introduce en la memoria compartida
				strcpy(seg[i].nombre, nombre);
				seg[i].cantidad = cantidad;
				seg[i].precio = precio;
				i++; 
			}
		}
	}
}

int creaMemoria(char *tipo)
{
	//Clave memoria compartida
	key_t clave;
 	//Identificador memoria compartida 
	int idmc;

  	/* Generación de la clave con ftok*/
	clave=ftok(tipo,'R');

  	/* Creación de la memoria compartida*/
	if((idmc=shmget(clave,TAM_MC*sizeof(PRODUCTO),IPC_CREAT|IPC_EXCL|0660))==-1) 
	{ 
		printf("Productos ya existentes en memoria\n"); 
		if((idmc=shmget(clave,TAM_MC*sizeof(PRODUCTO),0))==-1) 
			printf("Error al abrir la memoria de los productos\n"); 
	} 
	else 
		printf("Memoria creada de %s creada\n",tipo); 

	return idmc;
}