#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include "protocol.h"

int req_q_id;
int res_q_id;

void create_req_queue();

void create_res_queue();

void handle_addition(my_msgbuf * buf);

void handle_substraction(my_msgbuf * buf){
	
	int i;
	int res = buf->operands[0];
	
	for(i=1; i<buf->cant_operands; i++){
		res-=buf->operands[i];
	}
	
	my_msgbuf_res buf_res;
	buf_res.op_type = buf->op_type;
	buf_res.number_res = res;
	
	
	if( msgsnd(res_q_id, &buf_res, TAMCOLARES, 0) < 0 ){
		perror("Error al enviar server");
	}
}

int main(int argc, char *argv[]) {
	
	create_req_queue();
	create_res_queue();
	
	int op = -1;
	
	/**
	*	Leo de la cola
	*/
	
	my_msgbuf buf_rcv;
	
	while(op!=7){
		if( msgrcv(req_q_id, &buf_rcv, TAMCOLA, 0, 0) < 0 ){
			perror("Error al recibir");
			exit(1);
		}/*else{
			printf("Mensaje recibido, type: %i\n", buf_rcv.op_type);
		}*/
	
		op = buf_rcv.op_type;
		
		switch(op){
			case 3:
				handle_addition(&buf_rcv);
				break;
			case 4:
				handle_substraction(&buf_rcv);
				break;
			default:
				break;
		
		}
		
	}
	
	
	exit(0);
}

void handle_addition(my_msgbuf * buf){
	
	int i;
	int res = 0;
	
	for(i=0; i<buf->cant_operands; i++){
		res+=buf->operands[i];
	}
	
	my_msgbuf_res buf_res;
	buf_res.op_type = buf->op_type;
	buf_res.number_res = res;
	
	
	if( msgsnd(res_q_id, &buf_res, TAMCOLARES, 0) < 0 ){
		perror("Error al enviar server");
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
