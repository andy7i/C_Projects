/* PingPong server using UDP */

/* Question 4 of 2012-13 Problem Set Assignment One */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define SERVER_UDP_PORT	"5000"	
#define MAXLEN 4096

int main()
{
	int sd;
	char buf[MAXLEN];	
		
	/* Create a datagram socket */
	struct addrinfo hints, *res;
	sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	/* Get the server sockaddr info */
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo("localhost", "http", &hints, &res);

	/* Bind the server address to the socket */
	connect(sd, res->ai_addr, res->ai_addrlen);


	/* Loop endless for clients' packets */
	while (1) {
		struct 	sockaddr_in client;
		int client_len = sizeof(client);
		int n;
		if ((n = recvfrom(sd, buf, MAXLEN, 0, (struct sockaddr *)&client, &client_len)) < 0) {
			fprintf(stderr, "Can't receive datagram\n");
			exit(1);
		}
		printf("Got a UDP datagram\n");
		if (sendto(sd, buf, n, 0, (struct sockaddr *)&client, client_len) != n) {
			fprintf(stderr, "Can't send datagram\n");
			exit(1);
		}
	}
	close(sd);

	return(0);
}

