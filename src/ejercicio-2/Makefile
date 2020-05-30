.PHONY : all
.PHONY : raspberry
.PHONY : clean

all : client server tarea
raspberry : cliente_r servidor_r tarea

time_clnt.c time_svc.c: time.x
	rpcgen -C time.x

client_r : client.c time_clnt.c
	gcc -lnsl -o client_r client.c time_clnt.c

server_r : server.c time_svc.c 
	gcc -lnsl -o server_r server.c time_svc.c

client : client.c time_clnt.c 
	gcc -ltirpc -I/usr/include/tirpc -o client client.c time_clnt.c 

server : server.c time_svc.c 
	gcc -ltirpc -I/usr/include/tirpc -o server server.c time_svc.c 

tarea: tarea.c
	gcc -o tarea tarea.c

clean :
	rm client; rm server; rm time_clnt.c; rm time_svc.c; rm time.h; rm output.txt; rm tarea;

clean_r :
	rm cliente_r; rm servidor_r; rm time_clnt.c; rm time_svc.c; rm time.h; rm output.txt; rm tarea;

