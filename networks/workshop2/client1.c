

/* ------------------ client1.c --------------------- */

/* This is the first client and it sends a character to the server.
The server must be running first, and this client must be running next. */

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

	/* initialize the character value to A */
	structure.char_value = 'A';
	/* write the character to the socket */
	write(sd, &structure, sizeof(structure));
	close(sd);
	exit(0);
}
