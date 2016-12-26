
#define TAM_MAX 5
#define TAM_STR 15
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

int tipo(char * type);
int comprueba(int tipo, char *name);
#endif

typedef struct{
	char nombre[TAM_STR];
	char tipo[TAM_STR];
	int cantidad;
	float precio;
} producto;