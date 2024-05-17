#include "../../header/Functionalities/funcHandlerMain.h"

void borrowerFunc(int new_socket, struct BSTNodeBook *root)
{
    printf("here in src/Functionalities/BorrowFunc.c \n");

    // Get the functionality requested by the borrower
    char requestedFunc[BUFFER_SIZE] = {0};
    while (strcmp(requestedFunc, "logout") != 0)
    {
        char buffer[BUFFER_SIZE] = {0};
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        buffer[valread] = '\0';
        strncpy(requestedFunc, buffer, BUFFER_SIZE - 1);

        printf("Requested Functionality: %s\n", requestedFunc);
        if (strcmp(requestedFunc, "borrow") == 0)
        {
            // get the isbn and genere of the book from client. Also get the username from the client
            char isbn[BUFFER_SIZE] = {0};
            char genre[BUFFER_SIZE] = {0};
            char username[BUFFER_SIZE] = {0};
            int valread = read(new_socket, buffer, BUFFER_SIZE);
            buffer[valread] = '\0';
            strncpy(isbn, buffer, BUFFER_SIZE - 1);
            // printf("ISBN: %s\n", isbn);
            valread = read(new_socket, buffer, BUFFER_SIZE);
            buffer[valread] = '\0';
            strncpy(genre, buffer, BUFFER_SIZE - 1);
            // printf("Genre: %s\n", genre);
            valread = read(new_socket, buffer, BUFFER_SIZE);
            buffer[valread] = '\0';
            strncpy(username, buffer, BUFFER_SIZE - 1);
            // printf("Username: %s\n", username);
            // now we have to use this function to borrow the book struct LibraryBook *borrowBook(struct BSTNodeBook *root, const char *genreName, const char *ISBN, const char *username)
            // serach for the borrower and make the borrwer object
            struct Borrower *borrower = getBorrowerData(borrowerRoot, username);
            // use this borrower now
            //  void borrowBook(struct BSTNodeBook *root, const char *genreName, const char *ISBN, struct Borrower *borrower)
            if (borrowBook(root, genre, isbn, borrower) == 0)
            {
                // update the file
                writeBSTToFile(root, "../database/Books/books.txt");
                writeBSTToFileBorrower(borrowerRoot, "../database/users/borrower.txt");
                send(new_socket, "Book borrowed successfully", strlen("Book borrowed successfully"), 0);
            }
            else
            {
                send(new_socket, "Maximum limit(3) reached", strlen("Maximum limit(3) reached"), 0);
            }
        }
        else if (strcmp(requestedFunc, "return") == 0)
        {
            printf("here\n");
            send(new_socket, "Book returned successfully", strlen("Book returned successfully"), 0);
        }
        else if (strcmp(requestedFunc, "view") == 0)
        {
            // view all books
            char *booksInfo = getAllBooksInfoWrapper(root);
            send(new_socket, booksInfo, strlen(booksInfo), 0);
            free(booksInfo);
        }
        else if (strcmp(requestedFunc, "search") == 0)
        {
            // view all borrowed books
            char isbn[BUFFER_SIZE] = {0};
            char genre[BUFFER_SIZE] = {0};
            int valread = read(new_socket, buffer, BUFFER_SIZE);
            buffer[valread] = '\0';
            strncpy(isbn, buffer, BUFFER_SIZE - 1);
            printf("ISBN: %s\n", isbn);

            valread = read(new_socket, buffer, BUFFER_SIZE);
            buffer[valread] = '\0';
            strncpy(genre, buffer, BUFFER_SIZE - 1);
            printf("Genre: %s\n", genre);
            // now we have to use this function to serach book struct LibraryBook *searchBook(struct BSTNodeBook *root, const char *genreName, const char *ISBN)
            struct LibraryBook *searchedBook = searchBook(root, genre, isbn);
            if (searchedBook == NULL)
            {
                send(new_socket, "Book not found", strlen("Book not found"), 0);
            }
            else
            {
                // printf("book found\n");
                char *bookInfo = (char *)malloc(2 * BUFFER_SIZE * sizeof(char));
                getBookInfo(searchedBook, bookInfo);
                // printf("Book Info: %s\n", bookInfo);
                send(new_socket, bookInfo, strlen(bookInfo), 0);
            }
            // send(new_socket, "searched", strlen("searched"), 0);
        }
        else if (strcmp(requestedFunc, "logout") == 0)
        {
            // logout
            send(new_socket, "Logout success", strlen("Logout success"), 0);
        }
        else
        {
            // printf("Invalid functionality\n");
            // printf("Invalid functionality\n");
            send(new_socket, "Invalid functionality", strlen("Invalid functionality"), 0);
        }
        memset(buffer, 0, BUFFER_SIZE);
        memset(requestedFunc, 0, BUFFER_SIZE);
    }
}