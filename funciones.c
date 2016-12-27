#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "macros.h"

int comprueba(int tipo, char * name){
	int producto=0;
	if(strcmp(name,"eXIT")){
		switch(tipo){
			case CARNE:

			if(!strcmp(name,"pollo"))
				producto=POLLO;
			else if(!strcmp(name,"lomo"))
				producto=LOMO;
			else if(!strcmp(name,"ternera"))
				producto=TERNERA;
			else if(!strcmp(name,"hamburguesa"))
				producto=HAMBURGUESA;
			else if(!strcmp(name,"carne picada"))
				producto=CARNE_PICADA;

			break;

			case PESCADO:

			if(!strcmp(name,"acedias"))
				producto=ACEDIAS;
			else if(!strcmp(name,"cazon"))
				producto=CAZON;
			else if(!strcmp(name,"chocos"))
				producto=CHOCO;
			else if(!strcmp(name,"lenguado"))
				producto=LENGUADO;
			else if(!strcmp(name,"sardinas"))
				producto=SARDINA;

			break;

			case FRUTA:

			if(!strcmp(name,"platanos"))
				producto=PLATANO;
			else if(!strcmp(name,"manzanas"))
				producto=MANZANA;
			else if(!strcmp(name,"uvas"))
				producto=UVA;
			else if(!strcmp(name,"peras"))
				producto=PERA;
			else if(!strcmp(name,"sandias"))
				producto=SANDIA;

			break;

			case BEBIDAS:

			if(!strcmp(name,"coca-cola"))
				producto=COCA_COLA;
			else if(!strcmp(name,"fanta"))
				producto=FANTA;
			else if(!strcmp(name,"agua"))
				producto=AGUA;
			else if(!strcmp(name,"zumo"))
				producto=ZUMO;
			else if(!strcmp(name,"ron"))
				producto=RON;

			break;
		}
	}
	else
		producto=-1;
	return producto;
}
