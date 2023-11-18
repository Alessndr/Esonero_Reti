
#ifndef Socket_Utils_h
#define Socket_Utils_h
#include "Constants.h"

/*
 * Free allocated resources for the Windows Operating System.
 */

void clearWinsock(void);

/*
 * Prints a message error to stdout.
 */

void errorHandler(char *);

/*
 * Used to get the Client's Address and Port Number.
 */

char* getAddress(socketAddress_h);

int getPort(socketAddress_h);


#endif /* Socket_Utils_h */
