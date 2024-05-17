// server.h

#ifndef SERVER_H
#define SERVER_H

#define PORT 8000
#define MAX_CLIENTS 250
#define WELCOME_MESSAGE "Welcome to the server of Online Library Management System\n"


#define BUFFER_SIZE 1024
// #define MAX_NAME_LENGTH 100
// #define MAX_EMAIL_LENGTH 50
// #define MAX_PASSWORD_LENGTH 50
// #define MAX_USERNAME_LENGTH 50
// #define MAX_ID_LENGTH 20
// #define MAX_CONTACT_LENGTH 20
// #define MAX_ISBN_LENGTH 20
// #define MAX_TITLE_LENGTH 100
// #define MAX_AUTHOR_LENGTH 50
// #define MAX_GENRE_LENGTH 50


typedef struct MsgPacket {
    const char *username;
    const char *role;
    const char **payload;
    int payload_count;
    int choice;
} MsgPacket;




// Function prototypes
void startServer(int port);
void free_packet(MsgPacket *packet);
void send_packet(int new_socket, MsgPacket *packet);
void receive_packet(int new_socket, MsgPacket *packet);



#endif
