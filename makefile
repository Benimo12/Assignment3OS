all:server.o client.o 
	gcc -o server server.o
	gcc -o client client.o
	rm *.o
server.o:server.c hw2.h
	gcc -c server.c
client.o:client.c hw2.h
	gcc -c client.c
