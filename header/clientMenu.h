
#ifndef CLIENT_MENU_H
#define CLIENT_MENU_H


#define BUFFER_SIZE 1024
#define MAX_NAME_LENGTH 100


typedef struct MsgPacket {
    const char *username;
    const char *role;
    const char **payload;
    int payload_count;
    int choice;
} MsgPacket;




void loginMenu(int sock);

#endif
