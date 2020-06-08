.PHONY : all
.PHONY : clean

all : client server

client : protocol.h client.c
	gcc -o client client.c

server : protocol.h server.c
	gcc -o server server.c

clean :
	rm client; rm server;

