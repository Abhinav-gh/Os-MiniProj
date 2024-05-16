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


void loginMenu(int sock)
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
    usleep(100000);
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
    printf("%s\n", message);

}


// Borrower Menu
void borrowerMenu(int sock)
{
    printf("----------------Borrower Menu----------------\n");
    printf("1.) Show all genres\n");
    printf("2.) Show all books\n");
    printf("2.) Borrow a book\n");
    printf("3.) Return a book\n");
    printf("4.) Show borrowed books\n");
    printf("6.) My Details\n");
    printf("7.) Check fine\n");
    printf("8.) Change Password\n");
    printf("9.) Update Contact\n");
    printf("10. Check Due Dates\n");
    printf("11.) Logout\n");
}






