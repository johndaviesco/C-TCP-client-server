# run make to compile, run make test to quick run the server & client

all: tcpclient.c tcpserver.c
	gcc -o client tcpclient.c
	gcc -o server tcpserver.c


test: server client
	./server 8080 &
	./client localhost 8080 hi
