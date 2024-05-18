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
char clientUsername[BUFFER_SIZE] = {0};
char clientPassword[BUFFER_SIZE] = {0};
char message[BUFFER_SIZE] = {0};
int isfineCalculated = 0;

int loginMenu(int sock) // returns if authenticated or not
{

    int choice;
    char username[100],password[100];

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
        strcpy(clientUsername, username);
        strcpy(clientPassword, password);
        // printf("Welcome %s\n", clientUsername);
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
    printf("5.) Pay fine\n");
    printf("99.) Logout\n");
    printf("100.) Show menu again\n\n");
    printf("Enter your choice: (99:Logout ; 100: Menu)");
}
void LibraryMenuPrinter()
{
    printf("----------------Librarian Menu----------------\n");
    printf("1.) Add a book\n");
    printf("2.) Remove a book\n");
    printf("3.) View all books\n");
    printf("4.) View all borrowers\n");
    printf("5.) Add new borrowers\n");
    printf("6.) Remove a borrower\n");
    printf("99.) Logout\n");
    printf("100.) Show menu again\n\n");
    printf("Enter your choice: (99:Logout ; 100: Menu)");
}
void AdminMenuPrinter()
{
    printf("----------------Admin Menu----------------\n");
    printf("1.) Add a librarian\n");
    printf("2.) Remove a librarian\n");
    printf("3.) View all librarians\n");
    printf("4.) View all borrowers\n");
    printf("99.) Logout\n");
    printf("100.) Show menu again\n\n");
    printf("Enter your choice: (99:Logout ; 100: Menu)");
}
void ResponseHandler(int sock, int choice)
{
    // Listen for incoming messages from the server
        memset(message, 0, sizeof(message));
        read(sock, message, BUFFER_SIZE);
        if(choice==99)
        {
            printf("Thank you for using our application.\n Take care!\n");
            return;
        }
        printf("%s\n", message); // server tells if authenticated or not
        printf("Enter your choice: (99:Logout ; 100: Menu)");
}

void AdminMenu(int sock)
{
    send(sock, "admin", strlen("admin"), 0); // send role to server
    AdminMenuPrinter();
    int choice=0;
    while (choice != 99)
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
        case 99:
            send(sock, "logout", strlen("logout"), 0);
            break;
        case 100:
            AdminMenuPrinter();
            continue;
        default:
            printf("Invalid choice\n");
        }

        ResponseHandler(sock,choice);
    }
}
void BorrowerMenu(int sock)
{
    send(sock, "borrower", strlen("borrower"), 0); // send role to server
    BorrowerMenuPrinter();

    int choice=0;

    while (choice != 99)
    {

        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            // take the isbn, genre and username of the borrower
            send(sock, "borrow", strlen("borrow"), 0);
            printf("Enter the ISBN of the book you want to borrow: ");
            char isbnToBorrow[100],genreToBorrow[100];
            scanf("%s", isbnToBorrow);
            printf("Enter the genre of the book you want to borrow: ");
            scanf("%s", genreToBorrow);
            send(sock, isbnToBorrow, strlen(isbnToBorrow), 0);
            usleep(100000);
            send(sock, genreToBorrow, strlen(genreToBorrow), 0);
            usleep(100000);
            send(sock, clientUsername, strlen(clientUsername), 0);
            break;
        case 2:
            send(sock, "return", strlen("return"), 0);
            // take the isbn, genre and username of the borrower
            printf("Enter the ISBN of the book you want to return: ");
            char isbnToReturn[100],genreToReturn[100];
            scanf("%s", isbnToReturn);
            printf("Enter the genre of the book you want to return: ");
            scanf("%s", genreToReturn);
            // send the isbn, genre and username to server
            send(sock, isbnToReturn, strlen(isbnToReturn), 0);
            usleep(100000);
            send(sock, genreToReturn, strlen(genreToReturn), 0);
            usleep(100000);
            send(sock, clientUsername, strlen(clientUsername), 0);
            usleep(100000);
            isfineCalculated = 1;
            break;
        case 3:
            send(sock, "view", strlen("view"), 0);
            break;
        case 4:
            send(sock, "search", strlen("search"), 0);
            printf("Enter the book ISBN you want to search: ");
            char isbn[100],genre[100];
            scanf("%s", isbn);
            send(sock, isbn, strlen(isbn), 0);
            printf("Enter the genre of the book: ");
            scanf("%s", genre);
            send(sock, genre, strlen(genre), 0);
            break;
        case 5:
            // pay fine. send username and recieve fine from server
            send(sock, "payFine", strlen("payFine"), 0);
            usleep(100000);
            send(sock, &isfineCalculated, sizeof(int), 0);
            // clear message
            memset(message, 0, sizeof(message));
            recv(sock, message, BUFFER_SIZE, 0);
            message[strlen(message)] = '\0';
            printf("%s.", message);
            if (strcmp(message, "Fine not calculated. First go to return") == 0)
            {
                printf("Fine not calculated. First go to return\n");
                break;
            }
            memset(message, 0, sizeof(message));
            send(sock, clientUsername, strlen(clientUsername), 0);

            int fine;
            recv(sock, &fine, sizeof(int), 0);
            // check if fine is to be paid
            if (fine != 0)
            {
                printf(" Fine to be paid: %d\n", fine);
                printf("Do you want to pay the fine? (y/n): ");
                char answer;
                scanf(" %c", &answer);
                if (answer == 'y')
                {
                    printf("Paying fine...(input amount)\n");

                    // make user pay fine. Verify the same
                    int userfine;
                    scanf("%d", &userfine);
                    if (userfine < fine)
                    {
                        printf("Amount paid is less than the fine\n");
                        send(sock, "Fine not paid", strlen("Fine not paid"), 0);
                        break;
                    }
                    else
                    {
                        printf("Amount paid is more. Returning: %d\n", userfine - fine);
                    }
                    send(sock, "Fine paid", strlen("Fine paid"), 0);
                }
            }
            else
            {
                printf(" No fine to pay\n");
            }
            break;
        case 99:
            send(sock, "logout", strlen("logout"), 0);
            break;

        case 100:
            BorrowerMenuPrinter();
            continue;
        default:
            printf("Invalid choice. Try again\n");
        }

        ResponseHandler(sock,choice);
        
    }
}
void LibrarianMenu(int sock)
{
    send(sock, "librarian", strlen("librarian"), 0); // send role to server
    LibraryMenuPrinter();
    int choice=0;
    while (choice != 99)
    {
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            send(sock, "add", strlen("add"), 0);

            char title[100],author[100],ISBN[100],genre[100];
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
            printf("sending...\n");
            send(sock, arr[0], strlen(arr[0]), 0);
            usleep(100000);
            send(sock, arr[1], strlen(arr[1]), 0);
            break;
        case 3:
            send(sock, "viewBooks", strlen("viewBooks"), 0);
            break;
        case 4:
            send(sock, "viewBorrowers", strlen("viewBorrowers"), 0);
            break;
        case 5:
            send(sock, "addBorrower", strlen("addBorrower"), 0);
            printf("Enter the username of the borrower: ");
            char username[100],name[100],password[100];
            scanf("%s", username);
            printf("Enter the name of the borrower: ");
            scanf("%s", name);
            printf("Enter the password of the borrower: ");
            scanf("%s", password);
            printf("Enter the contact of the borrower: ");
            long long int contact;
            scanf("%lld", &contact);
            // now make a borrower struct and send it to server
            struct BorrowerPacket borrowerpacket;
            // add data to bookpacket
            strcpy(borrowerpacket.username, username);
            strcpy(borrowerpacket.name, name);
            strcpy(borrowerpacket.password, password);
            borrowerpacket.contact = contact;
            //server assigns ID and sets borrowed books array to null
            borrowerpacket.numBorrowedBooks = 0;
            borrowerpacket.fine = 0;
            borrowerpacket.isLate = 0;
            // now print all the fields of borrower packet
            printf("Username: %s\n", borrowerpacket.username);
            printf("Name: %s\n", borrowerpacket.name);
            printf("Password: %s\n", borrowerpacket.password);
            printf("Contact: %lld\n", borrowerpacket.contact);
            printf("ID: Is being assigned by server. Check later\n");
            printf("Num Borrowed Books: %d\n", borrowerpacket.numBorrowedBooks);
            printf("Fine: %d\n", borrowerpacket.fine);
            printf("Is Late: %d\n", borrowerpacket.isLate);
            // send the bookpacket to server
            send(sock, &borrowerpacket, sizeof(borrowerpacket), 0);

            break;
        case 6:
            send(sock, "removeBorrower", strlen("removeBorrower"), 0);
            // take the username of the borrower to remove
            printf("Enter the username of the borrower you want to remove: ");
            // char usernameToRemove[100];
            // scanf("%s", usernameToRemove);
            // directly read into buffr
            scanf("%s", message);
            // send the username to server
            send(sock, message, strlen(message), 0);
            memset(message, 0, sizeof(message));
            break;
        case 99:
            send(sock, "logout", strlen("logout"), 0);
            break;
        case 100:
            LibraryMenuPrinter();
            continue;
        default:
            printf("Invalid choice\n");
        }

        ResponseHandler(sock,choice);
    }
}
// ------------->>>>> Finished adding by Abhinav  <<<<<<<<<<<<------------
