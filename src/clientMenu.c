#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "../header/clientMenu.h"

#define bufferMsg_SIZE 1024
#define MAX_NAME_LENGTH 100

char bufferMsg[bufferMsg_SIZE] = {0};


// Function prototypes
void loginMenu(int sock);
void handleLogin(int sock, int choice);
void handleAuthentication(int sock, const char *username);
void borrowerMenu(int sock, const char *username);
void librarianMenu(int sock, const char *username);
void adminMenu(int sock, const char *username);
void send_packet(int sock, MsgPacket *packet);


void send_packet(int sock, MsgPacket *packet) {
    int len = strlen(packet->username) + strlen(packet->role) + sizeof(int) * 2;
    for (int i = 0; i < packet->payload_count; ++i) {
        len += strlen(packet->payload[i]) + 1;
    }

    char *bufferMsg = malloc(len + 1);
    if (!bufferMsg) {
        perror("Failed to allocate memory for bufferMsg");
        exit(EXIT_FAILURE);
    }

    // Construct the message
    int pos = sprintf(bufferMsg, "%s|%s|%d|%d", packet->username, packet->role, packet->choice, packet->payload_count);
    for (int i = 0; i < packet->payload_count; ++i) {
        pos += sprintf(bufferMsg + pos, "|%s", packet->payload[i]);
    }

    send(sock, bufferMsg, len + 1, 0);
    free(bufferMsg);
}




void loginMenu(int sock) {
    int choice;

    printf("----------------Login Menu----------------\n");
    printf("1.) Borrower\n");
    printf("2.) Librarian\n");
    printf("3.) Admin\n");
    printf("4.) Exit application\n\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 4) {
        printf("Exiting application...\n");
        printf("Thank you for using our application.\nTake care!\n");
        sleep(1);
        exit(0);
    }

    handleLogin(sock, choice);
}

void handleLogin(int sock, int choice) {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];

    printf("----------------Auth Menu----------------\n");
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    send(sock, username, strlen(username), 0);
    usleep(100000);  // Sleep for 100 milliseconds
    send(sock, password, strlen(password), 0);
    usleep(100000);

    const char *role;
    switch (choice) {
        case 1:
            role = "borrower";
            break;
        case 2:
            role = "librarian";
            break;
        case 3:
            role = "admin";
            break;
        default:
            fprintf(stderr, "Invalid choice\n");
            return;
    }

    send(sock, role, strlen(role), 0);
    handleAuthentication(sock, username);
}

void handleAuthentication(int sock, const char *username) {
    char message[bufferMsg_SIZE];

    read(sock, message, bufferMsg_SIZE);

    if (strcmp(message, "Authenticated") == 0) {
        printf("Login Successful\n");

        if (username[0] == 'L') {
            librarianMenu(sock, username);
        } else if (username[0] == 'A') {
            adminMenu(sock, username);
        } else {
            borrowerMenu(sock, username);
        }
    } else {
        printf("Login Failed\n");
    }
}



void borrowerMenu(int sock, const char *username) {
    while (1) {
        int choice;
        printf("----------------Borrower Menu----------------\n");
        printf("1.) Show all genres\n");
        printf("2.) Show all books\n");
        printf("3.) Borrow a book\n");
        printf("4.) Return a book\n");
        printf("5.) Show borrowed books\n");
        printf("6.) My Details\n");
        printf("7.) Check fine\n");
        printf("8.) Change Password\n");
        printf("9.) Update Contact\n");
        printf("10.) Check Due Dates\n");
        printf("11.) Logout\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        const char *payload[1] = {NULL};

        if (choice == 3 || choice == 4) {
            char isbn[MAX_NAME_LENGTH];
            printf("Enter the ISBN of the book: ");
            scanf("%s", isbn);
            payload[0] = strdup(isbn);
        } else if (choice == 8) {
            char new_password[MAX_NAME_LENGTH];
            printf("Enter your new password: ");
            scanf("%s", new_password);
            payload[0] = strdup(new_password);
        } else if (choice == 9) {
            char new_contact[MAX_NAME_LENGTH];
            printf("Enter your new contact number: ");
            scanf("%s", new_contact);
            payload[0] = strdup(new_contact);
        }

        MsgPacket packet = {
            .username = username,
            .role = "borrower",
            .payload = payload[0] ? payload : NULL,
            .payload_count = payload[0] ? 1 : 0,
            .choice = choice
        };

        send_packet(sock, &packet);
        
        while (1)
        {
            ssize_t bytesRead = read(sock, bufferMsg, bufferMsg_SIZE);
            if (bytesRead < 0) {
                perror("recv");
                break;
            } else if (bytesRead == 0) {
                break;
            } else if (bufferMsg[0] == '\0') {
                break;
            }
   

            printf("Received genre: %s\n", bufferMsg);
            memset(bufferMsg, 0, bufferMsg_SIZE);
        }

        // Free dynamically allocated payload strings
        if (payload[0]) {
            free((void*)payload[0]);
        }
    }
}






void librarianMenu(int sock, const char *username) {
    while (1) {

        int choice;

        printf("----------------Librarian Menu----------------\n");
        printf("1.) Add a book\n");
        printf("2.) Remove a book\n");
        printf("3.) Update a book\n");
        printf("4.) Show all books\n");
        printf("5.) Show all genres\n");
        printf("6.) Show all borrowers\n");
        printf("7.) Show all borrowed books\n");
        printf("8.) Show all late books\n");
        printf("9.) Show all fines\n");
        printf("10.) Logout\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        const char *payload[1] = {NULL};

        if (choice == 1) {
            char title[MAX_NAME_LENGTH];
            printf("Enter the title of the book: ");
            scanf("%s", title);
            payload[0] = strdup(title);
        } else if (choice == 2 || choice == 3) {
            char isbn[MAX_NAME_LENGTH];
            printf("Enter the ISBN of the book: ");
            scanf("%s", isbn);
            payload[0] = strdup(isbn);
        }

        MsgPacket packet = {
            .username = username,
            .role = "librarian",
            .payload = payload[0] ? payload : NULL,
            .payload_count = payload[0] ? 1 : 0,
            .choice = choice
        };

        send_packet(sock, &packet);

        while(1)
        {
            ssize_t bytesRead = read(sock, bufferMsg, bufferMsg_SIZE);
            if (bytesRead < 0) {
                perror("recv");
                break;
            } else if (bytesRead == 0) {
                // Connection closed by server
                break;
            } else if (bufferMsg[0] == '\0') {
                // End of transmission indicator
                break;
            }

            printf("Received genre: %s\n", bufferMsg);
            memset(bufferMsg, 0, bufferMsg_SIZE);
        }

        // Free dynamically allocated payload strings
        if (payload[0]) {
            free((void*)payload[0]);
        }

    }
}






void adminMenu(int sock, const char *username) {
    while (1) {
        int choice;

        printf("----------------Admin Menu----------------\n");
        printf("1.) Add a book\n");
        printf("2.) Remove a book\n");
        printf("3.) Update a book\n");
        printf("4.) Show all books\n");
        printf("5.) Show all genres\n");
        printf("6.) Show all borrowers\n");
        printf("7.) Show all borrowed books\n");
        printf("8.) Show all late books\n");
        printf("9.) Show all fines\n");
        printf("10.) Add a Librarian\n");
        printf("11.) Remove a Librarian\n");
        printf("12.) Update a Librarian\n");
        printf("13.) Show all Librarians\n");
        printf("14.) Logout\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        const char *payload[1] = {NULL};

        if (choice == 1) {
            char title[MAX_NAME_LENGTH];
            printf("Enter the title of the book: ");
            scanf("%s", title);
            payload[0] = strdup(title);
        } else if (choice == 2 || choice == 3) {
            char isbn[MAX_NAME_LENGTH];
            printf("Enter the ISBN of the book: ");
            scanf("%s", isbn);
            payload[0] = strdup(isbn);
        }

        MsgPacket packet = {
            .username = username,
            .role = "admin",
            .payload = payload[0] ? payload : NULL,
            .payload_count = payload[0] ? 1 : 0,
            .choice = choice
        };

        send_packet(sock, &packet);

        while(1)
        {
            ssize_t bytesRead = read(sock, bufferMsg, bufferMsg_SIZE);
            if (bytesRead < 0) {
                perror("recv");
                break;
            } else if (bytesRead == 0) {
                // Connection closed by server
                break;
            } else if (bufferMsg[0] == '\0') {
                // End of transmission indicator
                break;
            }

            printf("Received genre: %s\n", bufferMsg);
            memset(bufferMsg, 0, bufferMsg_SIZE);
        }

        // Free dynamically allocated payload strings
        if (payload[0]) {
            free((void*)payload[0]);
        }
    }
}

