/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "time.h"
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>


#define COMMAND "./tarea"
#define BILLION 1000000000L

/* Global variables for time measuring*/
struct timespec start, end;
unsigned long accum;


void local_calc_time(){

	pid_t pid;

	/* Registro tiempo inicial */
	if(clock_gettime(CLOCK_REALTIME, &start)<0){
		perror("Error en start");
	}
	
	if( (pid = fork() ) == 0 ){
		
		if( execl(COMMAND,NULL) < 0 ){
			perror("Error en execl");
			exit(-1);
		}
		
	}

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
		
	printf("Tiempo ejecución en local con execl: %lu \n",accum);
	
	wait(NULL);
	
	
}

void local_calc_time_system(){

	/* Registro tiempo inicial */
	if(clock_gettime(CLOCK_REALTIME, &start)<0){
		perror("Error en start");
	}

	system(COMMAND);

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
	
	printf("Tiempo ejecución en local con System: %lu \n",accum);

}

void time_1(char *host){

	CLIENT *clnt;

	clnt = clnt_create (host, TIME, TIMEVERS, "udp");


	int  *result_1;
	char *calc_time_1_arg;
	int  *result_2;
	char *calc_time_system_1_arg;

	
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
	
	result_1 = calc_time_1((void*)&calc_time_1_arg, clnt);
	
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	
	result_2 = calc_time_system_1((void*)&calc_time_system_1_arg, clnt);
	
	if (result_2 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	
	printf("numero system remoto: %i\n", *result_2);
	
	clnt_destroy (clnt);
	
}


int main(int argc, char *argv[]) {
	
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	
	host = argv[1];

	local_calc_time();
	local_calc_time_system();
	
	//time_1 (host);

	exit (0);
	
}
