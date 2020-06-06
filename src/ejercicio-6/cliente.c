#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define PORT 14550 /* El puerto donde se conectara */
#define MAXDATASIZE 2048
#define TOPEPRUEBAS 10

int main(int argc, char *argv[])
{
  
    
    int sockfd, numbytes;
    int request;
    char buf[MAXDATASIZE]; /* Buffer donde se reciben los datos */
    struct hostent *he; /* Se utiliza para convertir el nombre del host
    a su dirección IP */
    struct sockaddr_in their_addr; /* dirección del server donde se
    conectara */
    if (argc != 3){
        fprintf(stderr,"Ingrese ./cliente ip p\n Donde p puede ser: \n 1: Brindar turnos para sacar licencia de matrimonio \n 2: Informacion sobre la partida de nacimiento\n 3: Turno para inscripcion de un bebe recien nacido\n 4: Turno para patentar el auto\n 5: Turno para la transferencia de un vehiculo\n 6: Informacion sobre el dominio de un vehiculo\n");
        exit(EXIT_FAILURE);
    }
    if ((he=gethostbyname(argv[1])) == NULL) { 
        herror("gethostbyname");
        exit(EXIT_FAILURE);
    }
    
    
    /* Creamos el socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    } 

    /* Establecemos their_addr con la dirección del server */
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(their_addr.sin_zero), 8);
    
    /* Intentamos conectarnos con el servidor */
    if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct
    sockaddr)) == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    
    /*--Enviamos numero de solicitud----*/
    request = atoi(argv[2]);
    if ((send(sockfd,&request,sizeof(request),0))==-1){
        perror("send: ");
        exit(EXIT_FAILURE);
    }
    /* Recibimos los datos del servidor */
    if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    /* Visualizamos lo recibido */
    buf[numbytes] = '\0';
    printf(buf);
    /* Devolvemos recursos al sistema */
    close(sockfd);
    return 0;
} 
