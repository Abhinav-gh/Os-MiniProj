#include "../../header/Functionalities/funcHandlerMain.h"

void adminFunc(int new_socket)
{
    printf("here in src/Functionalities/AdminFunc.c \n");

    // Get the functionality requested by the borrower
    char requestedFunc[BUFFER_SIZE] = {0};
    while (strcmp(requestedFunc, "logout") != 0)
    {
        char buffer[BUFFER_SIZE] = {0};
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        buffer[valread] = '\0';
        strncpy(requestedFunc, buffer, BUFFER_SIZE - 1);

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
        memset(buffer, 0, BUFFER_SIZE);
        memset(requestedFunc, 0, BUFFER_SIZE);
    }
}