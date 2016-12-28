#include "macros.h"

//Lectura y escritura colas
void escr_msg(int qid,struct mymsgbuf *qbuf){ 
   msgsnd(qid,qbuf,TAM_MAX*sizeof(PRODUCTO),0);
} 

void imprimir_msg(int qid,long type,struct mymsgbuf *qbuf){
	while(msgrcv(qid,qbuf,TAM_MAX*sizeof(PRODUCTO),type,IPC_NOWAIT)!=(-1)){
        printf("%s\t\t%d\t%f",qbuf->articulo.nombre,1,qbuf->articulo.precio);
    }
}

int main(){
	//Tiempo
	time_t tiempo;
	struct tm *fecha;
	//Proceso fork()
	pid_t pid;
	int estado_hijo;
	//Colas
	key_t clave_cola;
	int cola_ID;
	long tipo=1;
	struct mymsgbuf lista;
	//Variables
	int codigo; //Se usa para escoger tipo de producto,producto,volver y salir
	int elementos=0;//Numero de elementos que hemos tomado
	int precio_total=0;//Precio total de todos los productos
	//Memoria compartida
	key_t clave;
	int memoria_ID;
	PRODUCTO *seg=NULL; //Guardaremos lo que leamos de la memoria compartida
    //Semaforos para zona de memoria compartida
	sem_t *sem_uso=NULL;
	sem_t *carne=sem_open("carne",0);
	sem_t *pescado=sem_open("pescado",0);
	sem_t *fruta=sem_open("fruta",0);
	sem_t *bebida=sem_open("bebida",0);

    //Limpiamos pantalla

	do{
		printf("\t\tElija el tipo de producto deseado\n\n");
		printf("\t\tTIPO\t\t\tCODIGO\n\n");
		printf("\t\tCarne............................1\n");
		printf("\t\tPescado..........................2\n");
		printf("\t\tFruta............................3\n");
		printf("\t\tBebidas..........................4\n");
		printf("\t\tSalir............................0\n");
		printf("\t\tIntroduzca codigo: ");
		scanf("%d",&codigo);

		while((codigo<0)||(codigo>4)){
			printf("Codigo incorrecto. Introduzca un codigo correcto:");
			scanf("%d",&codigo);
		}
		pid=fork();
		switch (pid){
			case -1:
			perror("Error fork\n");
			break;
			case 0:
			execlp("clear","clear",NULL);
			perror("Error exec\n");
			break;
			default:
			if(codigo!=0){ //Si es codigo=0 es que el usuario quiere salir
				wait(&estado_hijo); //Esperamos a que finalice el hijo es decir a que se limpie la pantalla
				switch(codigo){
					case CARNE:
					printf("\t\tElija el producto deseado\n\n");
					printf("\t\tPRODUCTO\t\t\tCODIGO\n\n");
					printf("\t\tPollo............................1\n");
					printf("\t\tLomo.............................2\n");
					printf("\t\tTernera..........................3\n");
					printf("\t\tHamburguesa......................4\n");
					printf("\t\tCarne picada.....................5\n");
					printf("\t\tVolver...........................0\n");
					printf("\t\tIntroduzca codigo: ");
					scanf("%d",&codigo);

					while((codigo<0)||(codigo>5)){
						printf("Codigo incorrecto. Introduzca un codigo correcto:");
						scanf("%d",&codigo);
					}

					clave=ftok("carne",'S');
					sem_uso=carne;

					break;
					case PESCADO:
					printf("\t\tElija el producto deseado\n\n");
					printf("\t\tPRODUCTO\t\t\tCODIGO\n\n");
					printf("\t\tAcedias..........................11\n");
					printf("\t\tCazon............................12\n");
					printf("\t\tChoco............................13\n");
					printf("\t\tLenguado.........................14\n");
					printf("\t\tSardina..........................15\n");
					printf("\t\tVolver...........................0\n");
					printf("\t\tIntroduzca codigo: ");
					scanf("%d",&codigo);

					while(((codigo<11)||(codigo>15))&&(codigo!=0)){
						printf("Codigo incorrecto. Introduzca un codigo correcto:");
						scanf("%d",&codigo);
					}

					clave=ftok("pescado",'S');
					sem_uso=pescado;

					break;
					case FRUTA:
					printf("\t\tElija el producto deseado\n\n");
					printf("\t\tPRODUCTO\t\t\tCODIGO\n\n");
					printf("\t\tPlatano..........................21\n");
					printf("\t\tManzana..........................22\n");
					printf("\t\tUva..............................23\n");
					printf("\t\tPera.............................24\n");
					printf("\t\tSandia...........................25\n");
					printf("\t\tVolver...........................0\n");
					printf("\t\tIntroduzca codigo: ");
					scanf("%d",&codigo);

					while(((codigo<21)||(codigo>25))&&(codigo!=0)){
						printf("Codigo incorrecto. Introduzca un codigo correcto:");
						scanf("%d",&codigo);
					}

					clave=ftok("fruta",'S');
					sem_uso=fruta;

					break;
					case BEBIDAS:
					printf("\t\tElija el producto deseado\n\n");
					printf("\t\tPRODUCTO\t\t\tCODIGO\n\n");
					printf("\t\tCocacola.........................31\n");
					printf("\t\tFanta............................32\n");
					printf("\t\tAgua.............................33\n");
					printf("\t\tZumo.............................34\n");
					printf("\t\tRon..............................35\n");
					printf("\t\tVolver...........................0\n");
					printf("\t\tIntroduzca codigo: ");
					scanf("%d",&codigo);

					while(((codigo<31)||(codigo>35))&&(codigo!=0)){
						printf("Codigo incorrecto. Introduzca un codigo correcto:");
						scanf("%d",&codigo);
					}

					clave=ftok("bebida",'S');
					sem_uso=bebida;

					break;
				}
				if(codigo!=VOLVER){ 
					if((memoria_ID=shmget(clave,TAM_MAX*sizeof(PRODUCTO),0))!=-1){//Obtenemos zona de memoria
						if((seg=shmat(memoria_ID,NULL,0))!=(PRODUCTO *)-1){ //La mapeamos para poder usarla
							sem_wait(sem_uso); //Bajamos semaforo
                            if(seg[codigo%10].cantidad!=0){//Comprobamos que hay cantidad
                            	clave_cola=ftok("carrito",'M'); //Obtenemos clave cola
                            	if((cola_ID=msgget(clave_cola,IPC_CREAT|0660))==-1)
                            		printf("Error al iniciar coger el carrito\n");
                            	else{
                            		lista.mtype=tipo;// Va a ser tipo 1 siempre
                            		lista.articulo=seg[codigo%10];
                            		escr_msg(cola_ID,&lista);
                                    seg[codigo%10].cantidad--;//Decrementamos cantidad del producto
                                    precio_total+=seg[codigo%10].precio;
                                    elementos++; 
                                }
                            }
                            else{
                            	printf("Se ha agotado ese producto\n");
                            	printf("Seleccione otro\n");
                            }

                            sem_post(sem_uso);
                            shmdt(seg); //Desenlazamos memoria compartida
                        }
                        else
                        	printf("ERROR. No se ha podido mapear la zona de memoria compartida\n");
                    }
                    else
                    	printf("ERROR. No se ha podido acceder a la zona de memoria compartida\n");
                }
                else
                	codigo=1;
            }
            else{ 
            	system("clear"); //Para limpiar terminal
            	if(elementos==0)
            		printf("Salimos sin comprar productos\n");
            	else{
            		tiempo=time(NULL);
            		fecha=localtime(&tiempo);
            		system("clear"); //Limpiar pantalla
            		printf("Ticket de compra:");
            		printf("\t\tSUPERMERCADO MERCADONA\n");
            		printf("DIREC: Calle Santo Tomas de Aquino\n");
            		printf("TELEF: 956088445\n");
            		printf("EMAIL: frasgombel@gmail.com\n\n");
            		printf("Caja # %d     Ticket # 002-0000006\n",(elementos%10));
            		printf("****************************\n\n\n");
            		printf("ATENDIO: JESUS\n");
            		printf("CLIENTE: PUBLICO GENERAL\n\n");
            		printf("FECHA:%02d/%02d/%04d\t\tHORA:%02d:%02d:%02d\n",fecha->tm_mday, fecha->tm_mon, 
            			fecha->tm_year+1900,fecha->tm_hour, 
            			fecha->tm_min, fecha->tm_sec);
            		printf("****************************\n");
            		printf("ARTICULO\t\t|CANT|PRECIO\n");
            		printf("****************************\n");
            		imprimir_msg(cola_ID,tipo,&lista);
            		printf("============================\n");
            		printf("\t\tTOTAL.....$\t\t%d",precio_total);
            	}

            }
        }
    }while(codigo!=0);

    sem_close(carne);
    sem_close(pescado);
    sem_close(fruta);
    sem_close(bebida);

    return 0;
}
