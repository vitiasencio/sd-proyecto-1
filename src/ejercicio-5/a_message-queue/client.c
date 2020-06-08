#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <time.h>

#include "protocol.h"

int req_q_id;
int res_q_id;

/* Variables para controlar el tiempo. */
struct timeval start, end;
unsigned long accum;

void create_req_queue();
void create_res_queue();
void print_header();
void print_options();
void terminate_server();
void handle_addition();
void handle_substraction();
void handle_product();
void handle_division();
void handle_dectobin();
void handle_bintohexa();

void record_init_time(){

	/* Registro tiempo inicial */
	if(gettimeofday(&start,NULL)<0){
		perror("Error en start");
	}
	
}

void record_end_time(){

	/* Registro tiempo final */
	if(gettimeofday(&end,NULL)<0){
		perror("Error en end");
	}

	/* Obtengo el tiempo transcurrido en nanosegundos */
	if(end.tv_sec==start.tv_sec){
		accum = ( end.tv_sec - start.tv_sec ) * MILLION + ( end.tv_usec - start.tv_usec ); 
	}else{
		accum = ( end.tv_sec - start.tv_sec )* MILLION + (MILLION - start.tv_usec) + end.tv_usec ; 
	}

}

int main() {
	
	create_req_queue();
	create_res_queue();
	
	pid_t pid;
	
	/*
	*	Creo un proceso con la imagen ejecutable del "server"
	*	donde se encuentran todos los procedimientos necesarios.
	*/
	if( (pid = fork() ) == 0 ){
		/* Estoy en el hijo */
		if( execl("./server",NULL) < 0 ){
			perror("Error en execl");
			exit(1);
		}
	}
	
	print_header();
	
	int op;
	
	char retry = 'x';
	
	while(retry!='n'){
		fflush(stdin);
		print_options();
		scanf("%i",&op);
		
		switch(op){
			case 1:
				handle_dectobin();
				break;
			case 2:
				handle_bintohexa();
				break;
			case 3:
				handle_addition();
				break;
			case 4:
				handle_substraction();
				break;
			case 5:
				handle_product();
				break;
			case 6:
				handle_division();
				break;
			case 7:
				retry='n';
				break;
			default:
				break;
		}
		
		int i=0;
		while(retry!='y' && retry!='n'){
			printf("\nDesea realizar otra operación? (y/n): ");
			fflush(stdin);
			scanf(" %c",&retry);
		}

		if(retry=='n'){
			terminate_server();
		}else if( retry=='y' ){
			retry='x';
		}
		
	}
	
	
	/**
	*	Elimino las colas de mensajes
	*/
	
	if( msgctl(req_q_id, IPC_RMID, NULL) < 0 ){
		perror("No se pudo eliminar la cola");
		exit(1);
	}
	if( msgctl(res_q_id, IPC_RMID, NULL) < 0 ){
		perror("No se pudo eliminar la cola");
		exit(1);
	}
	
	exit(0);
}

void handle_dectobin(){
	
	my_msgbuf buf;
	my_msgbuf_res buf_rcv;

	int cant_op = 0;
	long decbin;
	
	printf("Ingrese un numero positivo en decimal: ");
	scanf("%ld",&decbin);
	
	buf.decbin = decbin;
	buf.op_type = 1;
	buf.cant_operands = 1;

	record_init_time();

	/* Envio la request */
	if( msgsnd(req_q_id, &buf, TAMCOLA, 0) < 0 ){
		perror("Error al enviar");
	}
	
	/* Espero por la respuesta */
	if( msgrcv(res_q_id, &buf_rcv, TAMCOLARES, 0, 0) < 0 ){
		perror("Error al recibir");
	}else{
		printf("Resultado en binario: %s\n", buf_rcv.conv_res);
	}
	
	record_end_time();
	
	printf("Tiempo de ejecución: %lu microsegundos.\n",accum);
	
}

void handle_bintohexa(){
	
	my_msgbuf buf;
	my_msgbuf_res buf_rcv;

	int cant_op = 0;
	long decbin;
	
	printf("Ingrese un numero en binario: ");
	scanf("%ld",&decbin);
	
	buf.decbin = decbin;
	buf.op_type = 2;
	buf.cant_operands = 1;

	record_init_time();

	/* Envio la request */
	if( msgsnd(req_q_id, &buf, TAMCOLA, 0) < 0 ){
		perror("Error al enviar");
	}
	
	/* Espero por la respuesta */
	if( msgrcv(res_q_id, &buf_rcv, TAMCOLARES, 0, 0) < 0 ){
		perror("Error al recibir");
	}else{
		printf("Resultado en hexadecimal: %s\n", buf_rcv.conv_res);
	}
	
	
	record_end_time();
	
	printf("Tiempo de ejecución: %lu microsegundos.\n",accum);
}

void handle_addition(){

	my_msgbuf buf;
	my_msgbuf_res buf_rcv;

	int cant_op = 0;
	int i;
	float aux[4];
	
	printf("Ingrese el número de operandos: ");
	scanf("%i",&cant_op);
	
	while( (cant_op<2) || (cant_op>4) ){
		printf("Ingrese un número entre 2 y 4: ");
		scanf("%i",&cant_op);
	}
	
	for(i=0;i<cant_op;i++){
		printf("Ingrese operando: ");
		scanf("%f",&aux[i]);
	}
	
	for(i=0;i<cant_op;i++){
		buf.operands[i] = aux[i];
	}

	
	buf.op_type = 3;
	buf.cant_operands = cant_op;

	record_init_time();

	/* Envio la request */
	if( msgsnd(req_q_id, &buf, TAMCOLA, 0) < 0 ){
		perror("Error al enviar");
	}
	
	/* Espero por la respuesta */
	if( msgrcv(res_q_id, &buf_rcv, TAMCOLARES, 0, 0) < 0 ){
		perror("Error al recibir");
	}else{
		float res;
		if(buf_rcv.is_neg){
			res = buf_rcv.float_res*(-1);
		}else{
			res = buf_rcv.float_res;
		}
		printf("Resultado de la suma: %.2f\n", res);
	}

	
	record_end_time();
	
	printf("Tiempo de ejecución: %lu microsegundos.\n",accum);

}

void handle_substraction(){

	my_msgbuf buf;
	my_msgbuf_res buf_rcv;

	int cant_op = 0;
	int i;
	float aux[4];
	
	printf("Ingrese el número de operandos: ");
	scanf("%i",&cant_op);
	
	while( (cant_op<2) || (cant_op>4) ){
		printf("Ingrese un número entre 2 y 4: ");
		scanf("%i",&cant_op);
	}
	
	for(i=0;i<cant_op;i++){
		printf("Ingrese operando: ");
		scanf("%f",&aux[i]);
	}
	
	for(i=0;i<cant_op;i++){
		buf.operands[i] = aux[i];
	}

	
	buf.op_type = 4;
	buf.cant_operands = cant_op;

	record_init_time();

	/* Envio la request */
	if( msgsnd(req_q_id, &buf, TAMCOLA, 0) < 0 ){
		perror("Error al enviar");
	}
	
	/* Espero por la respuesta */
	if( msgrcv(res_q_id, &buf_rcv, TAMCOLARES, 0, 0) < 0 ){
		perror("Error al recibir");
	}else{
		float res;
		if(buf_rcv.is_neg){
			res = buf_rcv.float_res*(-1);
		}else{
			res = buf_rcv.float_res;
		}
		printf("Resultado de la resta: %.2f\n", res);
	}

	record_end_time();
	
	printf("Tiempo de ejecución: %lu microsegundos.\n",accum);

}

void handle_product(){

	my_msgbuf buf;
	my_msgbuf_res buf_rcv;

	int cant_op = 2;
	int i;
	float aux[2];
	
	for(i=0;i<cant_op;i++){
		printf("Ingrese operando: ");
		scanf("%f",&aux[i]);
	}
	
	for(i=0;i<cant_op;i++){
		buf.operands[i] = aux[i];
	}

	buf.op_type = 5;
	buf.cant_operands = cant_op;

	record_init_time();

	/* Envio la request */
	if( msgsnd(req_q_id, &buf, TAMCOLA, 0) < 0 ){
		perror("Error al enviar");
	}
	
	/* Espero por la respuesta */
	if( msgrcv(res_q_id, &buf_rcv, TAMCOLARES, 0, 0) < 0 ){
		perror("Error al recibir");
	}else{
		float res;
		if(buf_rcv.is_neg){
			res = buf_rcv.float_res*(-1);
		}else{
			res = buf_rcv.float_res;
		}
		printf("Resultado del producto: %.2f\n", res);
	}
	
	record_end_time();
	
	printf("Tiempo de ejecución: %lu microsegundos.\n",accum);
	
}

void handle_division(){

	my_msgbuf buf;
	my_msgbuf_res buf_rcv;

	int cant_op = 2;
	int i;
	float aux[2];
	
	printf("Ingrese dividendo: ");
	scanf("%f",&aux[0]);
	
	printf("Ingrese divisor: ");
	scanf("%f",&aux[1]);

	while(aux[1]==0){
		printf("El divisor no puede ser cero.\n");
		printf("Ingrese divisor: ");
		scanf("%f",&aux[1]);
	}
	
	for(i=0;i<cant_op;i++){
		buf.operands[i] = aux[i];
	}

	buf.op_type = 6;
	buf.cant_operands = cant_op;

	record_init_time();

	/* Envio la request */
	if( msgsnd(req_q_id, &buf, TAMCOLA, 0) < 0 ){
		perror("Error al enviar");
	}
	
	/* Espero por la respuesta */
	if( msgrcv(res_q_id, &buf_rcv, TAMCOLARES, 0, 0) < 0 ){
		perror("Error al recibir");
	}else{
		float res;
		if(buf_rcv.is_neg){
			res = buf_rcv.float_res*(-1);
		}else{
			res = buf_rcv.float_res;
		}
		printf("Resultado de la división: %.2f\n", res);
	}
	
	record_end_time();
	
	printf("Tiempo de ejecución: %lu microsegundos.\n",accum);
	
}

void create_res_queue(){
	
	/**
	*	Creo una llave para pasar a msgget
	*/
	
	key_t key;
	
	if( ( key = ftok(PATHRES,IDRES) ) < 0 ){
		perror("Error al crear la key");
		exit(1);
	}
	
	/**
	*	Creo la cola de mensajes
	*/
	
	if( ( res_q_id = msgget(key, IPC_CREAT | 0777) ) < 0 ){
		perror("Error al crear la cola");
		exit(1);
	}

}

void create_req_queue(){
	
	/**
	*	Creo una llave para pasar a msgget
	*/
	
	key_t key;
	
	if( ( key = ftok(PATHREQ,IDREQ) ) < 0 ){
		perror("Error al crear la key");
		exit(1);
	}
	
	/**
	*	Creo la cola de mensajes
	*/
	
	if( ( req_q_id = msgget(key, IPC_CREAT | 0777) ) < 0 ){
		perror("Error al crear la cola");
		exit(1);
	}

}

void terminate_server(){

	my_msgbuf buf;
	buf.op_type = 7;
	
	if( msgsnd(req_q_id, &buf, TAMCOLA, 0) < 0 ){
		perror("Error al enviar terminate");
	}
}

void print_header(){
	printf("******************************************************************************\n");
	printf("****************************** MINI OPERACIONES ******************************\n");
	printf("******************************************************************************\n\n");
}

void print_options(){
	printf("\n¿Qué desea hacer?\n");
	printf("	1. Convertir un número de decimal a binario.\n");
	printf("	2. Convertir un número binario a hexadecimal.\n");
	printf("	3. Sumar números (hasta 4 operandos).\n");
	printf("	4. Restar números (hasta 4 operandos).\n");
	printf("	5. Multiplicar dos números.\n");
	printf("	6. Dividir dos números.\n");
	printf("	7. Salir.\n\n");
	printf("Ingrese una opcion: ");
}
