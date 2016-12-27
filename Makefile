reponedor:	funciones.c reponedor.c
	gcc -W -Wall -o reponedor reponedor.c funciones.c -pthread

crea: creaMultitienda.c
	gcc -W -Wall -o crea creaMultitienda.c -pthread

consumidor: consumidor.c
	gcc -W -Wall -o consumidor consumidor.c -pthread

elimina: eliminaMultitienda.c
	gcc -W -Wall -o elimina eliminaMultitienda.c -pthread

