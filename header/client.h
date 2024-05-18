#ifndef CLIENT_H
#define CLIENT_H

#define PORT 8000
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

// Thread synchronization variables
pthread_mutex_t mutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Global variables
char buffer[BUFFER_SIZE] = {0};
int sock = 0;
int isAuthenticated = 0;

// Function prototypes
void connectToServer(const char *server_ip);
void loginMenuWrapper();
void FunctionalityMenu(int role);
void FunctionalityMenuWrapper();

#endif
