cli : client.c libmysock.a ser
	gcc -o client client.c -L. -lmysock -lpthread -lm

ser : server.c libmysock.a 
	gcc -o server server.c -L. -lmysock -lpthread -lm

libmysock.a : mysock.o 
	ar rcs libmysock.a mysock.o 

mysock.o : mysock.c
	gcc -c mysock.c  -lpthread -lm 

clean :
	rm -f mysock.o client server libmysock.a 
 