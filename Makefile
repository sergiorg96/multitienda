todo: funciones.c reponedor.c creaMultitienda.c consumidor.c eliminaMultitienda.c
	gcc -W -Wall -o reponedor reponedor.c funciones.c -lpthread
	gcc -W -Wall -o crea creaMultitienda.c -lpthread
	gcc -W -Wall -o consumidor consumidor.c -lpthread
	gcc -W -Wall -o elimina eliminaMultitienda.c -lpthread
reponedor:	funciones.c reponedor.c creaMultitienda.c consumidor.c eliminaMultitienda.c
	gcc -W -Wall -o reponedor reponedor.c funciones.c -lpthread

crea: creaMultitienda.c
	gcc -W -Wall -o crea creaMultitienda.c -lpthread

consumidor: consumidor.c
	gcc -W -Wall -o consumidor consumidor.c -lpthread

elimina: eliminaMultitienda.c
	gcc -W -Wall -o elimina eliminaMultitienda.c -lpthread

