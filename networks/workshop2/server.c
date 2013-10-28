

/* ------------------ server.c --------------------- */

/* This is the server that reads a character from clientl.c from the socket.
The server will write the character to the socket and send it to client2.c */

#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <sys/un.h>
#include  <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define PORT 49643



main()
{
	int readable;
	int waitreado;
	int sd, nsl, ns2, lin, is_ns_ready;
	int yes=1; 
	typedef struct _msg
	{
		char char_value;
	} structure_type;

	structure_type structure;
	struct sockaddr_in name, namein; /* create socket for requests */

	sd = socket(AF_INET, SOCK_STREAM, 0);

	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	name.sin_family = AF_INET;
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	name.sin_port = htons(PORT);
	memset(&(name.sin_zero), '\0', 8);
	/* bind it to the created socket, and set max /* queue length */
	if (bind(sd, (struct sockaddr *)&name, sizeof(name)) != 0){
		perror("server-bind");
		exit(0);
	 }

	listen(sd,5);

	/* accept a connection */
	lin = sizeof(namein);
	if ((nsl = accept(sd, (struct sockaddr *)&namein, &lin)) == -1)
		perror("ns-error");
	if ((ns2 = accept(sd, (struct sockaddr *)&namein, &lin)) == -1)
		perror("ns-error");

	/* close sd - use only ns*/
	close(sd);

	/* wait for data from client1 */
	recv(nsl, &structure, sizeof(structure), 0);
	printf ("\nThe value stored in structure is below\n");
	printf(" %c \n", structure.char_value);
	fflush(stdout);
	/* increment the value stored in structure */
	structure.char_value++;
	send(ns2, &structure, sizeof(structure),0);
	/* main server closes ns */
	close(nsl);
	close(ns2);
	exit(0);
}
