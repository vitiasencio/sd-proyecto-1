#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

void create_req_queue();

void create_res_queue();

void print_header();

void print_options();


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
	my_msgbuf buf;
	
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
				printf("suma\n");
				break;
			case 4:
				printf("resta\n");
				break;
			case 5:
				printf("multiplicar\n");
				break;
			case 6:
				printf("dividir\n");
				break;
			case 7:
				retry='n';
				break;
			default:
				break;
		}

		buf.op_type = op;
		if( msgsnd(req_q_id, &buf, TAMCOLA, 0) < 0 ){
			perror("Error al enviar");
			//exit(-1);
		}
		
		while(retry!='y' && retry!='n'){
			printf("Desea realizar otra operación? (y/n): ");
			scanf("%c",&retry);
		}

		if(retry=='y'){
			retry='x';
		}
		
	}
	
	
	/**
	*	Escribo en la cola
	*/
	
	
	
	
	/**
	*	Leo de la cola
	*/
	
//	my_msgbuf buf_rcv;
	
//	if( msgrcv(req_q_id, &buf_rcv, TAMCOLA, 1, 0) < 0 ){
//		perror("Error al recibir");
//	}else{
//		printf("Mensaje recibido, type: %lu\n", buf_rcv.type);
//	}
	
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
