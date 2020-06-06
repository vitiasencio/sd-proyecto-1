#include "array.h"
#include <time.h>

#define BILLION 1000000000L

/* Global variables for time measuring*/
struct timespec start, end;
unsigned long accum;

/*
	Funcion encargada de llenar la estructura request
	con valores aleatorios para los arreglos.
*/
request * create_req(){

	/* Estructuras para obtener un nsec para usar como semilla en srand */
	struct timespec aux;
		
	if(clock_gettime(CLOCK_REALTIME, &aux)<0){
		perror("Error en start");
	}

	srand(aux.tv_nsec);

	request *req = (struct request *) malloc(sizeof(struct request));
	
	int i;
	
	for(i=0; i<512;i++){
		req->arr1[i] = (int) (rand()%10);
		req->arr2[i] = (int) (rand()%10);
	}

	return req;
	
}

response * add_arr_1_local(request *req){
	
	int i;
	response *res = (struct response *) malloc(sizeof(struct response));
	
	for(i=0; i<512;i++){
		res->arr[i] = req->arr1[i] + req->arr2[i];
	}
	
	return res;
	
}

void arrayprog_1_local(){

	/* 
	*	Obtengo una request con numeros aleatorios 
	*	para llamar al procedimiento.
	*/
	request *req = create_req();


	/* Registro tiempo inicial */
	if(clock_gettime(CLOCK_REALTIME, &start)<0){
		perror("Error en start");
	}
	
	/* Llamada al procedimiento local */
	response *res = add_arr_1_local(req);

	/* Registro tiempo final */
	if(clock_gettime(CLOCK_REALTIME, &end)<0){
		perror("Error en end");
	}

	/* Obtengo el tiempo transcurrido en nanosegundos */
	if(end.tv_sec==start.tv_sec){
		accum = ( end.tv_sec - start.tv_sec ) * BILLION + ( end.tv_nsec - start.tv_nsec ); 
	}else{
		accum = ( end.tv_sec - start.tv_sec )* BILLION + (BILLION - start.tv_nsec) + end.tv_nsec ; 
	}
	
	printf("El tiempo de ejecución de la tarea LOCAL: %lu ns.\n", accum);

}

void arrayprog_1(CLIENT * clnt){

	response  *result_1;
	request *req = create_req();

	
	/* Registro tiempo inicial */
	if(clock_gettime(CLOCK_REALTIME, &start)<0){
		perror("Error en start");
	}
	
	/* Llamada al procedimiento remoto */
	result_1 = add_arr_1(req, clnt);

	/* Registro tiempo final */
	if(clock_gettime(CLOCK_REALTIME, &end)<0){
		perror("Error en end");
	}

	if (result_1 == (response *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	/* Obtengo el tiempo transcurrido en nanosegundos */
	if(end.tv_sec==start.tv_sec){
		accum = ( end.tv_sec - start.tv_sec ) * BILLION + ( end.tv_nsec - start.tv_nsec ); 
	}else{
		accum = ( end.tv_sec - start.tv_sec )* BILLION + (BILLION - start.tv_nsec) + end.tv_nsec ; 
	}
	
	printf("El tiempo de ejecución de la tarea REMOTA: %lu ns.\n", accum);

}

CLIENT * get_clnt(char *host){

	CLIENT *clnt;

	clnt = clnt_create (host, ARRAYPROG, ARRAYPROGVERS, "tcp");
	
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
	
	return clnt;
	
}

void destroy_clnt(CLIENT * clnt){

	clnt_destroy(clnt);
}


int main (int argc, char *argv[]){

	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}

	host = argv[1];
	
	/* Creo el handler para cominicarme con el server */
	CLIENT * clnt = get_clnt(host);

	arrayprog_1_local();
	
	arrayprog_1(clnt);
	
	/* Elimino la información de autenticación asociada a clnt */
	destroy_clnt(clnt);
	
	exit (0);

}
