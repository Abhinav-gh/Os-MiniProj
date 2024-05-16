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
    printf("%s\n", message); // server tells if authenticated or not
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
void BorrowerMenuPrinter()
{
    printf("----------------Borrower Menu----------------\n");
    printf("1.) Borrow a book\n");
    printf("2.) Return a book\n");
    printf("3.) View all books\n");
    printf("4.) Search a book\n");
    printf("5.) Logout\n\n");
    printf("6.) Show menu again\n\n");
    printf("Enter your choice: ");
}
void BorrowerMenu(int sock)
{
    send(sock, "borrower", strlen("borrower"), 0); // send role to server
    BorrowerMenuPrinter();

    int choice;

    while (choice != 5)
    {

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
            send(sock, "search", strlen("search"), 0);
            printf("Enter the book ISBN you want to search: ");
            char isbn[100];
            scanf("%s", isbn);
            send(sock, isbn, strlen(isbn), 0);
            printf("Enter the genre of the book: ");
            char genre[100];
            scanf("%s", genre);
            send(sock, genre, strlen(genre), 0);
            break;
        case 5:
            send(sock, "logout", strlen("logout"), 0);
            break;

        case 6:
            BorrowerMenuPrinter();
            continue;
        default:
            printf("Invalid choice\n");
        }
        // Listen for incoming messages from the server
        read(sock, message, BUFFER_SIZE);
        printf("%s\n", message); // server tells if authenticated or not
        printf("Enter your choice: ");
    }
}
void LibraryMenuPrinter()
{
    printf("----------------Librarian Menu----------------\n");
    printf("1.) Add a book\n");
    printf("2.) Remove a book\n");
    printf("3.) View all books\n");
    printf("4.) View all borrowers\n");
    printf("5.) Logout\n\n");
    printf("6.) Show menu again\n\n");
    printf("Enter your choice: ");
}
void LibrarianMenu(int sock)
{
    send(sock, "librarian", strlen("librarian"), 0); // send role to server
    LibraryMenuPrinter();
    int choice;
    while (choice != 5)
    {
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
        case 6:
            LibraryMenuPrinter();
            continue;
        default:
            printf("Invalid choice\n");
        }

        // Listen for incoming messages from the server
        read(sock, message, BUFFER_SIZE);
        printf("%s\n", message); // server tells if authenticated or not
        printf("Enter your choice: ");
    }
}
void AdminMenuPrinter()
{
    printf("----------------Admin Menu----------------\n");
    printf("1.) Add a librarian\n");
    printf("2.) Remove a librarian\n");
    printf("3.) View all librarians\n");
    printf("4.) View all borrowers\n");
    printf("5.) Logout\n\n");
    printf("6.) Show menu again\n\n");
    printf("Enter your choice: ");
}
void AdminMenu(int sock)
{
    send(sock, "admin", strlen("admin"), 0); // send role to server
    AdminMenuPrinter();
    int choice;
    while (choice != 5)
    {

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
        case 6:
            AdminMenuPrinter();
            continue;
        default:
            printf("Invalid choice\n");
        }

        // Listen for incoming messages from the server
        read(sock, message, BUFFER_SIZE);
        printf("%s\n", message); // server tells if authenticated or not
        printf("Enter your choice: ");
    }
}
// ------------->>>>> Finished adding by Abhinav  <<<<<<<<<<<<------------
