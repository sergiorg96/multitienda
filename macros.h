#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>

#define TAM_MAX 5
#define TAM_STR 15
//Defines añadidos
#define NUM_TIPO_PROD 4
#define TAM_LECTURA 100
#define TAM_TIPO 20
#define TAM_MC 5
#define TAM_FCHESCR 40
//Hasta aquí
//CARNES
#define CARNE 1
#define POLLO 1
#define LOMO 2
#define TERNERA 3
#define HAMBURGUESA 4
#define CARNE_PICADA 5
//PESCADOS
#define PESCADO 2
#define ACEDIAS 11
#define CAZON 12
#define CHOCO 13
#define LENGUADO 14
#define SARDINA 15
//FRUTA
#define FRUTA 3
#define PLATANO 21
#define MANZANA 22
#define UVA 23
#define PERA 24
#define SANDIA 25
//BEBIDAS
#define BEBIDAS 4
#define COCA_COLA 31
#define FANTA 32
#define AGUA 33
#define ZUMO 34
#define RON 35

#ifndef FUNCIONES_H
#define FUNCIONES_H

int comprueba(int tipo, char *name);
//Funciones de creaMultitienda.c y eliminaMultitienda.c
void *leefichero(void *datos);
void *escribefichero(void *datos);
int creaMemoria(char *tipo);
#endif

//Typedef añadidos de creaMultitienda.c
typedef struct{
	char nombre[TAM_STR];
	int cantidad;
	float precio;
} PRODUCTO;

typedef struct{
	FILE *dfich;
	int idmc;	
}HILO;
