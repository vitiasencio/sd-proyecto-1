all: cliente servidor servidor_agencia_b
.PHONY: all
.CLEAN: all

cliente: cliente.c
	gcc -o cliente cliente.c 

servidor : servidor.c
	gcc -o servidor servidor.c

servidor_agencia_b : servidor_agencia_b.c
	gcc -o servidor_agencia_b servidor_agencia_b.c

clean:
	rm cliente; rm servidor; rm servidor_agencia_b;
