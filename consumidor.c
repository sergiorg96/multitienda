#include "macros.h"

//Lectura y escritura colas
void escr_msg(int qid,struct mymsgbuf *qbuf){ 
   msgsnd(qid,qbuf,TAM_MAX*sizeof(PRODUCTO),0);
} 

void imprimir_msg(int qid,long type,struct mymsgbuf *qbuf){
	while(msgrcv(qid,qbuf,TAM_MAX*sizeof(PRODUCTO),type,IPC_NOWAIT)!=(-1)){
        printf("|%-12s",qbuf->articulo.nombre);
        printf("\t\t      %d   %.2f|\n",1,qbuf->articulo.precio);
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
	float precio_total=0;//Precio total de todos los productos
	int posicion; //Posicion en la zona de memoria compartida
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
    if(fruta!=NULL||carne!=NULL||pescado!=NULL||bebida!=NULL){
    	do{
    		system("clear");
    		printf("Elija el tipo de producto deseado\n\n");
    		printf("TIPO\t\t\tCODIGO\n\n");
    		printf("Carne............................1\n");
    		printf("Pescado..........................2\n");
    		printf("Fruta............................3\n");
    		printf("Bebidas..........................4\n");
    		printf("Salir............................0\n");
    		printf("Introduzca codigo: ");
    		scanf("%d",&codigo);

    		while((codigo<0)||(codigo>4)){
    			printf("\nCodigo incorrecto. Introduzca un codigo correcto:");
    			scanf("%d",&codigo);
    		}
    		pid=fork();
    		switch (pid){
    			case -1:
    			perror("Error fork\n");
                sleep(2);
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
    					printf("Elija el producto deseado\n\n");
    					printf("PRODUCTO\t\t\tCODIGO\n\n");
    					printf("Pollo............................1\n");
    					printf("Lomo.............................2\n");
    					printf("Ternera..........................3\n");
    					printf("Hamburguesa......................4\n");
    					printf("Carne picada.....................5\n");
    					printf("Volver...........................0\n");
    					printf("Introduzca codigo: ");
    					scanf("%d",&codigo);

    					while((codigo<0)||(codigo>5)){
    						printf("\nCodigo incorrecto. Introduzca un codigo correcto:");
    						scanf("%d",&codigo);
    					}

    					clave=ftok("carnes.txt", 'R');
    					sem_uso=carne;

    					break;
    					case PESCADO:
    					printf("Elija el producto deseado\n\n");
    					printf("PRODUCTO\t\t\tCODIGO\n\n");
    					printf("Acedias..........................11\n");
    					printf("Cazon............................12\n");
    					printf("Choco............................13\n");
    					printf("Lenguado.........................14\n");
    					printf("Sardina..........................15\n");
    					printf("Volver...........................0\n");
    					printf("Introduzca codigo: ");
    					scanf("%d",&codigo);

    					while(((codigo<11)||(codigo>15))&&(codigo!=0)){
    						printf("\nCodigo incorrecto. Introduzca un codigo correcto:");
    						scanf("%d",&codigo);
    					}

    					clave=ftok("pescados.txt", 'R');
    					sem_uso=pescado;

    					break;
    					case FRUTA:
    					printf("Elija el producto deseado\n\n");
    					printf("PRODUCTO\t\t\tCODIGO\n\n");
    					printf("Platano..........................21\n");
    					printf("Manzana..........................22\n");
    					printf("Uva..............................23\n");
    					printf("Pera.............................24\n");
    					printf("Sandia...........................25\n");
    					printf("Volver...........................0\n");
    					printf("Introduzca codigo: ");
    					scanf("%d",&codigo);

    					while(((codigo<21)||(codigo>25))&&(codigo!=0)){
    						printf("\nCodigo incorrecto. Introduzca un codigo correcto:");
    						scanf("%d",&codigo);
    					}

    					clave=ftok("frutas.txt", 'R');
    					sem_uso=fruta;

    					break;
    					case BEBIDAS:
    					printf("Elija el producto deseado\n\n");
    					printf("PRODUCTO\t\t\tCODIGO\n\n");
    					printf("Cocacola.........................31\n");
    					printf("Fanta............................32\n");
    					printf("Agua.............................33\n");
    					printf("Zumo.............................34\n");
    					printf("Ron..............................35\n");
    					printf("Volver...........................0\n");
    					printf("Introduzca codigo: ");
    					scanf("%d",&codigo);

    					while(((codigo<31)||(codigo>35))&&(codigo!=0)){
    						printf("\nCodigo incorrecto. Introduzca un codigo correcto:");
    						scanf("%d",&codigo);
    					}

    					clave=ftok("bebidas.txt", 'R');
    					sem_uso=bebida;

    					break;
    				}
    				if(codigo!=VOLVER){ 
    					if((memoria_ID=shmget(clave,TAM_MAX*sizeof(PRODUCTO),0))!=-1){//Obtenemos zona de memoria
    						if((seg=shmat(memoria_ID,NULL,0))!=(PRODUCTO *)-1){ //La mapeamos para poder usarla
    							sem_wait(sem_uso); //Bajamos semaforo
    							posicion=(codigo%10)-1;
                                if(seg[posicion].cantidad!=0){//Comprobamos que hay cantidad
                                	clave_cola=ftok("carrito",'M'); //Obtenemos clave cola
                                	if((cola_ID=msgget(clave_cola,IPC_CREAT|0660))==-1){
                                		printf("Error al iniciar coger el carrito\n");
                                        sleep(2);
                                    }
                                	else{
                                		lista.mtype=tipo;// Va a ser tipo 1 siempre
                                		lista.articulo=seg[posicion];
                                		escr_msg(cola_ID,&lista);
                                        seg[posicion].cantidad--;//Decrementamos cantidad del producto
                                        precio_total+=seg[posicion].precio;
                                        elementos++; 
                                    }
                                }
                                else{
                                	printf("Se ha agotado ese producto\n");
                                	printf("Seleccione otro\n");
                                    sleep(2);
                                }

                                sem_post(sem_uso);
                                shmdt(seg); //Desenlazamos memoria compartida
                            }
                            else{
                            	printf("ERROR. No se ha podido mapear la zona de memoria compartida\n");
                                sleep(2);
                            }
                        }
                        else{
                        	printf("ERROR. No se ha podido acceder a la zona de memoria compartida\n");
                            sleep(2);
                        }
                    }
                    else
                    	codigo=1;
                }
                else{ 
                	system("clear"); //Para limpiar terminal
                	if(elementos==0){
                		printf("Salimos sin comprar productos\n");
                        sleep(2);
                    }
                	else{
                		tiempo=time(NULL);
                		fecha=localtime(&tiempo);
                		system("clear"); //Limpiar pantalla
                		printf("Ticket de compra:\n");
                		printf(" _____________________________________\n");
                		printf("|\tSUPERMERCADO MERCADONA        |\n");
                		printf("|DIREC: Calle Santo Tomas de Aquino   |\n");
                		printf("|TELEF: 956088445                     |\n");
                		printf("|EMAIL: frasgombel@gmail.com          |\n|\t\t\t\t      |\n");
                		printf("|Caja # %d         Ticket # 002-0000006|\n",(elementos%10));
                		printf("|*************************************|\n|\t\t\t\t      |\n|\t\t\t\t      |\n");
                		printf("|ATENDIO: JESUS                       |\n");
                		printf("|CLIENTE: PUBLICO GENERAL             |\n|\t\t\t\t      |\n");
                		printf("|FECHA:%02d/%02d/%04d        HORA:%02d:%02d:%02d|\n",fecha->tm_mday, fecha->tm_mon, 
                			fecha->tm_year+1900,fecha->tm_hour, 
                			fecha->tm_min, fecha->tm_sec);
                		printf("|*************************************|\n");
                		printf("|ARTICULO                 |CANT|PRECIO|\n");
                		printf("|*************************************|\n");
                		imprimir_msg(cola_ID,tipo,&lista);
                		printf("|=====================================|\n");
                		printf("|\tSUBTOTAL.....$    %6.2f       |\n",precio_total);
                		printf("|\tIVA..........$    %6.2f       |\n",(precio_total*0.1));
                		printf("|\tTOTAL........$    %6.2f       |\n",precio_total+(precio_total*0.1));
                		printf(" ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");
                	}

                }
            }
        }while(codigo!=0);

        sem_close(carne);
        sem_close(pescado);
        sem_close(fruta);
        sem_close(bebida);
    }
    else{
        printf("ERROR: No ha sido posible el uso de los semaforos.\n");
        sleep(1);
    }

    return 0;
}
