#include "../../header/Functionalities/funcHandlerMain.h"

void borrowerFunc(int new_socket, struct BSTNodeBook *root)
{
    printf("here in src/Functionalities/BorrowFunc.c \n");

    // Get the functionality requested by the borrower
    char requestedFunc[BUFFER_SIZE] = {0};
    while (strcmp(requestedFunc, "logout") != 0)
    {
        char buffer[BUFFER_SIZE] = {0};
        int valread = read(new_socket, requestedFunc, BUFFER_SIZE);
        requestedFunc[valread] = '\0';

        printf("Requested Functionality: %s\n", requestedFunc);
        if (strcmp(requestedFunc, "borrow") == 0)
        {
            // get the isbn and genere of the book from client. Also get the username from the client
            char isbn[BUFFER_SIZE] = {0},genre[BUFFER_SIZE] = {0},username[BUFFER_SIZE] = {0};
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
            memset(buffer, 0, BUFFER_SIZE);
            // now we have to use this function to borrow the book struct LibraryBook *borrowBook(struct BSTNodeBook *root, const char *genreName, const char *ISBN, const char *username)
            // serach for the borrower and make the borrwer object
            struct Borrower *borrower = getBorrowerData(borrowerRoot, username);
            // use this borrower now

            // check if the borrower has fine to pay. If yes then return
            if (borrower->fine > 0)
            {
                send(new_socket, "You have fine to pay. Please pay the fine first", strlen("You have fine to pay. Please pay the fine first"), 0);
            }
            else
            {
                int result = borrowBook(root, genre, isbn, borrower);

                if (result == 0)
                {
                    // update the file
                    writeBSTToFile(root, "../database/Books/books.txt");
                    writeBSTToFileBorrower(borrowerRoot, "../database/users/borrower.txt");
                    send(new_socket, "Book borrowed successfully", strlen("Book borrowed successfully"), 0);
                }
                else if (result == -1)
                {
                    send(new_socket, "Book not found", strlen("Book not found"), 0);
                }
                else if (result == -2)
                {
                    send(new_socket, "No copies available", strlen("No copies available"), 0);
                }
                else if (result == -3)
                {
                    send(new_socket, "Book not available", strlen("Book not available"), 0);
                }
                else if (result == 1)
                {
                    send(new_socket, "Maximum limit(3) reached", strlen("Maximum limit(3) reached"), 0);
                }
            }
        }
        else if (strcmp(requestedFunc, "return") == 0)
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
            memset(buffer, 0, BUFFER_SIZE);
            // printf("Username: %s\n", username);
            // now we have to use this function to return the book struct LibraryBook *returnBook(struct BSTNodeBook *root, const char *genreName, const char *ISBN, const char *username)
            // serach for the borrower and make the borrwer object
            struct Borrower *borrower = getBorrowerData(borrowerRoot, username);
            // use this borrower now
            //  void returnBook(struct BSTNodeBook *root, const char *genreName, const char *ISBN, struct Borrower *borrower)
            int isreturned = returnBook(root, genre, isbn, borrower);
            if (isreturned == 1)
            {
                send(new_socket, "Book not found", strlen("Book not found"), 0);
                return;
            }
            else if (isreturned == 2)
            {
                send(new_socket, "Book not borrowed by this user", strlen("Book not borrowed by this user"), 0);
                return;
            }
            else
            {
                // update the file
                writeBSTToFile(root, "../database/Books/books.txt");
                writeBSTToFileBorrower(borrowerRoot, "../database/users/borrower.txt");
                send(new_socket, "Book returned successfully", strlen("Book returned successfully"), 0);
            }
        }
        else if (strcmp(requestedFunc, "view") == 0)
        {
            // view all books
            char *booksInfo = getAllBooksInfoWrapper(root);
            send(new_socket, booksInfo, strlen(booksInfo), 0);
            usleep(100000);
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
        else if (strcmp(requestedFunc, "payFine") == 0)
        {
            int isfineCalculated = 0;
            read(new_socket, &isfineCalculated, sizeof(int));
            if (isfineCalculated == 0)
            {
                send(new_socket, "Fine not calculated. First go to return", strlen("Fine not calculated. First go to return"), 0);
            }
            else
            {
                // printf("Fine calculated\n");
                send(new_socket, "Fine calculated", strlen("Fine calculated"), 0);
                // recieve the username
                // clean the socket first
                memset(buffer, 0, BUFFER_SIZE);

                char username[BUFFER_SIZE] = {0};
                int valread = read(new_socket, buffer, BUFFER_SIZE);
                buffer[valread] = '\0';
                strncpy(username, buffer, BUFFER_SIZE - 1);
                // printf("Username: %s\n", username);

                // search for the borrower
                struct Borrower *borrower = getBorrowerData(borrowerRoot, username);
                int fine = borrower->fine;
                printf("Fine is %d\n", fine);
                if (fine == 0)
                {
                    int fine_zero = 0;
                    send(new_socket, &fine_zero, sizeof(int), 0);
                }
                else
                {
                    send(new_socket, &fine, sizeof(int), 0);
                    // wait for confirmation string.
                    char confirmation[BUFFER_SIZE] = {0};
                    valread = read(new_socket, confirmation, BUFFER_SIZE);
                    confirmation[valread] = '\0';

                    // check if the fine is paid or not
                    if (strcmp(confirmation, "Fine paid") == 0)
                    {
                        resetFines(borrower);
                        writeBSTToFileBorrower(borrowerRoot, "../database/users/borrower.txt");
                        send(new_socket, "Fine paid successfully", strlen("Fine paid successfully"), 0);
                    }
                    else
                    {
                        send(new_socket, "Fine not paid by user", strlen("Fine not paid by user"), 0);
                    }
                }
            }
        }
        else if(strcmp(requestedFunc, "viewBorrowedBooks")==0){
            char username[BUFFER_SIZE] = {0};
            int valread = read(new_socket, username, BUFFER_SIZE);
            username[valread] = '\0';
            // printf("Username: %s\n", username);
            char * borrowedBooksInfo = getAllBorrowedBooks(borrowerRoot,username);
            // printf("Borrowed Books Info: %s\n", borrowedBooksInfo);
            if(borrowedBooksInfo==NULL){
                send(new_socket, "No borrowed books", strlen("No borrowed books"), 0);
                continue;
            }
            send(new_socket, borrowedBooksInfo, strlen(borrowedBooksInfo), 0);
            // usleep(100000);
            // send(new_socket, "Viewed")
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