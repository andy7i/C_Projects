

/* ------------------ client2.c --------------------- */

/* This is the second client and it reads a character from the server.
The server must be running first, then client1 be the second, and
client2 be the last. */

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
	int sd;
	typedef struct _msg
	{
		char char_value;
	} structure_type;
	structure_type structure;
	struct sockaddr_in name;

		/* create a socket for connecting to the server */
	sd = socket(AF_INET, SOCK_STREAM, 0);

	/* establish a connection to the localhost server */
	name.sin_family = AF_INET;
    name.sin_addr.s_addr = inet_addr("127.0.0.1");
    name.sin_port = htons(PORT);
    memset(&(name.sin_zero), '\0', 8);

	if (connect(sd, (struct sockaddr *)&name, sizeof(name)) == -1)
		perror("client-connect");

	/* Get the data from the Server */
	read(sd, &structure, sizeof(structure));
	printf ("\nThe value stored in structure is below\n");
	printf(" %c \n", structure.char_value);

	close(sd);
	exit(0);
}
