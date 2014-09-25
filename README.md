C-TCP-client-server
===================

A TCP client and server written in C

To Compile the Server

  $ gcc tcpserver.c -o server

Run the server with a port number

  $ ./server 8080

Compile the client

  $ gcc tcpclient.c -o client
  
Run the client and connect a server hostname with on the operating port-number and your message

  $ ./client localhost 8080 hi
  
you should recieve a message back saying 

  message from server: HI
