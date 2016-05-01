#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h> 
#include <string.h> 
#include <sys/wait.h> 
  
#define bufsize 1024

void eatZombies(int n){
  // This function removes the zombie process state left
  // after the child exits. Learn about this in NWEN 301!

  wait3(NULL,WNOHANG,NULL); // Nom Nom 
}

// passing major arguments as fuctions for latter use in adressing and port arguments 
int main(int argc, char *argv[]) {
  int parrentfd, length, msgsock, status;
  struct sockaddr_in server;
  char buffer[bufsize];
  int n ;
  
  // for forking, and cleaning up zombie child state afterwards
  // You must not change this code. 
  pid_t id;       
  signal(SIGCHLD, &eatZombies);
 
  // socket
  parrentfd = socket(AF_INET, SOCK_STREAM, 0);
  if (parrentfd < 0){
   fprintf(stderr, "ERROR: opening socket\n");
   exit(1);
  }

  // check if there is a port number
  if (argc > 2 || atoi(argv[1]) == 0)
  {
    fprintf(stderr, "\n usage: \n\n $ %s port_number\n\n", argv[0]);
    exit(1);  
  }

  // socket addressing structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(atoi(argv[1])); // this time 1st arg is port#

  // BINDING the socket  
  if (bind(parrentfd, (struct sockaddr*)&server, sizeof(server))) {
    fprintf(stderr, "ERROR: binding socket error\n");
    exit(1);
  }
	

  // listening
  listen(parrentfd,5);
  
  while (1) {
    
    //accepting connection request
    length = sizeof(server);
	  msgsock = accept(parrentfd, (struct sockaddr *) &server, &length);
    if (msgsock < 0){ 
      fprintf(stderr, "ERROR: on accept\n");
      exit(1);
    }
    
    // the next call makes a new child process that will actually handle the client.
    // an operation that creates  copy of the previous fuctions as a child  
    id = fork();
    
  
    // when if > 0 loop back to the server function
    if (id > 0){
      continue;
    }

    // when id < 0, we had an error.
    else if (id < 0){
      fprintf(stderr, "ERROR: forking faliure\n");
      close(msgsock);
      exit(1);

    }
    
    
    // when id == 0 the program will carry on to intract with the client   
    if (id == 0)
        {
          // reading from message from socket
          if (read(msgsock, buffer, bufsize-1)<0){
            fprintf(stderr, "ERROR: reading from socke\n");
            exit(1);
          }
          printf("Here is the message: %s\n",buffer);
    
          // alteration to the message from the client
          // iterate through every thing in the string an change to a upper case letter if not already and change every e or E to a A
          int i;
          for(i=0;i<strlen(buffer);i++){
            buffer[i] = buffer[i] == 'e' || buffer[i] == 'E' ? 'A' : toupper(buffer[i]);  
          }
          
          // writing back to the socket
          if (write(msgsock,buffer,bufsize) < 0){
            fprintf(stderr, "ERROR: writing to socket\n");
            
            //loop continues 
            close(msgsock);
            exit(1);
          }
          close(msgsock);
    }    


    // Exit
    exit(0);
  }
  
 
}

