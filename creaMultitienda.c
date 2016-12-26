#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <ctype.h>
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
} producto;

void leefichero(FILE *dfich, int idmc);

int creaMemoria(char *tipo);

int main()
{
	//Descriptor de fichero de lectura
	FILE *dfich;
	int i;
	//Tipo de producto
	char tipo[TAM_TIPO];
	int idsmc[NUM_TIPO_PROD];

	for (i=1; i<=NUM_TIPO_PROD; i++)
	{
		//Elige el tipo de alimento para abrir su fichero
		switch(i)
		{
			case CARNE:
				strcpy(tipo,"carnes.txt");
				break;
			case PESCADO:
				strcpy(tipo,"pescados.txt");
				break;
			case FRUTA:
				strcpy(tipo,"frutas.txt");
				break;
			case BEBIDAS:
				strcpy(tipo,"bebidas.txt");
				break;
		} 

		//Creamos la memoria compartida de cada uno de los tipos
		idsmc[i-1]=creaMemoria(tipo);

		if ((dfich=fopen(tipo, "r"))==NULL)
		{
			fprintf(stderr, "No ha podido abrirse el fichero %s \n",tipo);
		} else {
			leefichero(dfich, idsmc[i-1]);
			fclose(dfich);
		}
	}

	return 0;
}

void leefichero(FILE *dfich, int idmc)
{
	char nombre[TAM_STR];
	int cantidad;
	int i=0;
	float precio;
	//Linea de lectura del fichero
	char linea[TAM_LECTURA];
	//Puntero para el mapeado de direcciones
	producto *seg = NULL;
	//Mapeado de la dirección de memoria
	if (idmc != -1)
    {
    	if((seg=shmat(idmc,NULL,0))== (producto *)-1) 
			printf("Error al mapear el segmento\n"); 
      	else 
      	{
	  		while(feof(dfich)==0)
			{
				//Lee una línea del fichero
				fgets(linea,TAM_LECTURA,dfich);
				//Divide los parámetros de la línea
				sscanf(linea, "%s %d %f\n",nombre, &cantidad, &precio);
				//Los introduce en la memoria compartida
				strcpy(seg[i].nombre, nombre);
				seg[i].cantidad = cantidad;
				seg[i].precio = precio;
				i++; 
			}
			fprintf(stdout,"Nombre: %s Cantidad: %d Precio: %f\n",seg[1].nombre,seg[1].cantidad,seg[1].precio);
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

  	/* Creación de la memoria compartida MIRAR EL SIZEOF*/
  	if((idmc=shmget(clave,TAM_MC*sizeof(producto),IPC_CREAT|IPC_EXCL|0660))==-1) 
    { 
    	printf("Productos ya existentes en memoria\n"); 
    	if((idmc=shmget(clave,TAM_MC*sizeof(producto),0))==-1) 
			printf("Error al abrir la memoria de los productos\n"); 
    } 
  	else 
    	printf("Memoria creada de %s creada\n",tipo); 

	return idmc;
}