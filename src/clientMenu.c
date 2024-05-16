#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../header/clientMenu.h"

#define BUFFER_SIZE 1024
char message[BUFFER_SIZE] = {0};


int loginMenu(int sock) // returns if authenticated or not
{

    int choice;
    char username[100];
    char password[100];

    printf("----------------Login Menu----------------\n");
    printf("1.) Borrower\n");
    printf("2.) Librarian\n");
    printf("3.) Admin\n");
    printf("4.) Logout application\n\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    printf("----------------Auth Menu----------------\n");
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    send(sock, username, strlen(username), 0);
    usleep(100000); // micro seconds
    send(sock, password, strlen(password), 0);
    usleep(100000);
    
    switch (choice)
    {
        case 1:
            send(sock, "borrower", strlen("borrower"), 0);
            break;
        case 2:
            send(sock, "librarian", strlen("librarian"), 0);
            break;
        case 3:
            send(sock, "admin", strlen("admin"), 0);
            break;
        case 4:
            printf("Exiting application...\n");
            printf("Thank you for using our application.\n Take care!\n");
            sleep(1);
            exit(0);
            break;
    }

    // Listen for incoming messages from the server
    read(sock, message, BUFFER_SIZE);       
    printf("%s\n", message);    // server tells if authenticated or not
    if (strcmp(message, "Authenticated") == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

// ---------->>>>>>>>>>> Added by Abhinav  <<<<<<<<<<<<----------
void BorrowerMenu(int sock){
    int choice;
    printf("----------------Borrower Menu----------------\n");
    printf("1.) Borrow a book\n");
    printf("2.) Return a book\n");
    printf("3.) View borrowed books\n");
    printf("4.) Pay fine\n");
    printf("5.) Logout\n\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            send(sock, "borrow", strlen("borrow"), 0);
            break;
        case 2:
            send(sock, "return", strlen("return"), 0);
            break;
        case 3:
            send(sock, "view", strlen("view"), 0);
            break;
        case 4:
            send(sock, "pay", strlen("pay"), 0);
            break;
        case 5:
            send(sock, "logout", strlen("logout"), 0);
            break;
    }

    // // Listen for incoming messages from the server
    // read(sock, message, BUFFER_SIZE);       
    // printf("%s\n", message);    // server tells if authenticated or not
    
}
void LibrarianMenu(int sock){
    int choice;
    printf("----------------Librarian Menu----------------\n");
    printf("1.) Add a book\n");
    printf("2.) Remove a book\n");
    printf("3.) View all books\n");
    printf("4.) View all borrowers\n");
    printf("5.) Logout\n\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            send(sock, "add", strlen("add"), 0);
            break;
        case 2:
            send(sock, "remove", strlen("remove"), 0);
            break;
        case 3:
            send(sock, "viewBooks", strlen("viewBooks"), 0);
            break;
        case 4:
            send(sock, "viewBorrowers", strlen("viewBorrowers"), 0);
            break;
        case 5:
            send(sock, "logout", strlen("logout"), 0);
            break;
    }

    // Listen for incoming messages from the server
    read(sock, message, BUFFER_SIZE);       
    printf("%s\n", message);    // server tells if authenticated or not
}
void AdminMenu(int sock){
    int choice;
    printf("----------------Admin Menu----------------\n");
    printf("1.) Add a librarian\n");
    printf("2.) Remove a librarian\n");
    printf("3.) View all librarians\n");
    printf("4.) View all borrowers\n");
    printf("5.) Logout\n\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            send(sock, "addLibrarian", strlen("addLibrarian"), 0);
            break;
        case 2:
            send(sock, "removeLibrarian", strlen("removeLibrarian"), 0);
            break;
        case 3:
            send(sock, "viewLibrarians", strlen("viewLibrarians"), 0);
            break;
        case 4:
            send(sock, "viewBorrowers", strlen("viewBorrowers"), 0);
            break;
        case 5:
            send(sock, "logout", strlen("logout"), 0);
            break;
    }

    // Listen for incoming messages from the server
    read(sock, message, BUFFER_SIZE);       
    printf("%s\n", message);    // server tells if authenticated or not
}
// ------------->>>>> Finished adding by Abhinav  <<<<<<<<<<<<------------





