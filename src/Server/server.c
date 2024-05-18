#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "../../header/server.h"
#include "../../header/auth.h"

int IsAuthenticated = 0;

void startServer(int port) 
{
    // server_fd : file descriptor for server socket
    // new_socket : file descriptor for client socket
    // valread : number of bytes read from client
    int server_fd, new_socket, valread;

    // Address structure for server
    struct sockaddr_in address;

    // Set of socket options 
    int opt = 1;

    // Address length
    int addrlen = sizeof(address);

    // Buffer to store message from client
    char buffer[BUFFER_SIZE] = {0};


    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }


    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    write(1, "Waiting for the client.....\n", sizeof("Waiting for the client.....\n"));

    while (1) {


        // Accept the incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        write(1, "Connected to the client.....\n", sizeof("Connected to the client.....\n"));
        send(new_socket, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE), 0);
        int pid = fork();

        if (pid == 0) {

            close(server_fd);


            // Handle authentication
            IsAuthenticated =  authHandler(new_socket); // Step 1 Authentication. Done
            funcHandler(new_socket, IsAuthenticated); // Step 2 Give functionality.

                   

            close(new_socket); 
            exit(0); 
        } 
        
        else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } 
        
        else {
            // Close client socket in parent process
            close(new_socket); 
        }
    }

    return;
}

int main() {
    // Start the server
    startServer(PORT);

    return 0;
}
