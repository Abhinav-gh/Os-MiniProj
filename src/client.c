#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h> // Include pthread library for threading
#include "../header/client.h"
#include "../header/clientMenu.h"

// Thread synchronization variables
pthread_mutex_t mutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Global variables
char buffer[BUFFER_SIZE] = {0};
int sock = 0;

// ---------->>>>>>>>> Added by Abhinav <<<<<<<<<<----------
int isAuthenticated = 0;
// Function prototypes
void connectToServer(const char *server_ip);
void loginMenuWrapper();

// Function to execute loginMenu with mutex protection
void loginMenuWrapper()
{
    pthread_mutex_lock(&mutex);
    isAuthenticated = loginMenu(sock);
    pthread_mutex_unlock(&mutex);
}

void connectToServer(const char *server_ip)
{
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    read(sock, buffer, BUFFER_SIZE);
    printf("%s\n", buffer);
    memset(buffer, 0, BUFFER_SIZE);

    loginMenuWrapper(); // Call loginMenu through the wrapper function
}
// --->>>>>>>ADDED BY ABHINAV <<<<<<----
void FunctionalityMenu(int role)
{
    role == -1 ? perror("Role not recognized by server\n") : NULL; // error handling
    pthread_mutex_lock(&mutex);

    role == 0 ? BorrowerMenu(sock) : role == 1 ? LibrarianMenu(sock)
                                               : AdminMenu(sock);
    pthread_mutex_unlock(&mutex);
}

FunctionalityMenuWrapper()
{
    int role = -1;
    if (isAuthenticated)
    {
        read(sock, buffer, BUFFER_SIZE);
        if (strcmp(buffer, "borrower") == 0)
        {
            role = 0;
        }
        else if (strcmp(buffer, "librarian") == 0)
        {
            role = 1;
        }
        else if (strcmp(buffer, "admin") == 0)
        {
            role = 2;
        }
        else
        { // handle error
            perror("Role not recognized by server\n");
            exit(1);
        }
        FunctionalityMenu(role);
    }
    else
    {
        printf("Authentication failed\n");
    }
}

int main()
{

    pthread_mutex_init(&mutex, NULL); // Initialize the mutex
    connectToServer(SERVER_IP);       // Step1 is connecting and auth. Done
    FunctionalityMenuWrapper();       // Step2 is Post auth to call the functionality menu

    pthread_mutex_destroy(&mutex); // Destroy the mutex before exiting

    return 0;
}
