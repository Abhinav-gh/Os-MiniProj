#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "../header/auth.h"
#include "../header/Librarian.h"
#include "../header/Admin.h"
#include "../header/borrower.h"


#define BUFFER_SIZE 1024


// Function prototypes
int authHandler(int new_socket);
User *read_user_data(const char *filename, const char *role);
int authenticate_user(const char *username, const char *password, const char *role);



// Function to read user data from file
User *read_user_data(const char *filename, const char *role)
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

    char tempUsername[BUFFER_SIZE];
    char tempPassword[BUFFER_SIZE];

    // Initialize user to NULL, indicating not found
    user = NULL;

    if (strcmp(role, "borrower") == 0) {
        while (fscanf(file, "%s %*s %s %*lld %*d %*d %*d %*d", tempUsername, tempPassword) == 2) {
            if (user == NULL) {
                user = (User *)malloc(sizeof(User));
                if (user == NULL) {
                    perror("Error allocating memory");
                    exit(EXIT_FAILURE);
                }
            }
            strcpy(user->username, tempUsername);
            strcpy(user->password, tempPassword);
            break; 
        }
    } else {
        while (fscanf(file, "%s %*s %*s %s", tempUsername, tempPassword) == 2) {
            if (strstr(tempUsername, role) != NULL) {
                if (user == NULL) {
                    user = (User *)malloc(sizeof(User));
                    if (user == NULL) {
                        perror("Error allocating memory");
                        exit(EXIT_FAILURE);
                    }
                }
                strcpy(user->username, tempUsername);
                strcpy(user->password, tempPassword);
                break; 
            }
        }
    }

    fclose(file);
    return user; // Return user found or NULL if not found
}


// Function to authenticate user
int authenticate_user(const char *username, const char *password, const char *role)
{
    User *user;
    if(strcmp(role, "admin") == 0){
        user = read_user_data("../database/users/admin.txt", role);

        printf("username: %s\n", user->username);
        printf("password: %s\n", user->password);

        if (user != NULL) {
            if (strcmp(user->username, username) == 0 && strcmp(user->password, password) == 0) {
                free(user);
                return 1; // User authenticated
            }
            free(user);
        }
    }

    if(strcmp(role, "librarian") == 0){
        user = read_user_data("../database/users/librarian.txt", role);
        if (user != NULL) {
            if (strcmp(user->username, username) == 0 && strcmp(user->password, password) == 0) {
                free(user);
                return 1; // User authenticated
            }
            free(user);
        }
    }

    if(strcmp(role, "borrower") == 0){
        user = read_user_data("../database/users/borrower.txt", role);
        if (user != NULL) {
            if (strcmp(user->username, username) == 0 && strcmp(user->password, password) == 0) {
                free(user);
                return 1; // User authenticated
            }
            free(user);
        }
    }

    return 0; // User not found or authentication failed
}

// Function to handle authentication for incoming connections
int authHandler(int new_socket)
{
    char username[BUFFER_SIZE] = {0};
    char password[BUFFER_SIZE] = {0};
    char role[BUFFER_SIZE] = {0};
    char buffer[BUFFER_SIZE] = {0};
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
        usleep(1000000);
        // ------->>>>> Addd by abhinav <<<<<-------
        if(strcmp(role, "borrower") == 0){
            send(new_socket, "borrower", strlen("borrower"), 0);
        } else if(strcmp(role, "librarian") == 1){
            send(new_socket, "librarian", strlen("librarian"), 0);
        } else if(strcmp(role, "admin") == 2){
            send(new_socket, "admin", strlen("admin"), 0);
        }
        return 1;
    } 

    send(new_socket, "Not Authenticated", strlen("Not Authenticated"), 0);
    return 0;
}
