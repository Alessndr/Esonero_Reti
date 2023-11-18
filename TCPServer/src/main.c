/*
 * Including header files, omitting those that are included by other header files.
 */

#include "Socket_Utils.h"
#include "Math.h"
#include "Protocol.h"


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

	//create welcomeSocket for the Server to listen on


	int welcomeSocket;
	welcomeSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (welcomeSocket < 0) {
		errorHandler("Error: Couldn't create Welcome Socket.\n");
		clearWinsock();
		return -1;
	}
	//set connection settings

	socketAddress serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress)); // ensures that extra bytes contain 0 sad.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(PROTO_PORT); /* converts values between the host and network byte order. Specifically, htons() converts 16-bit quantities from host byte order to network byte order. */

	//binding welcomeSocket
	if (bind(welcomeSocket, (genericSocketAddress) &serverAddress, sizeof(serverAddress)) < 0){
		errorHandler("bind() failed.\n");
		closesocket(welcomeSocket);
		clearWinsock();
		return -1;
	}
	/*checks whether it is feasible to start the infinite loop and listen, or not.
	 *QLEN is set to 5.
	 *Deallocates resources upon failing.
	 */

	if (listen(welcomeSocket, QLEN) < 0){
		errorHandler("Error: listen() failed.\n");
		closesocket(welcomeSocket);
		clearWinsock();
		return -1;
	}

	/*
	 * If listen is successful, Server starts endlessly listening,
	 * waiting for a new connection.
	 */

	while(1){
		// accept new connection
		socketAddress clientAddress; // structure for the client address
		int clientAddress_len;
		int clientSocket;       // socket descriptor for the client

		printf("Waiting for a client to connect to the welcome socket to create a new connection socket...\n\n");


		clientAddress_len = sizeof(clientAddress); // set the size of the client address

		/*
		 * Server tries to connect to a Client requesting a new connection.
		 */

		if ((clientSocket = accept(welcomeSocket, (genericSocketAddress) &clientAddress,
				&clientAddress_len)) < 0) {
			errorHandler("Error: accept() failed.\n");
			// close connection
			closesocket(clientSocket);
			clearWinsock();
			return 0;
		}

		/*
		 * Prints Client's Address and Port number to stdout.
		 * See Socket_Utils.h/Socket_Utils.c
		 */

		printf("Connection established with: %s:%d\n", getAddress(&clientAddress),getPort(&clientAddress));

		//Reference to the message structure declared in Protocol.h
		Message msg;

		/*
		 * Second while loop to continue communicating with the Client via the same
		 * Connection Socket.
		 */


		while (1) {
			//Making sure there are no garbage values.
			memset(&msg, 0, sizeof(Message));

			strcpy(msg.expression, "You can do 4 operations with two integers:"
					"\n1. + to add"
					"\n2. - to subtract"
					"\n3. / to divide"
					"\n4. x to multiply"
					"\n4. = to end the connection with me\n\n");

			/*
			 * Sending instructions to the Client
			 */

			if (send(clientSocket, &msg, sizeof(Message), 0) != sizeof(Message)) {
				errorHandler("send() failed");
				closesocket(clientSocket);
				break;
			}


			memset(&msg, 0, sizeof(Message));

			if ((recv(clientSocket, &msg, sizeof(Message), 0)) <= 0) {

				errorHandler("recv() failed or connection closed abruptly!\n");
				closesocket(clientSocket);
				break;
			}

			/*
			 * Checking whether the Client wants to disconnect.
			 * If so, the Server will close the client socket,
			 * continue listening, and wait for a new connection.
			 */


			if(msg.expression[0] == '=' && strlen(msg.expression) == 1){
				printf("Client decided to disconnect...\n\n");
				closesocket(clientSocket);
				break;
			}


			/*
			 * Declaring the structure in Math.h to store:
			 * 		operands (int x and int y),
			 * 		operation to perform,
			 * 		result.
			 */

			Integers integers;
			memset(&integers, 0, sizeof(Integers));

			/*
			 * Starting parsing process.
			 * See Math.h/Math.c
			 */


			bool parseSuccess = parseStringOperation(msg.expression,&integers);

			/*
			 * Resetting msg to correctly put information inside.
			 */

			memset(&msg, 0, sizeof(Message));

			/*
			 * Checking the boolean value: if true, it goes on and checks with
			 * a switch statement which function to invoke.
			 * Otherwise, handles the ParsingException.
			 */

			if(parseSuccess){
				switch(integers.operation){
				case '+':
					add(&integers);
					break;
				case '/':
					division(&integers);
					break;
				case 'x':
					multi(&integers);
					break;
				case '-':
					sub(&integers);
					break;
				}
				strcpy(msg.expression, integers.result);
			}else
				/*
				 * Handling ParsingException: this message will be sent to the Client
				 */
				strcpy(msg.expression, "ParsingException: string format not recognized!\n");

			if (send(clientSocket, &msg, sizeof(Message), 0) != sizeof(Message)) {
				errorHandler("Error: send() failed");
				closesocket(clientSocket);
				break;
			}

		}
	}

	/*
	 * Under normal circumstances, this block of code will not be executed, because
	 * the Server will keep listening endlessly unless shutdown directly from an
	 * Administrator.
	 */

	closesocket(welcomeSocket);
	clearWinsock();
	return 0;
}
