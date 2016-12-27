#include "macros.h"


int main(){

	key_t clave;
	sem_t * fruta, * pescado, * carne, * bebida, * sem_uso;
	char nombre[TAM_STR];
	char lista[TAM_STR][10];
	int repuestos[10];
	int p=0, cantidad=0, i=0, j,shmid;
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
				scanf("%s", nombre);
				while(getchar()!='\n');
				nombre[0]=tolower(nombre[0]); 

				for (i=1,p=0;i<=4&&p==0;i++){
					p=comprueba(i,nombre);
				}

				if(p>0){
					printf("¿Qué cantidad de %s desea reponer?\n", nombre);
					scanf("%d",&cantidad);
					strcpy(lista[j],nombre);
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
							sem_wait(sem_uso);
							seg[(p%10)-1].cantidad=seg[(p%10)-1].cantidad+cantidad;
							sleep(3);
							sem_post(sem_uso);
							printf("\t------Producto repuesto correctamente------\n\n");
							shmdt(seg);
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
			printf("LISTA DE PRODUCTOS REPUESTOS:\n");
			for(i=0;i<j;i++)
				printf("%s...%d\n", lista[i], repuestos[i]);
			sem_close(carne);
			sem_close(pescado);
			sem_close(fruta);
			sem_close(bebida);
		}
	}
	return 0;
}
