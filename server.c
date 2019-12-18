/* Huseyin Berk Gok - 230201039 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

const char WELCOMING[] = "Hi there, its the SERVER. Wanna do some math ;? Gimme some integer and wait for the show (@-@)*\n(Usage: <first integer> <second integer> ... <nth integer>)\n";

int main(int argc, char *argv[]) {

	int simpleSocket = 0;
	int simplePort = 0;
	int returnStatus = 0;
	struct sockaddr_in simpleServer;

	if (2 != argc) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	
	simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (simpleSocket == -1) {
		fprintf(stderr, "Could not create a socket!\n");
		exit(1);
	} else {
		fprintf(stderr, "Socket created!\n");
	}

	/* retrieve the port number for listening */
	simplePort = atoi(argv[1]);

	/* retrieve the port number for listening */
	/* use INADDR_ANY to bind to all local addresses */
	bzero(&simpleServer, sizeof(simpleServer));
	simpleServer.sin_family = AF_INET;
	simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
	simpleServer.sin_port = htons(simplePort);

	/* bind to the address and port with our socket */
	returnStatus = bind(simpleSocket, (struct sockaddr *)&simpleServer, sizeof(simpleServer));

	if (returnStatus == 0) {
		fprintf(stderr, "Bind completed!\n");
	} else {
		fprintf(stderr, "Could not bind to address!\n");
		close(simpleSocket);
		exit(1);
	}

	/* let's listen on the socket for connections */
	returnStatus = listen(simpleSocket, 5);

	if (returnStatus == -1) {
		fprintf(stderr, "Cannot listen on socket!\n");
		close(simpleSocket);
		exit(1);
	}

	while(1) {
		struct sockaddr_in clientName = {0};
		int simpleChildSocket = 0;
		int clientNameLength = sizeof(clientName);

		/* wait here */

		simpleChildSocket = accept(simpleSocket, (struct sockaddr *)&clientName, &clientNameLength);

		if (simpleChildSocket == -1) {
			fprintf(stderr, "Cannot accept connections!\n");
			close(simpleSocket);
			exit(1);
		}

		/* handle the new connection request */
		/* write out our message to the client */
		returnStatus = write(simpleChildSocket, WELCOMING, strlen(WELCOMING));

		if (returnStatus < 0) {
		fprintf(stderr, "Error writing to socket");
		}

		char buffer[256];
		//bzero(buffer,strlen(buffer)); // removed since i use memset

		/* get the message from the server */
		returnStatus = read(simpleChildSocket, buffer, sizeof(buffer));

		if (returnStatus > 0) {
			printf("Client has entered: %s",buffer);
		} else {
			fprintf(stderr, "Error reading from client\n");
		}
		
		/* extract integers from buffer	*/
		
		char delim[] = " ";
		char* ptr = strtok(buffer, delim);

		int total = 0;
		while (ptr != NULL) {
			total += atoi(ptr);
			ptr = strtok(NULL, delim);
		}
		/* creating an answer text to send to client */
		char answer[256];
		snprintf(answer, sizeof answer, "Sum of entered numbers = %d", total);

		memset(buffer, 0, sizeof buffer); // clear the buffer after using it

		returnStatus = write(simpleChildSocket, answer, strlen(answer));

		if (returnStatus < 0) {
		fprintf(stderr, "Error writing to socket");
		}		

		close(simpleChildSocket);			
	}

	close(simpleSocket);
	return 0;
}
