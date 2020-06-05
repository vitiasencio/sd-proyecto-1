all: cliente servidor
.PHONY: all
.CLEAN: all

cliente: cliente.c
	gcc -o cliente cliente.c 

servidor : servidor.c
	gcc -o servidor servidor.c

clean:
	rm cliente; rm servidor