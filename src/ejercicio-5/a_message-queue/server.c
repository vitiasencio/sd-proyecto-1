#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>

#define TAMCOLA sizeof(struct mensaje)
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

typedef struct mensaje my_msgbuf;
int req_q_id;
int res_q_id;


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

int main(int argc, char *argv[]) {
	
	create_req_queue();
	//create_res_queue();
	
	int op = -1;
	
	/**
	*	Leo de la cola
	*/
	
	my_msgbuf buf_rcv;
	
	while(op!=7){
		if( msgrcv(req_q_id, &buf_rcv, TAMCOLA, 0, 0) < 0 ){
			perror("Error al recibir");
		}else{
			printf("Mensaje recibido, type: %i\n", buf_rcv.op_type);
			op = buf_rcv.op_type;
		}
		
	}
	
	/**
	*	Elimino la cola de mensajes
	
	if( msgctl(req_q_id, IPC_RMID, NULL) < 0 ){
		perror("No se pudo eliminar la cola");
		exit(-1);
	}
	*/
	
	exit(0);
}
