// server.h

#ifndef SERVER_H
#define SERVER_H

#define PORT 8000
#define MAX_CLIENTS 250
#define WELCOME_MESSAGE "Welcome to the server of Online Library Management System\n"


#define BUFFER_SIZE 1024
#define MAX_NAME_LENGTH 100


typedef struct MsgPacket
{
    const char *username;
    const char *role;
    const char *payload;
    int choice;
} MsgPacket;




// Function prototypes
void startServer(int port);




#endif
