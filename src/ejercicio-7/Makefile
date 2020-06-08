.PHONY: all
.PHONY: clean

all: cliente servidor 

cliente: ejercicio7_clnt.o cliente.o ejercicio7_xdr.o ejercicio7_clnt.o
	gcc -Insl -o cliente cliente.o ejercicio7_clnt.o ejercicio7_xdr.o

servidor: ejercicio7_svc.o servidor.o ejercicio7_xdr.o
	gcc -Insl -o servidor ejercicio7_svc.o servidor.o ejercicio7_xdr.o 

ejercicio7_svc.o: ejercicio7_svc.c
	gcc -c ejercicio7_svc.c

ejercicio7_clnt.o: ejercicio7_clnt.c
	gcc -c ejercicio7_clnt.c

cliente.o: cliente.c
	gcc -c cliente.c

servidor.o: servidor.c
	gcc -c servidor.c

ejercicio7_xdr.o: ejercicio7_xdr.c
	gcc -c ejercicio7_xdr.c

ejercicio7_clnt.c ejercicio7.h ejercicio7_svc.c ejercicio7_xdr.c: ejercicio7.x
	rpcgen ejercicio7.x

clean: 
	rm cliente; rm servidor; rm ejercicio7_svc.c; rm ejercicio7_clnt.c; rm ejercicio7_xdr.c; rm cliente.o; rm servidor.o; rm ejercicio7.h; rm ejercicio7_svc.o; rm ejercicio7_clnt.o; rm ejercicio7_xdr.o; 

