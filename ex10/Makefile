CC=gcc
CFLAGS= -Wall
LIBS= -lpthread

all server.c client.c:
	$(CC) $(CFLAGS) server.c -o server $(LIBS)
	$(CC) $(CFLAGS) client.c -o client $(LIBS)
	./server 60000 abcd

clean:
	rm -f server client *.o