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

            char title[100];
            char author[100];
            char ISBN[100];
            char genre[100];
            int numCopies, isAvailable, yearPublished;
            printf("Enter the genre of the book: ");
            scanf("%s", genre);
            send(sock, genre, strlen(genre), 0);
            printf("Enter the title of the book: ");
            scanf("%s", title);
            printf("Enter the author of the book: ");
            scanf("%s", author);
            printf("Enter the ISBN of the book: ");
            scanf("%s", ISBN);
            printf("Enter the number of copies of the book: ");
            scanf("%d", &numCopies);
            printf("Enter the year of publication of the book: ");
            scanf("%d", &yearPublished);
            // now mak a book struct and send it to server
            struct LibraryBookPacket bookpacket;
            // add data to bookpacket
            strcpy(bookpacket.title, title);
            strcpy(bookpacket.author, author);
            strcpy(bookpacket.ISBN, ISBN);
            bookpacket.numCopies = numCopies;
            bookpacket.isAvailable = 1;
            bookpacket.yearPublished = yearPublished;

            // print all the fields of book packet
            printf("Title: %s\n", bookpacket.title);
            printf("Author: %s\n", bookpacket.author);
            printf("ISBN: %s\n", bookpacket.ISBN);
            printf("Num Copies: %d\n", bookpacket.numCopies);
            printf("Is Available: %d\n", bookpacket.isAvailable);
            printf("Year Published: %d\n", bookpacket.yearPublished);

            // send the bookpacket to server
            send(sock, &bookpacket, sizeof(bookpacket), 0);

            break;
        case 2:
            // prepare to send a 2d aray of 2 strings, isbn and genre
            send(sock, "remove", strlen("remove"), 0);
            char arr[2][BUFFER_SIZE] = {0};
            printf("Enter the ISBN of the book you want to remove: ");
            scanf("%s", arr[0]);
            printf("Enter the genre of the book you want to remove: ");
            scanf("%s", arr[1]);
            // print arr
            // printf("arr[0]: %s\n", arr[0]);
            // printf("arr[1]: %s\n", arr[1]);
            // send the 2d array to server
            printf("sending...\n");
            send(sock,arr[0], strlen(arr[0]), 0);
            usleep(100000);
            send(sock,arr[1], strlen(arr[1]), 0);
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
// ------------->>>>> Finished adding by Abhinav  <<<<<<<<<<<<------------
