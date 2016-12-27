#include "macros.h"

#define VOLVER  0

int main(int argc, char * argv[]){

    int codigo;
    int i;
    int existe=1; //Inicializado a no existe 1
    int posicion=0;
    key_t clave;
    int memoria_ID;
    PRODUCTO *seg=NULL;
    PRODUCTO *lista=NULL;
    //Semaforos
    sem_t *sem_uso=NULL;
    sem_t *carne=sem_open("carne",0);
    sem_t *pescado=sem_open("pescado",0);
    sem_t *fruta=sem_open("fruta",0);
    sem_t *bebida=sem_open("bebida",0);

    system("clear"); //Limpiamos pantalla

    while(codigo==0){

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

        if(codigo!=0){

            system("clear"); //Limpiar pantalla

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

                    while((codigo<11)||(codigo>15)&&(codigo!=0){
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

                    while((codigo<21)||(codigo>25)&&(codigo!=0)){
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

                    while((codigo<31)||(codigo>35)&&(codigo!=0)){
                        printf("Codigo incorrecto. Introduzca un codigo correcto:");
                        scanf("%d",&codigo);
                    }

                    clave=ftok("bebida",'S');
                    sem_uso=bebidas;

                    break;
                }

                if(codigo!=VOLVER){
                    if((shmid=shmget(clave,TAM_MAX*sizeof(PRODUCTO),0))!=-1){
                        if((seg=shmat(shmid,NULL,0))!=(PRODUCTO *)-1){
                            sem_wait(sem_uso);

                            if(seg[codigo%10].cantidad!=0){//Comprobamos que hay cantidad

                                seg[codigo%10].cantidad--;//NO SE SI SE PUEDE PONER ASI 

                                for(i=0;i<posicion;i++){ //
                                    existe=strncmp(lista[i].nombre,seg[codigo%10].nombre,TAM_STR);
                                    if(existe==0){ //Esto significa que si existe
                                        lista[i].cantidad++;
                                        lista[i].precio=(seg[codigo%10].precio)*(lista[i].cantidad);
                                        i=posicion++; //Para que acabe el bucle
                                    }
                                }
                                if(existe!=0){//No existe
                                    lista[posicion].cantidad=1;
                                    strcpy(lista[posicion].nombre,seg[codigo%10].nombre);
                                    lista[posicion].precio=seg[codigo%10].precio;
                                    posicion++;
                                } 
                            }
                            else
                                printf("Se ha agotado ese producto\n");
                            
                            sem_post(sem_uso);
                            shmdt(seg); //Desenlazamos memoria compartida
                        }
                        else
                            printf("ERROR. No se ha podido mapear la zona de memoria compartida\n");
                    }
                    else
                        printf("ERROR. No se ha podido acceder a la zona de memoria compartida\n");
                }

            }
        else{ //Elegimos salir del supermercado y hay que hacer recuento del carrito e imprimirlo 
             // Aqui usaremos cola
            if(posicion==0)
                printf("Salimos sin comprar productos\n");
            else{
                
            }
        }
    
    }

    sem_close(carne);
    sem_close(pescado);
    sem_close(fruta);
    sem_close(bebida);

    return 0;
}
