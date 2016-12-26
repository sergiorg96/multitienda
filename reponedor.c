#include <stdio.h> 
#include <sys/shm.h> 
#include <string.h> 
#include <ctype.h> 
#include <semaphore.h>
#include "macros.h"
#include <fcntl.h>

int main(int argc, char *argv[]){

	key_t clave;
	sem_t * fruta, * pescado, * carne, * bebida, * sem_uso;
	char nombre[TAM_STR];
	int p=0, cantidad=0, i=0, shmid;
	producto * seg=NULL;
	carne=sem_open("carne",0);
	pescado=sem_open("pescado",0);
	fruta=sem_open("fruta",0);
	bebida=sem_open("bebida",0);

	if(mutex==NULL||fruta==NULL||carne==NULL||pescado==NULL||bebida==NULL)
		printf("ERROR: No ha sido posible el uso de los semaforos.\n");
	else{
		
		if(argc==0){
			printf("¿Qué desea reponer?\n");
			scanf("%s", nombre);
			for (i=1;i<=4&&p==0;i++){
				p=comprueba(i,nombre);
			}
			if(p!=0){
				printf("¿Qué cantidad de %s desea reponer?", nombre);
				scanf("%d",&cantidad);
				switch(p/10+1){
					case 1:
					clave=ftok("carne", 'S');
					sem_uso=carne;
					break;
					case 2:
					clave=ftok("pescado", 'S');
					sem_uso=pescado;
					break;
					case 3:
					clave=ftok("fruta", 'S');
					sem_uso=fruta;
					break;
					case 4:
					clave=ftok("bebida", 'S');
					sem_uso=bebida;
					break;
				}
				if((shmid=shmget(clave,TAM_MAX*sizeof(producto),0))!=-1){
					if((seg=shmat(shmid,NULL,0))==(producto *)-1){
						sem_wait(sem_uso);
						seg[p%10].cantidad=seg[p%10].cantidad+cantidad;
						sem_post(sem_uso);
						shmdt(seg);
					}
					else
						printf("ERROR: No se ha podido mapear la zona de memoria.\n");
				}
				else
					printf("ERROR: No se ha podido acceder a la memoria compartida con id %d.\n", shmid);
			}
			else
				printf("ERROR: Producto no reconocido.\n");
		}
		else
			printf("QUIZAS QUERAMOS QUE LEA DE FICHERO Y SIEMPRE FUNCIONANDO?\n");
		sem_close(carne);
		sem_close(pescado);
		sem_close(fruta);
		sem_close(bebida);
	}
	return 0;
}