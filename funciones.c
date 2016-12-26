#include <ctype.h>
#include <string.h>
#include "macros.h"

int tipo(char * type){

	int t=0;

	if(strcmp(type,"Carne"))
		t=CARNE;
	else if(strcmp(type,"Pescado"))
		t=PESCADO;
	else if(strcmp(type,"Fruta"))
		t=FRUTA;
	else if(strcmp(type,"Bebida"))
		t=BEBIDAS;
	return t;
}

int comprueba(int tipo, char * name){
	int producto=0;
	switch(tipo){
		case CARNE:

		if(strcmp(name,"Pollo"))
			producto=POLLO;
		else if(strcmp(name,"Lomo"))
			producto=LOMO;
		else if(strcmp(name,"Ternera"))
			producto=TERNERA;
		else if(strcmp(name,"Hamburguesa"))
			producto=HAMBURGUESA;
		else if(strcmp(name,"Carne picada"))
			producto=CARNE_PICADA;

		break;

		case PESCADO:

		if(strcmp(name,"Acedias"))
			producto=ACEDIAS;
		else if(strcmp(name,"Cazon"))
			producto=CAZON;
		else if(strcmp(name,"Chocos"))
			producto=CHOCO;
		else if(strcmp(name,"Lenguado"))
			producto=LENGUADO;
		else if(strcmp(name,"Sardinas"))
			producto=SARDINA;

		break;

		case FRUTA:

		if(strcmp(name,"Platanos"))
			producto=PLATANO;
		else if(strcmp(name,"Manzanas"))
			producto=MANZANA;
		else if(strcmp(name,"Uvas"))
			producto=UVA;
		else if(strcmp(name,"Peras"))
			producto=PERA;
		else if(strcmp(name,"Sandias"))
			producto=SANDIA;

		break;
		
		case BEBIDAS:

		if(strcmp(name,"Coca cola"))
			producto=COCA_COLA;
		else if(strcmp(name,"Fanta"))
			producto=FANTA;
		else if(strcmp(name,"Agua"))
			producto=AGUA;
		else if(strcmp(name,"Zumo"))
			producto=ZUMO;
		else if(strcmp(name,"Ron"))
			producto=RON;

		break;
	}
	return producto;
}