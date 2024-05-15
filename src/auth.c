#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "../header/auth.h"

char buffer[BUFFER_SIZE] = {0};

void authHandler(int new_socket)
{
    char username[BUFFER_SIZE] = {0};
    char password[BUFFER_SIZE] = {0};
    char role[BUFFER_SIZE] = {0};
    int valread;

    // Read username
    valread = read(new_socket, buffer, BUFFER_SIZE);
    buffer[valread] = '\0'; 
    strncpy(username, buffer, BUFFER_SIZE - 1); 

    // Read password
    valread = read(new_socket, buffer, BUFFER_SIZE);
    buffer[valread] = '\0'; 
    strncpy(password, buffer, BUFFER_SIZE - 1); 

    // Read role
    valread = read(new_socket, buffer, BUFFER_SIZE);
    buffer[valread] = '\0';
    strncpy(role, buffer, BUFFER_SIZE - 1); 


    // Authenticate user
    if (authenticate_user(username, password, role)) {
        send(new_socket, "Authenticated", strlen("Authenticated"), 0);
    } else {
        send(new_socket, "Not Authenticated", strlen("Not Authenticated"), 0);
    }
}


// Function to read user data from file
User *read_user_data(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    User *user = (User *)malloc(sizeof(User));
    if (user == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%s %s", user->username, user->password);
    fclose(file);
    return user;
}


// Function to authenticate user
int authenticate_user(const char *username, const char *password, const char *role)
{
    if(strcmp(role, "admin") == 0){
        User *user = read_user_data("../database/admin.txt");
    
        if (strcmp(user->username, username) == 0 && strcmp(user->password, password) == 0) 
            return 1;    
    }

    if(strcmp(role, "librarian") == 0){
        User *user = read_user_data("../database/librarian.txt");
        if (strcmp(user->username, username) == 0 && strcmp(user->password, password) == 0) 
            return 1;       
    }

    if(strcmp(role, "borrower") == 0){
        User *user = read_user_data("../database/borrower.txt");
        if (strcmp(user->username, username) == 0 && strcmp(user->password, password) == 0) 
            return 1;
    }

    return 0;
}
