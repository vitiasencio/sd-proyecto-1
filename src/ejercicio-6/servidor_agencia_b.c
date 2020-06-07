#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <netdb.h>

#define MYPORT 14551 /*Nro de puerto donde se conectaran los clientes*/
#define PORT_A_C 14550 /*Nro de puerto donde se conectaran como cliente al agente A*/
#define BACKLOG 10 /* Tamaño de la cola de conexiones recibidas */

void brindar_turno_patentar_auto(int newfd){
    time_t tiempo;
    time(&tiempo); //retorna el tiempo en segundos desde 1/1/1970
    tiempo = tiempo + 86400;
    struct tm *result;
    result = localtime(&tiempo);
    char mensaje1[300]  = "Usted tiene un turno para patentar su auto el dia : ";
    strcat(mensaje1,asctime(result));
    strcat(mensaje1,"\n");
    strcat(mensaje1,"Recuerde traer: \n - DNI\n - Constacia de CUIL, CUIT o CDI \n - Documentacion obtenida a traves de la agencia\n");

    if(send(newfd,mensaje1,sizeof(mensaje1),0)==-1){
        perror("send: ");
        exit(EXIT_FAILURE);
    }
}
void informacion_dominio_vehiculo(int newfd){
    char mensaje [300]  = "Informacion dominio del vehiculo \n - Datos de radicacion \n - Afecciones: denuncia de robo,embargo,prenda,etc \n - Inhibiciones del titular \n - Caracteristicas del rodado: marca, tipo, modelo, numero de motor y chasis, ano de la primera inscripcion, usos\n" ;
    if(send(newfd,mensaje,sizeof(mensaje),0)==-1){
        perror("send: ");
        exit(EXIT_FAILURE);
    }
}
void brindar_turno_transferencia_vehiculo(int newfd){
    time_t tiempo;
    time(&tiempo); //retorna el tiempo en segundos desde 1/1/1970
    tiempo = tiempo + 86400;
    struct tm *result;
    result = localtime(&tiempo);
    char mensaje1[350]  = "Usted tiene un turno para transferir su auto el dia : ";
    strcat(mensaje1,asctime(result));
    strcat(mensaje1,"\n");
    strcat(mensaje1,"Recuerde: \n - El comprador debera contar con constancia de CUIT/CUIL/CDI \n - El vendedor debera tener el titulo de la propiedad y cedula del vehiculo \n - Documento del comprador y vendedor \n");

    if(send(newfd,mensaje1,sizeof(mensaje1),0)==-1){
        perror("send: ");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    
    /*-----Variables como servidor-----*/
    int sockfd,numbytes; /* El servidor escuchara por sockfd */
    int newfd; /* las transferencias de datos se realizar mediante newfd */
    struct sockaddr_in my_addr; /* contendrá la dir IP y el nro de puerto local */
    struct sockaddr_in cliente_addr; //Contendrá la dir IP y nro de puerto del cliente
    socklen_t sin_size; /* Contendra el tamaño de la escructura sockaddr_in */
    int request;
    
    /*-----Variables como cliente-------------------------------*/
    int sockfd2;
    struct hostent *he; /* Se utiliza para convertir el nombre del host a su dirección IP */
    struct sockaddr_in  agencia_a_addr; /* dirección del server donde se conectara */
    char buf[500]; /* Buffer donde se reciben los datos */
    /*Guarda la ip de la otra agencia*/
    if (argc != 2){
        fprintf(stderr,"Ingrese ./servidor ip \n");
        exit(EXIT_FAILURE);
    }
    if ((he=gethostbyname(argv[1])) == NULL) { 
        herror("gethostbyname");
        exit(EXIT_FAILURE);
    }
    agencia_a_addr.sin_family = AF_INET;
    agencia_a_addr.sin_port = htons(PORT_A_C);
    agencia_a_addr.sin_addr =  *((struct in_addr *)he->h_addr);
    bzero(&(agencia_a_addr.sin_zero), 8);
    /*--------------------------------------------------------*/
    
    
  
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
            while(1){
                /* Recibimos la solicitud del cliente*/
                if((numbytes = recv(newfd,&request,sizeof(request),0))==-1){
                    perror("recv: ");
                    exit(EXIT_FAILURE);
                }
                switch(request){
                
                    case 4: brindar_turno_patentar_auto(newfd);
                            break;
                    case 5: brindar_turno_transferencia_vehiculo(newfd);
                            break;
                    case 6: informacion_dominio_vehiculo(newfd);
                            break;
                 /*Casos donde el servidor se comporta como un cliente y debe comunicarse con la agencia b*/
                    case 1:
                    case 2: 
                    case 3: //1,2 o 3.
                            
                            /*----se crea el socket-----*/
                            if ((sockfd2 = socket(AF_INET, SOCK_STREAM, 0)) == -1){
                                perror("socket:");
                                exit(EXIT_FAILURE);
                            }
                            
                            
                            /* Intentamos conectarnos con el servidor */
                            if (connect(sockfd2, (struct sockaddr *)&agencia_a_addr, sizeof(struct sockaddr)) == -1)
                            {
                                perror("connect");
                                exit(EXIT_FAILURE);
                            }
                            
                             /*--Enviamos numero de solicitud----*/
                            if ((send(sockfd2,&request,sizeof(request),0))==-1){
                                perror("send: ");
                                exit(EXIT_FAILURE);
                            }
                            
                            /* Recibimos los datos del servidor */
                            if ((numbytes=recv(sockfd2, buf, sizeof(buf), 0)) == -1)
                            {
                                perror("recv");
                                exit(EXIT_FAILURE);
                            }
                            printf("Esperando respuesta del agente B\n");
                            printf("Redirigiendo respuesta al cliente\n");
                            /*Redirigimos la respuesta al cliente*/
                            if(send(newfd,buf,sizeof(buf),0)==-1){
                                perror("send: ");
                                exit(EXIT_FAILURE);
                            }
                            close(sockfd2);
                            break;
                    default:
                            printf("error");
                            exit(1);
                }
            
            }
           
            close(newfd);
            exit(0);
        }
    }
    close(sockfd);
    return 0;
}
