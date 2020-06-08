.PHONY : all
.PHONY : raspberry
.PHONY : clean

all : client server
raspberry : client_r server_r

array_clnt.c array_svc.c array_xdr.c array.h: array.x
	rpcgen -C array.x

client_r : client.c array_clnt.c
	gcc -lnsl -o client_r client.c array_clnt.c array_xdr.c

server_r : server.c array_svc.c 
	gcc -lnsl -o server_r server.c array_svc.c array_xdr.c

client : client.c array_clnt.c array_xdr.c
	gcc -ltirpc -I/usr/include/tirpc -o client client.c array_clnt.c array_xdr.c

server : server.c array_svc.c array_xdr.c
	gcc -ltirpc -I/usr/include/tirpc -o server server.c array_svc.c array_xdr.c

clean :
	rm client; rm server; rm array_clnt.c; rm array_svc.c; rm array_xdr.c; rm array.h;

clean_r :
	rm client_r; rm server_r; rm array_clnt.c; rm array_svc.c;  rm array_xdr.c; rm array.h;

