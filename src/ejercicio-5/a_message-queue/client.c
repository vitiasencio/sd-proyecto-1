#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include "protocol.h"

/*#define TAMCOLA sizeof(struct mensaje)
#define TAMCOLARES sizeof(struct mensaje_res)

#define PATHREQ "/tmp"
#define IDREQ 'S'

#define PATHRES "/usr"
#define IDRES 'T'

struct mensaje {
	int op_type;
	int cant_operands;
	int op1;
	int op2;
	int op3;
	int op4;
};

struct mensaje_res{
	int op_type;
	int float_res;
	char hexa_res[20];
}


typedef struct mensaje my_msgbuf;
typedef struct mensaje_res my_msgbuf_res;
*/
int req_q_id;
int res_q_id;

void create_req_queue();
void create_res_queue();
void print_header();
void print_options();
void terminate_server();
void handle_addition();
void handle_substraction();
void handle_product();
void handle_division();

int main() {
	
	create_req_queue();
	create_res_queue();
	
	pid_t pid;
	
	if( (pid = fork() ) == 0 ){
		
		/* Estoy en el hijo */
		if( execl("./server",NULL) < 0 ){
			perror("Error en execl");
			exit(1);
		}
		
	}
	
	
	print_header();
	
	int op=-1;
	
	char retry = 'x';
	
	while(retry!='n'){
		fflush(stdin);
		print_options();
		scanf("%i",&op);
		
		switch(op){
			case 1:
				printf("decimal binario\n");
				break;
			case 2:
				printf("binario hexadecimal\n");
				break;
			case 3:
				//printf("suma\n");
				handle_addition();
				break;
			case 4:
				//printf("resta\n");
				handle_substraction();
				break;
			case 5:
				//printf("multiplicar\n");
				handle_product();
				break;
			case 6:
				//printf("dividir\n");
				handle_division();
				break;
			case 7:
				retry='n';
				break;
			default:
				break;
		}
		
		while(retry!='y' && retry!='n'){
			printf("Desea realizar otra operación? (y/n): ");
			scanf("%c",&retry);
		}

		if(retry=='n'){
			terminate_server();
		}else if( retry=='y' ){
			retry='x';
		}
		
	}
	
	
	/**
	*	Elimino la cola de mensajes
	*/
	
	if( msgctl(req_q_id, IPC_RMID, NULL) < 0 ){
		perror("No se pudo eliminar la cola");
		exit(-1);
	}
	if( msgctl(res_q_id, IPC_RMID, NULL) < 0 ){
		perror("No se pudo eliminar la cola");
		exit(-1);
	}
	
	exit(0);
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
	printf("¿Qué desea hacer?\n");
	printf("	1. Convertir un número de decimal a binario.\n");
	printf("	2. Convertir un número binario a hexadecimal.\n");
	printf("	3. Sumar números (hasta 4 operandos).\n");
	printf("	4. Restar números (hasta 4 operandos).\n");
	printf("	5. Multiplicar dos números.\n");
	printf("	6. Dividir dos números.\n");
	printf("	7. Salir.\n\n");
	printf("Ingrese una opcion: ");
}
