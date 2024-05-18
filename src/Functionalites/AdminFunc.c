#include "../../header/Functionalities/funcHandlerMain.h"

void adminFunc(int new_socket)
{
    printf("Control passed to src/Functionalities/AdminFunc.c \n");

    // Get the functionality requested by the borrower
    char requestedFunc[BUFFER_SIZE] = {0};
    while (strcmp(requestedFunc, "logout") != 0)
    {
        int valread = read(new_socket, requestedFunc, BUFFER_SIZE);
        requestedFunc[valread] = '\0';

        printf("Requested Functionality: %s\n", requestedFunc);

        if (strcmp(requestedFunc, "addLibrarian") == 0)
        {
            // add book
            send(new_socket, "Librarian added", strlen("Librarian added"), 0);
        }
        else if (strcmp(requestedFunc, "removeLibrarian") == 0)
        {
            // remove book
            send(new_socket, "Librarian removed", strlen("Librarian removed"), 0);
        }
        else if (strcmp(requestedFunc, "viewLibrarians") == 0)
        {
            // view all books
            send(new_socket, "Librarians viewed", strlen("Librarians viewed"), 0);
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
        memset(requestedFunc, 0, BUFFER_SIZE);
    }
}