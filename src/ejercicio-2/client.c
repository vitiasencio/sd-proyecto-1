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
		
		/* Estoy en el hijo */
		if( execl(COMMAND,NULL) < 0 ){
			perror("Error en execl");
			exit(1);
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
		
	printf("Tiempo ejecución LOCAL con EXECL: %lu \n",accum/1000);
	
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
	
	printf("Tiempo ejecución LOCAL con SYSTEM: %lu \n",accum/1000);

}

void remote_calc_time(CLIENT *clnt){

	int  *result_1;
	char *calc_time_1_arg;

	result_1 = calc_time_1((void*)&calc_time_1_arg, clnt);
	
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");		
	}
	
	printf("Tiempo ejecución REMOTO con EXECL: %i \n",*result_1);
	
}

void remote_calc_time_system(CLIENT *clnt){

	int  *result_2;
	char *calc_time_system_1_arg;

	result_2 = calc_time_system_1((void*)&calc_time_system_1_arg, clnt);
	
	if (result_2 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	
	printf("Tiempo ejecución REMOTO con SYSTEM: %i \n",*result_2);
	
}

CLIENT * get_clnt(char *host){

	CLIENT *clnt;

	clnt = clnt_create (host, TIME, TIMEVERS, "udp");
	
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
	
	return clnt;
	
}

void destroy_clnt(CLIENT * clnt){

	clnt_destroy(clnt);
}

int main(int argc, char *argv[]) {
	
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	
	/* Obtengo el host que paso desde la terminal */
	host = argv[1];
	
	/* Creo el handler para cominicarme con el server */
	CLIENT * clnt = get_clnt(host);
	
	/* Procedimientos para tiempos locales */
	local_calc_time();
	local_calc_time_system();
	
	/* Procedimientos para tiempos remotos */
	remote_calc_time(clnt);
	remote_calc_time_system(clnt);
	
	/* Elimino la información de autenticación asociada a clnt */
	destroy_clnt(clnt);
	

	exit (0);
	
}
