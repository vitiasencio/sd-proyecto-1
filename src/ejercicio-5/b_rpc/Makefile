.PHONY : all
.PHONY : raspberry
.PHONY : clean

all : client server
raspberry : client_r server_r

miniop_clnt.c miniop_svc.c miniop_xdr.c miniop.h: miniop.x
	rpcgen -C miniop.x

client_r : client.c miniop_clnt.c
	gcc -lnsl -o client_r client.c miniop_clnt.c miniop_xdr.c

server_r : server.c miniop_svc.c 
	gcc -lnsl -o server_r server.c miniop_svc.c miniop_xdr.c

client : client.c miniop_clnt.c miniop_xdr.c
	gcc -ltirpc -I/usr/include/tirpc -o client client.c miniop_clnt.c miniop_xdr.c

server : server.c miniop_svc.c miniop_xdr.c
	gcc -ltirpc -I/usr/include/tirpc -o server server.c miniop_svc.c miniop_xdr.c

clean :
	rm client; rm server; rm miniop_clnt.c; rm miniop_svc.c; rm miniop_xdr.c; rm miniop.h;

clean_r :
	rm client_r; rm server_r; rm miniop_clnt.c; rm miniop_svc.c;  rm miniop_xdr.c; rm miniop.h;

