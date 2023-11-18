/*
 * Including header files, omitting those that are included by other header files.
 */

#include "Protocol.h"
#include "Socket_Utils.h"


int main(int argc, char *argv[]) {
#if defined WIN32
	// Initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != 0) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif

	// create client socket
	int clientSocket = 0;
	clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (clientSocket < 0) {
			errorHandler("Socket was not created correctly.\n");
			clearWinsock();
			return -1;
		}



	// set connection settings
	socketAddress serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP
	serverAddress.sin_port = htons(PROTO_PORT); // Server port

		// connection
		if (connect(clientSocket, (genericSocketAddress) &serverAddress, sizeof(serverAddress)) < 0) {
			errorHandler("Connection failed.\n");
			closesocket(clientSocket);
			clearWinsock();
			return -1;
		}



	/*
	 * Loop until Client decides to shutdown the connection with the server.
	 */


	while(1){


		Message msg;
		/*
		 * Making sure there are no garbage values.
		 * This operation is made every time a new message has to be sent.
		 */
		memset(&msg, 0, sizeof(Message));

		/*
		 * Client receives from server a message and prints it on stdout.
		 */

		if ((recv(clientSocket, &msg, sizeof(Message), 0)) <= 0) {
			errorHandler("recv() failed or connection closed abruptly.\n");
			closesocket(clientSocket);
			clearWinsock();
			return -1;
		}
		printf("%s\n", msg.expression);

		memset(&msg, 0, sizeof(Message));

		/*
		 * User input is required.
		 */

		printf("Enter the string: ");
		if (fgets(msg.expression, sizeof(Message), stdin) != NULL) {
			char *newline = strchr(msg.expression, '\n');
			if (newline != NULL) {
				*newline = '\0';
			}
		}

		/*
		 * Checks if Client wants to disconnect.
		 * If so, the loop is terminated and the Client will send a special message to the
		 * server, communicating this intention. Then, the Client will close its socket
		 * and deallocate resources.
		 */

		if(msg.expression[0]=='=' && strlen(msg.expression) == 1){
			break;
		}

		/*
		 * The Client sends the message to the Server.
		 */


		if(send(clientSocket, &msg, sizeof(Message), 0) != sizeof(Message)) {
			errorHandler("send() failed");
			closesocket(clientSocket);
			clearWinsock();
			return -1;
		}

		memset(&msg, 0, sizeof(Message));

		/*
		 * The Client receives the result from the server and prints it to stdout.
		 */

		if ((recv(clientSocket, &msg, sizeof(Message), 0)) <= 0) {
			errorHandler("recv() failed or connection closed abruptly.\n");
			closesocket(clientSocket);
			clearWinsock();
			return -1;
		}

		printf("Result is: %s\n\n",msg.expression);
	}

	/*
	 * The Client decided to end the connection with the server.
	 * The Client closes its socket and deallocates resources.
	 * The Server will close the Client socket,
	 * but will keep listening and waiting for a new Client.
	 */

	Message closeMsg;
	memset(&closeMsg, 0, sizeof(Message));
	strcpy(closeMsg.expression, "=");
	send(clientSocket, &closeMsg, sizeof(Message), 0);
	closesocket(clientSocket);
	clearWinsock();


	return 0;
}
