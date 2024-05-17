#include "../../header/Functionalities/funcHandlerMain.h"

void librarianFunc(int new_socket, struct BSTNodeBook *root)
{
    printf("here in src/Functionalities/LibraryFunc.c \n");

    // Get the functionality requested by the borrower
    char requestedFunc[BUFFER_SIZE] = {0};
    while (strcmp(requestedFunc, "logout") != 0)
    {
        char buffer[BUFFER_SIZE] = {0};
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        buffer[valread] = '\0';
        strncpy(requestedFunc, buffer, BUFFER_SIZE - 1);

        printf("Requested Functionality: %s\n", requestedFunc);
        if (strcmp(requestedFunc, "add") == 0)
        {
            char genre[BUFFER_SIZE] = {0};
            valread = read(new_socket, buffer, BUFFER_SIZE);
            buffer[valread] = '\0';
            strncpy(genre, buffer, BUFFER_SIZE - 1);
            printf("Genre: %s\n", genre);
            // implement add book now
            // recieve the library book struct from the client
            // struct LibraryBook *newBook = (struct LibraryBook *)malloc(sizeof(struct LibraryBook));
            // the client has done this :send(sock, &book, sizeof(book), 0);
            // the librarybook struct is like this:
            //             struct LibraryBook {
            //     char title[MAX_TITLE_LENGTH];
            //     char author[MAX_AUTHOR_LENGTH];
            //     char ISBN[MAX_ISBN_LENGTH];
            //     int numCopies;
            //     int isAvailable;
            //     int yearPublished;
            //     time_t issueDate;
            //     time_t returnDate;
            //     struct Borrower borrower;
            // };
            // so we have to recieve the struct from the client

            struct LibraryBookPacket bookpacket;
            struct LibraryBook newBook;
            int valread = recv(new_socket, &bookpacket, sizeof(bookpacket), 0);

            // get bookpacket data into new
            strcpy(newBook.title, bookpacket.title);
            strcpy(newBook.author, bookpacket.author);
            strcpy(newBook.ISBN, bookpacket.ISBN);
            newBook.numCopies = bookpacket.numCopies;
            newBook.isAvailable = bookpacket.isAvailable;
            newBook.yearPublished = bookpacket.yearPublished;
            // set issue date here using time.h
            time_t t = time(NULL);
            newBook.issueDate = t;
            newBook.returnDate = 0;
            if (valread > 0)
            {
                addBook(&root, genre, &newBook);
            writeBSTToFile(root, "../database/Books/books.txt");
            send(new_socket, "Book added successfully", strlen("Book added successfully"), 0);
            }
            else
            {
                printf("Failed to receive data\n");
                send(new_socket, "Failed to add book", strlen("Failed to add book"), 0);
            }
            
        }
        else if (strcmp(requestedFunc, "remove") == 0)
        {
            // remove book
            send(new_socket, "Book removed successfully", strlen("Book removed successfully"), 0);
        }
        else if (strcmp(requestedFunc, "viewBooks") == 0)
        {
            // view all books
            char *booksInfo = getAllBooksInfoWrapper(root);
            send(new_socket, booksInfo, strlen(booksInfo), 0);
            free(booksInfo);
        }
        else if (strcmp(requestedFunc, "viewBorrowers") == 0)
        {
            // view all books
            send(new_socket, "Borrowers viewed", strlen("Borrowers viewed"), 0);
        }
        else if (strcmp(requestedFunc, "logout") == 0)
        {
            // logout
            send(new_socket, "Logout success", strlen("Logout success"), 0);
        }
        else
        {
            // printf("Invalid functionality\n");
            send(new_socket, "Invalid functionality", strlen("Invalid functionality"), 0);
        }
        // reset the buffer
        memset(buffer, 0, BUFFER_SIZE);
        memset(requestedFunc, 0, BUFFER_SIZE);
    }
}