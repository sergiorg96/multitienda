#include "macros.h"


int main(){

	key_t clave;
	sem_t * fruta, * pescado, * carne, * bebida, * sem_uso;
	char nombre[TAM_STR];
	char lista[10][TAM_STR ];
	int repuestos[10];
	int p=0, cantidad=0, i=0, j,shmid, introducidos;
	PRODUCTO * seg=NULL;
	carne=sem_open("carne",0);
	pescado=sem_open("pescado",0);
	fruta=sem_open("fruta",0);
	bebida=sem_open("bebida",0);
	if(fork()==0)
		execlp("clear","clear",NULL);
	else{
		wait(NULL);
		if(fruta==NULL||carne==NULL||pescado==NULL||bebida==NULL)
			printf("ERROR: No ha sido posible el uso de los semaforos.\n");
		else{
			for(j=0;p!=-1&&j<10;j++){
				printf("¿Qué desea reponer? Introduzca \"EXIT\" si desea salir. \n");
				scanf("%[^\n]", nombre);
				while(getchar()!='\n');
				nombre[0]=tolower(nombre[0]); 

				for (i=1,p=0;i<=4&&p==0;i++){
					p=comprueba(i,nombre);
				}

				if(p>0){
					do{
						printf("¿Qué cantidad de %s desea reponer?\n", nombre);
						scanf("%d",&cantidad);
						while(getchar()!='\n');
						if(cantidad<0)
							printf("Introduzca un número mayor o igual a 0.\n\n");
					}while(cantidad<0);

					strcpy(lista[j],nombre);
					lista[j][0]=toupper(lista[j][0]);
					repuestos[j]=cantidad;
					switch(p/10+1){
						case 1:
						clave=ftok("carnes.txt", 'R');
						sem_uso=carne;
						break;
						case 2:
						clave=ftok("pescados.txt", 'R');
						sem_uso=pescado;
						break;
						case 3:
						clave=ftok("frutas.txt", 'R');
						sem_uso=fruta;
						break;
						case 4:
						clave=ftok("bebidas.txt", 'R');
						sem_uso=bebida;
						break;
					}
					if((shmid=shmget(clave,TAM_MAX*sizeof(PRODUCTO),0))!=-1){
						if((seg=shmat(shmid,NULL,0))!=(PRODUCTO *)-1){
							for(introducidos=1;introducidos<=cantidad;introducidos++){
								sem_wait(sem_uso);
								seg[(p%10)-1].cantidad++;
								sleep(1);
								sem_post(sem_uso);
								printf("\t------Producto numero %d repuesto correctamente------\n", introducidos);
							}
							shmdt(seg);
							system("clear");
							printf("%s introducido correctamente.\n\n",lista[j]);
						}
						else
							printf("ERROR: No se ha podido mapear la zona de memoria.\n");
					}
					else
						printf("ERROR: No se ha podido acceder a la memoria compartida con id %d.\n", shmid);
				}
				else{ 
					if(p==0)
						printf("ERROR: Producto no reconocido.\n\n");
					j--;
				}
			}
			printf("\t\t------¡HASTA PRONTO!------\n\n");
			printf("\tLISTA DE PRODUCTOS REPUESTOS:\n");
			printf("\t---------------------------\n");
			for(i=0;i<j;i++)
				printf("\t|%-12s\t\t%2d|\n", lista[i], repuestos[i]);
			printf("\t---------------------------\n");
			sem_close(carne);
			sem_close(pescado);
			sem_close(fruta);
			sem_close(bebida);
		}
	}
	return 0;
}
