#ifndef Protocol_h
#define Protocol_h

#define PROTO_PORT 8080
#define BUFFER_SIZE 2048
#define QLEN 5

typedef struct {
	char expression[BUFFER_SIZE];
} Message;


#endif /* Protocol_h */
