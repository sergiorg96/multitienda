#include "macros.h"

int comprueba(int tipo, char * name){
	int producto=0;
	if(strcmp(name,"eXIT")&&strcmp(name,"exit")){
		switch(tipo){
			case CARNE:

			if(!strncmp(name,"pollo",3))
				producto=POLLO;
			else if(!strncmp(name,"lomo",3))
				producto=LOMO;
			else if(!strncmp(name,"ternera",3))
				producto=TERNERA;
			else if(!strncmp(name,"hamburguesa",3))
				producto=HAMBURGUESA;
			else if(!strncmp(name,"carne picada",3))
				producto=CARNE_PICADA;

			break;

			case PESCADO:

			if(!strncmp(name,"acedias",3))
				producto=ACEDIAS;
			else if(!strncmp(name,"cazon",3))
				producto=CAZON;
			else if(!strncmp(name,"chocos",3))
				producto=CHOCO;
			else if(!strncmp(name,"lenguado",3))
				producto=LENGUADO;
			else if(!strncmp(name,"sardinas",3))
				producto=SARDINA;

			break;

			case FRUTA:

			if(!strncmp(name,"platanos",3))
				producto=PLATANO;
			else if(!strncmp(name,"manzanas",3))
				producto=MANZANA;
			else if(!strncmp(name,"uvas",3))
				producto=UVA;
			else if(!strncmp(name,"peras",3))
				producto=PERA;
			else if(!strncmp(name,"sandias",3))
				producto=SANDIA;

			break;

			case BEBIDAS:

			if(!strncmp(name,"coca-cola",3))
				producto=COCA_COLA;
			else if(!strncmp(name,"fanta",3))
				producto=FANTA;
			else if(!strncmp(name,"agua",3))
				producto=AGUA;
			else if(!strncmp(name,"zumo",3))
				producto=ZUMO;
			else if(!strncmp(name,"ron",3))
				producto=RON;

			break;
		}
	}
	else
		producto=-1;
	return producto;
}
