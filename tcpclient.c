 
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>

#define bufsize 1024
#define EXIT_SUCCESS 0
#define EXIT_FAILURE -1

#define TRUE 1
#define FALSE 0


int main(int argc, char *argv[]) {
	int sock, portno, msg;
	struct hostent *host;
	struct sockaddr_in server;
	char buffer[bufsize];

	if (argc < 4) {
		printf("\n usage: \n\n $ %s hostname port string\n\n", argv[0]);
		return EXIT_FAILURE;
	}

	// look up hostname (server) using DNS

	if ((host = gethostbyname(argv[1])) == 0) {
	    fprintf(stderr, "%s: unknown host\n", argv[1]);
	    return EXIT_FAILURE;
	}

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0); 

	// check socket is less than zero
	if (sock < EXIT_SUCCESS)
	{
		fprintf(stderr, "ERROR: cannot create a socket\n");
		return EXIT_FAILURE;
	}

	// Set up fields for socket to point to host and port
	bcopy((char *) host->h_addr, (char *) &server.sin_addr.s_addr,host->h_length);
	server.sin_port = htons(atoi(argv[2]));
	server.sin_family = AF_INET;

	// connect (3-way handshake)
	if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < EXIT_SUCCESS)
	{
		fprintf(stderr, "ERROR: connecting\n");
		return EXIT_FAILURE;
	}	

	// Copy the arg into buf so we can send it to the server
	strncpy(buffer, argv[3], bufsize);

	// Send sentence to server
	msg = write(sock, buffer, bufsize); 

	// check to see if it failed to send a message to server
	if (msg < EXIT_SUCCESS)
	{
		fprintf(stderr, "ERROR: cannot send to server\n");
		return EXIT_FAILURE;
	}

	// read response from server
	msg = read(sock, buffer, bufsize-1); 

	// check to see if it failed to recieve a message from server
	if (msg < EXIT_SUCCESS)
	{
		fprintf(stderr, "ERROR: no message from the server\n");
		return EXIT_FAILURE;
	}

	// print result to window
	printf("\nmessage from server: %s\n\n", buffer);

	close(sock);
	return EXIT_SUCCESS;
}
