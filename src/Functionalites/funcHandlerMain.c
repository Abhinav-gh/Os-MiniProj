#include "../../header/Functionalities/funcHandlerMain.h"
#include "../books.c"
#include "./BorrowFunc.c"
#include "./AdminFunc.c"
#include "./LibraryFunc.c"


void funcHandler(int new_socket){
    initializeBooks();
    char role[BUFFER_SIZE] = {0};
    char buffer[BUFFER_SIZE] = {0};
    // read the role coming from the client
    int valread = read(new_socket, buffer, BUFFER_SIZE);
    buffer[valread] = '\0';
    strncpy(role, buffer, BUFFER_SIZE - 1);

    //empty the buffer
    memset(buffer, 0, BUFFER_SIZE);

    // check the role and call the respective functionality
    if (strcmp(role, "admin") == 0)
    {
        adminFunc(new_socket);
    }
    else if (strcmp(role, "librarian") == 0)
    {
        librarianFunc(new_socket,root);
    }
    else if (strcmp(role, "borrower") == 0)
    {
        borrowerFunc(new_socket,root);
    }
    else
    {
        perror("Role not recognized by server\n");
    }
}