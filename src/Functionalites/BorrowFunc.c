#include "../../header/Functionalities/funcHandlerMain.h"

void borrowerFunc(int new_socket, struct Node *root)
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
            send(new_socket, "Book borrowed successfully", strlen("Book borrowed successfully"), 0);
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
        else if (strcmp(requestedFunc, "borrowed") == 0)
        {
            // view all borrowed books
            send(new_socket, "Borrowed books viewed", strlen("Borrowed books viewed"), 0);
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