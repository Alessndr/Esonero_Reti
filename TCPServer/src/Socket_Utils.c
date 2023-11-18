#include "Socket_Utils.h"

/*
 *
 *Sends an error message on stdout
 *
 */

void errorHandler(char *errorMessage) {
	printf("%s", errorMessage);
}

void clearWinsock(void){
#if defined WIN32
	WSACleanup();
#endif
}


char* getAddress(socketAddress_h socketAddress){
	return inet_ntoa(socketAddress->sin_addr);
}
int getPort(socketAddress_h socketAddress){
	return ntohs(socketAddress->sin_port);
}


