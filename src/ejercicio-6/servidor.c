#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>

#define MYPORT 14550 /*Nro de puerto donde se conectaran los clientes*/
#define PAR_PORT 14551 /*Nro de puerto donde se conectaran como cliente el agente B*/
#define BACKLOG 10 /* Tamaño de la cola de conexiones recibidas */
#define BYTESAENVIAR 2048

void brindar_turno_licencia_matrimonio(int newfd){
    time_t tiempo;
    time(&tiempo); //retorna el tiempo en segundos desde 1/1/1970
    tiempo = tiempo + 86400;
    struct tm *result;
    result = localtime(&tiempo);
    char mensaje1[100]  = "Usted tiene un turno para licencia de matrimonio el dia : ";
    strcat(mensaje1,asctime(result));
    strcat(mensaje1,"\n");
    printf("%s",mensaje1);

    if(send(newfd,mensaje1,sizeof(mensaje1),0)==-1){
        perror("send: ");
        exit(EXIT_FAILURE);
    }
}
void informacion_partida_nacimiento(int newfd){
    char mensaje [209]  = "El acta de nacimiento informa \n - La fecha y hora de nacimiento \n - El nombre del bebe recien nacido \n - El nombre de los padres \n - Localidad de nacimiento \n - Domicilio del hospital en que se produjo \n" ;
    if(send(newfd,mensaje,sizeof(mensaje),0)==-1){
        perror("send: ");
        exit(EXIT_FAILURE);
    }
}
void turno_inscripcion_bebe_nacido(int newfd){
    time_t tiempo;
    time(&tiempo); //retorna el tiempo en segundos desde 1/1/1970
    tiempo = tiempo + 86400;
    struct tm *result;
    result = localtime(&tiempo);
    char mensaje1[100]  = "Usted tiene un turno para inscribir a su bebe recien nacido el dia : ";
    strcat(mensaje1,asctime(result));
    strcat(mensaje1,"\n");
    printf("%s",mensaje1);

    if(send(newfd,mensaje1,sizeof(mensaje1),0)==-1){
        perror("send: ");
        exit(EXIT_FAILURE);
    }
}

int main() {
    
    /*-----Variables como servidor-----*/
    int sockfd,numbytes; /* El servidor escuchara por sockfd */
    int newfd; /* las transferencias de datos se realizar mediante newfd */
    struct sockaddr_in my_addr; /* contendrá la dir IP y el nro de puerto local */
    struct sockaddr_in cliente_addr; //Contendrá la dir IP y nro de puerto del cliente
    socklen_t sin_size; /* Contendra el tamaño de la escructura sockaddr_in */
    int request;
    
    /*-----Variables como cliente-----*/
    //int sockfd2,numbytes;
    //int newfd2;
    //char buf[MAXDATASIZE]; /* Buffer donde se reciben los datos */
    //struct hostent *he_agencia_b; /* Se utiliza para convertir el nombre del host a su dirección IP */
    //struct sockaddr_in agengia_b_addr; /* dirección del server donde se conectara */
    
    
  
    /*----se crea el socket-----*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket:");
        exit(EXIT_FAILURE);
    }

    // Asigna valores a la estruct my_addr para luego poder llamar a la función bind()
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT); /*formato de network byte order htons es para convertirlo para que se pueda transmitir por la red */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* automaticamente usa IP local */
    bzero(&(my_addr.sin_zero), 8); /* rellena con ceros el resto de la estructura */

    /*-----BIND-----Asigna un nombre al socket */
    if ( bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
        perror("bind: ");
        exit(EXIT_FAILURE);
    }

    /*-----LISTEN-----Habilita el socket para recibir conexiones, con una cola de x conexiones en espera como máximo */
    if (listen(sockfd, BACKLOG) == -1){
        perror("listen: ");
        exit(EXIT_FAILURE);
    }
    

    /*-----ACCEPT-----*/
    while(1) /* loop que llama a accept() */
    {
        sin_size = sizeof(struct sockaddr_in);
        /*Se espera por conexiones ,*/
        if ((newfd = accept(sockfd, (struct sockaddr *)&cliente_addr, &sin_size)) == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE); /* Si se produce un error se finaliza el programa */
        }
        printf("server: conexion desde: %s\n", inet_ntoa(cliente_addr.sin_addr));
        printf("Desde puerto: %d \n", ntohs(cliente_addr.sin_port));
        
        if (!fork())
        { /* Comienza el proceso hijo*/ 
            
            /* Recibimos la solicitud del cliente*/
            if((numbytes = recv(newfd,&request,sizeof(request),0))==-1){
                perror("recv: ");
                exit(EXIT_FAILURE);
            }
            switch(request){
            
                case 1: brindar_turno_licencia_matrimonio(newfd);
                        break;
                case 2: informacion_partida_nacimiento(newfd);
                        break;
                case 3: turno_inscripcion_bebe_nacido(newfd);
                        break;
            }
            
            
           // char message[] = "hola";
            /*enviamos los datos mediante newfd */
            //if (send(newfd,message,sizeof(message), 0) == -1)
             //   perror("send");
            close(newfd);
            exit(0);
        }
    }
    close(sockfd);
    return 0;
}
