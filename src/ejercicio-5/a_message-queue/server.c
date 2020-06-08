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
void handle_substraction(my_msgbuf * buf);
void handle_product(my_msgbuf * buf);
void handle_division(my_msgbuf * buf);
void handle_dectobin(my_msgbuf * buf);
void handle_bintohexa(my_msgbuf * buf);

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
		}
	
		op = buf_rcv.op_type;
		
		switch(op){
			case 1:
				handle_dectobin(&buf_rcv);
				break;
			case 2:
				handle_bintohexa(&buf_rcv);
				break;
			case 3:
				handle_addition(&buf_rcv);
				break;
			case 4:
				handle_substraction(&buf_rcv);
				break;
			case 5:
				handle_product(&buf_rcv);
				break;
			case 6:
				handle_division(&buf_rcv);
				break;
			default:
				break;
		
		}
		
	}
	
	
	exit(0);
}

void handle_dectobin(my_msgbuf * buf){
	
	long decbin = buf->decbin;
	int mod=0,i=0;
	char aux;
	my_msgbuf_res buf_res;
	buf_res.op_type = buf->op_type;
	
	for(i=0;decbin>0;i++){
		mod = decbin%2;
		buf_res.conv_res[i] = mod + '0';
		decbin/=2;
	}
	
	buf_res.conv_res[i] = '\0';
	
	int medio = i/2; 
	int fin = i-1;
	
	for(i=0;i<medio;i++){
		aux = buf_res.conv_res[i];
		buf_res.conv_res[i] = buf_res.conv_res[fin-i];
		buf_res.conv_res[fin-i] = aux; 
	}
	
	if( msgsnd(res_q_id, &buf_res, TAMCOLARES, 0) < 0 ){
		perror("Error al enviar server");
	}
}

void handle_bintohexa(my_msgbuf * buf){
	
	int hexConstant[] = {0, 1, 10, 11, 100, 101, 110, 111, 1000, 
                         1001, 1010, 1011, 1100, 1101, 1110, 1111};

    int index, i, digit;
	
	long decbin = buf->decbin, temp;

	char aux;
	my_msgbuf_res buf_res;
	buf_res.op_type = buf->op_type;
	
	temp = decbin;
    index = 0;
	
	while(temp!=0)
    {
        /* Obtengo los últimos 4 digitos binarios */
        digit = temp % 10000;

        /* Encuentro el dígito Hexa relacionado a los últimos 4 dígitos binarios*/
        for(i=0; i<16; i++)
        {
            if(hexConstant[i] == digit)
            {
                if(i<10)
                {
                    /* Digitos entre 0 y 9  */
                    buf_res.conv_res[index] = (char)(i + 48);
                }
                else
                {
                    /* Digitos de la A a la F */
                    buf_res.conv_res[index] = (char)((i-10) + 65);
                }

                index++;
                break;
            }
        }

        /* Remove the last 4 digits as it is processed */
        temp /= 10000;
    }
	
	buf_res.conv_res[index] = '\0';
	
	int medio = index/2; 
	int fin = index-1;
	
	for(i=0;i<medio;i++){
		aux = buf_res.conv_res[i];
		buf_res.conv_res[i] = buf_res.conv_res[fin-i];
		buf_res.conv_res[fin-i] = aux; 
	}
	
	
	if( msgsnd(res_q_id, &buf_res, TAMCOLARES, 0) < 0 ){
		perror("Error al enviar server");
	}
}

void handle_addition(my_msgbuf * buf){
	
	int i;
	float res = 0;
	
	for(i=0; i<buf->cant_operands; i++){
		printf("operando %i: %f\n",i,buf->operands[i]);
		res+=buf->operands[i];
	}
	
	my_msgbuf_res buf_res;
	buf_res.op_type = buf->op_type;
	
	if(res<0){
		buf_res.is_neg = 1;
		buf_res.float_res = res*(-1);	
	}else{
		buf_res.is_neg = 0;
		buf_res.float_res = res;
	}
	
	if( msgsnd(res_q_id, &buf_res, TAMCOLARES, 0) < 0 ){
		perror("Error al enviar server");
	}
}

void handle_substraction(my_msgbuf * buf){
	
	int i;
	float res = buf->operands[0];
	
	for(i=1; i<buf->cant_operands; i++){
		res-=buf->operands[i];
	}
	
	my_msgbuf_res buf_res;
	buf_res.op_type = buf->op_type;
	
	if(res<0){
		buf_res.is_neg = 1;
		buf_res.float_res = res*(-1);	
	}else{
		buf_res.is_neg = 0;
		buf_res.float_res = res;
	}
	
	
	if( msgsnd(res_q_id, &buf_res, TAMCOLARES, 0) < 0 ){
		perror("Error al enviar server");
	}
}

void handle_product(my_msgbuf * buf){
	
	int i;
	float res = buf->operands[0];
	
	for(i=1; i<buf->cant_operands; i++){
		res*=buf->operands[i];
	}
	
	my_msgbuf_res buf_res;
	buf_res.op_type = buf->op_type;
	
	if(res<0){
		buf_res.is_neg = 1;
		buf_res.float_res = res*(-1);	
	}else{
		buf_res.is_neg = 0;
		buf_res.float_res = res;
	}
	
	printf("resultado: %i\n",buf_res.float_res);
	
	if( msgsnd(res_q_id, &buf_res, TAMCOLARES, 0) < 0 ){
		perror("Error al enviar server");
	}
}

void handle_division(my_msgbuf * buf){
	
	int i;
	float res = buf->operands[0];
	
	for(i=1; i<buf->cant_operands; i++){
		res/=buf->operands[i];
	}
	
	my_msgbuf_res buf_res;
	buf_res.op_type = buf->op_type;
	
	if(res<0){
		buf_res.is_neg = 1;
		buf_res.float_res = res*(-1);	
	}else{
		buf_res.is_neg = 0;
		buf_res.float_res = res;
	}
	
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
