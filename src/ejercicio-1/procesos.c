#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>


#define BILLION 1000000000L

int main(){

	/* Variables y estructuras del programa */
	pid_t pid;
	struct timespec start, end;
	unsigned long accum;
	
	/* Registro tiempo inicial */
	if(clock_gettime(CLOCK_REALTIME, &start)<0){
		perror("Error en start");
	}
	
	if( (pid = fork() ) == 0 ){
		/* Estoy en el hijo */
		
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
		
		printf("Tiempo de creación en microsegundos desde el hijo: %lu \n",accum/1000);
		exit(0);
	}else 
	if ( pid < 0 ) {
		perror("Error al crear el proceso");
	}
	
	/* Estoy en el padre */
	
	wait(NULL);

	exit(0);
}
