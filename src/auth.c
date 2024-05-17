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
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    User *user = (User *)malloc(sizeof(User));
    if (user == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    User *userArr = (User *)malloc(MAX_USERS * sizeof(User));
    if (userArr == NULL)
    {
        perror("Error allocating memory for user array");
        exit(EXIT_FAILURE);
    }
    char tempUsername[BUFFER_SIZE];
    char tempPassword[BUFFER_SIZE];

    // Initialize user to NULL, indicating not found
    user = NULL;
    int iteration = 0;

    if (strcmp(role, "borrower") == 0)
    {

        while (fscanf(file, "%s %*s %s %*lld %*d %*s %*s %*s %*d %*d %*d ", tempUsername, tempPassword) == 2)
        {
            // printf("Iteration %d\n", iteration);
            user = (User *)malloc(sizeof(User));
            if (user == NULL)
            {
                perror("Error allocating memory");
                exit(EXIT_FAILURE);
            }
            strcpy(user->username, tempUsername);
            strcpy(user->password, tempPassword);
            userArr[iteration++] = *user;
            if (feof(file))
                break;
        }
    }
    else
    {
        while (fscanf(file, "%s %*s %*s %s", tempUsername, tempPassword) == 2)
        {
            // printf("Iteration %d\n", iteration);
            user = (User *)malloc(sizeof(User));
            if (user == NULL)
            {
                perror("Error allocating memory");
                exit(EXIT_FAILURE);
            }
            strcpy(user->username, tempUsername);
            strcpy(user->password, tempPassword);
            userArr[iteration++] = *user;
            if (feof(file))
                break;
        }
    }

    fclose(file);
    return userArr; // Return user found or NULL if not found
}

// Function to authenticate user
int authenticate_user(const char *username, const char *password, const char *role)
{
    User *userArr;  // array of users
    if (strcmp(role, "admin") == 0)
    {
        userArr = read_user_data("../database/users/admin.txt", role);
    }
    if (strcmp(role, "librarian") == 0)
    {
        userArr = read_user_data("../database/users/librarian.txt", role);
    }

    if (strcmp(role, "borrower") == 0)
    {
        userArr = read_user_data("../database/users/borrower.txt", role);
    }
    int result = UserMatchFrom_userArr(userArr, username, password);
    free(userArr);
    return result;

}
// Function to match user from user array. If user is found and authenticated, return 1, else return 0
int UserMatchFrom_userArr(User *userArr, const char *username, const char *password)
{
    for (int i = 0; i < MAX_USERS; i++)
    {
        if (strcmp(userArr[i].username, username) == 0 && strcmp(userArr[i].password, password) == 0)
        {
            return 1; // User authenticated.
        }
    }
    return 0;   // User not authenticated
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
    if (authenticate_user(username, password, role))
    {
        send(new_socket, "Authenticated", strlen("Authenticated"), 0);
        // printf("Th role is %s\n", role);
        usleep(3000000);
        // ------->>>>> Addd by abhinav <<<<<-------
        if (strcmp(role, "borrower") == 0)
        {
            send(new_socket, "borrower", strlen("borrower"), 0);
        }
        else if (strcmp(role, "librarian") == 0)
        {
            send(new_socket, "librarian", strlen("librarian"), 0);
        }
        else if (strcmp(role, "admin") == 0)
        {
            send(new_socket, "admin", strlen("admin"), 0);
        }
        return 1;
    }

    send(new_socket, "Not Authenticated", strlen("Not Authenticated"), 0);
    return 0;
}
