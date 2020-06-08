#include "ejercicio7.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <rpc/rpc.h>
#include <time.h>


int main(int argc, char* argv[]){
	struct timeval t_inicial;
	struct timeval t_final;

	void*entrada;
	char*server;
	tiempo* reloj_server;

	long tiempo_cliente;
	long unsigned variacion;
	long tiempo_call;
	long tiempo_server;
	
	CLIENT *clnt;
	
	if(argc!=2){
		fprintf(stderr,"usage: %s hostname \n",argv[0]);
		exit(EXIT_FAILURE);
	}
	
	int i=0;
	for(i=1 ; i<argc ; i++){
		//obtengo el servidor i que paso desde la terminal.
		server = argv[i];
		
		clnt = clnt_create(server,EJERCICIO7,VERSION1,"tcp");

		if(clnt ==(CLIENT*)NULL){
			clnt_pcreateerror(server);
			exit(EXIT_FAILURE);
		}
		
		//llama al procedimiento remoto en el servidor
		gettimeofday(&t_inicial,NULL);
		//Obtiene los datos del reloj del servidor.
		reloj_server = reloj_1(&entrada,clnt);
		gettimeofday(&t_final,NULL);
		
		//Obtiene cuanto tardo el llamado remoto
		tiempo_call = ((t_final.tv_sec * 1000000) + t_final.tv_usec) - ((t_inicial.tv_sec*1000000) + t_inicial.tv_usec);
		
		// Obtiene el tiempo del servidor en este momento
		tiempo_server = (reloj_server->sec * 1000000) + (reloj_server->usec) + (tiempo_call*0.5);
		//Obtiene el reloj del ciente 
		tiempo_cliente = (t_final.tv_sec * 1000000) + t_final.tv_usec;
		
		//obtiene la diferencia en us
		variacion = tiempo_server - tiempo_cliente;
		
		printf("Los resultados del experimento fueron los siguientes: \n");
		printf("El reloj del cliente establecio un tiempo de: %lu microsegundos \n",tiempo_cliente);
		printf("El reloj del servidor establecio un tiempo de: %lu microsegundos \n",tiempo_server);
		printf("La llamada remota en microsegundos tardo: %lu microsegundos \n",tiempo_call);
		printf("Los resultados muestran una variacion de %lu microsegundos entre el reloj del servidor y el reloj del cliente \n",variacion);
		
		//libero recursos
		clnt_destroy(clnt);
	}
	

}
