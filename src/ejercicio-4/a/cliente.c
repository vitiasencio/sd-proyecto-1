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
#define MAXDATASIZE 4
#define TOPEPRUEBAS 10

int main(int argc, char *argv[])
{
    struct timeval ti,tf;
    double tiempo;
    
    int sockfd, numbytes;
    char buf[MAXDATASIZE]; /* Buffer donde se reciben los datos */
    struct hostent *he; /* Se utiliza para convertir el nombre del host
    a su dirección IP */
    struct sockaddr_in their_addr; /* dirección del server donde se
    conectara */
    if (argc != 2){
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }
    if ((he=gethostbyname(argv[1])) == NULL) { 
        herror("gethostbyname");
        exit(1);
    }
    
    double suma=0;
    double promedio=0;
    for(int i=0; i<TOPEPRUEBAS; i++){

        /* Creamos el socket */
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("socket");
            exit(1);
        } 

        /* Establecemos their_addr con la dirección del server */
        their_addr.sin_family = AF_INET;
        their_addr.sin_port = htons(PORT);
        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        bzero(&(their_addr.sin_zero), 8);
        
        gettimeofday(&ti,NULL);
        /* Intentamos conectarnos con el servidor */
        if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct
        sockaddr)) == -1)
        {
            perror("connect");
            exit(1);
        }
        
        
        /* Recibimos los datos del servidor */
        if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
        {
            perror("recv");
            exit(1);
        }
        gettimeofday(&tf,NULL);
        tiempo = ((tf.tv_sec*1000000)+tf.tv_usec) - ((ti.tv_sec*1000000) + ti.tv_usec);
        printf("tiempo: %g us en recibir %d bytes.\n",tiempo,sizeof(buf));

        suma = suma + tiempo;
    }
    promedio = suma/TOPEPRUEBAS;
    /* Visualizamos lo recibido */
    buf[numbytes] = '\0';
    printf("Demora en promedio %g us en recibir %d bytes\n",promedio,sizeof(buf));
    
    /* Devolvemos recursos al sistema */
    close(sockfd);
    return 0;
} 
