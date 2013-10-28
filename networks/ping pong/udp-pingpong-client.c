/* PingPong client using UDP */

/* Question 4 of 2012-13 Problem Set Assignment One */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>


#define SERVER_UDP_PORT "5000"
#define MAXLEN 4096
#define ECHOLEN 64

// calculate the time diff between start and end
long delay(struct timeval t1, struct timeval t2)
{
	long d;
	d = (t2.tv_sec - t1.tv_sec) * 1000000;
	d += t2.tv_usec - t1.tv_usec;
	return(d);
}

int main(int argc, char **argv)
{
	int i, j, sd;
	char *pname;
	char rbuf[MAXLEN], sbuf[MAXLEN];
	struct timeval start, end;
	
	pname = argv[0];
	if (argc != 2) {
		fprintf(stderr, "Usage: %s hostname\n", pname);
		exit(1);
	}
	
	/* create the UDP datagram socket */
	if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		fprintf(stderr, "Can't create a socket\n");
		exit(1);
	}
	
	/* get the server address info */
	struct addrinfo input, *list, *head;
	memset(&input, 0, sizeof(input));
	input.ai_family = AF_INET;
	input.ai_socktype = SOCK_DGRAM;
	
	if ((getaddrinfo(argv[1], SERVER_UDP_PORT, &input, &list)) != 0) {
		fprintf(stderr, "Can't get server's address info\n");
		exit(1);
	}
	struct sockaddr_in *server;
	
	head = list;
	while (head) { //search through result list
		if (head->ai_family == AF_INET) { // IPv4
			server = (struct sockaddr_in *) head->ai_addr;
			break;
		} else { //next addrinfo node
			head = head->ai_next;
		}
	}
	
	/* fill up the echo message */	
	for (i = 0; i < ECHOLEN; i++) {
		j = (i < 26) ? i : i % 26;
		sbuf[i] = 'a' + j;
	}
	
	gettimeofday(&start, NULL); /* start delay measurement */
	
	socklen_t server_len = sizeof(struct sockaddr_in);
	if (sendto(sd, sbuf, ECHOLEN, 0, (struct sockaddr *)server, server_len) == -1) {
		fprintf(stderr, "sendto error\n");
		exit(1);
	}
	
	if (recvfrom(sd, rbuf, MAXLEN, 0, (struct sockaddr *)server, &server_len) < 0) {
		fprintf(stderr, "recvfrom error\n");
		exit(1);
	}
	
	gettimeofday(&end, NULL); /* end delay measurement */
	
	if (strncmp(sbuf, rbuf, ECHOLEN) != 0)
		printf("Data is corrupted\n");
	
	printf("Round-trip delay = %ld microsecond.\n", delay(start, end));
	
	close(sd);
	freeaddrinfo(list);
	return(0);
}
