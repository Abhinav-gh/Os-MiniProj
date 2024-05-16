#include "../../header/Functionalities/funcHandlerMain.h"

void librarianFunc(int new_socket){
    printf("here in src/Functionalities/LibraryFunc.c \n");

    // Get the functionality requested by the borrower
    char requestedFunc[BUFFER_SIZE] = {0};
    char buffer[BUFFER_SIZE] = {0};
    int valread = read(new_socket, buffer, BUFFER_SIZE);
    buffer[valread] = '\0';
    strncpy(requestedFunc, buffer, BUFFER_SIZE - 1);

//     // Trim the input string
// for (int i = 0; i < strlen(requestedFunc); i++) {
//     if (isspace(requestedFunc[i])) {
//         requestedFunc[i] = '\0';
//         break;
//     }
// }

    printf("Requested Functionality: %s\n", requestedFunc);

    if(strcmp(requestedFunc, "add") == 0){
        // implement add book now
        send(new_socket, "Book added successfully", strlen("Book added successfully"), 0);
    }
    else if(strcmp(requestedFunc, "remove") == 0){
        // remove book
        send(new_socket, "Book removed successfully", strlen("Book removed successfully"), 0);
    }
    else if(strcmp(requestedFunc, "viewBooks") == 0){
        // view all books
        send(new_socket, "Books viewed", strlen("Books viewed"), 0);
    }
    else if(strcmp(requestedFunc, "viewBorrowers") == 0){
        // view all books
        send(new_socket, "Borrowers viewed", strlen("Borrowers viewed"), 0);
    }
    else if(strcmp(requestedFunc, "logout") == 0){
        // logout
        send(new_socket, "Logout success", strlen("Logout success"), 0);
    }
    else{
        // printf("Invalid functionality\n");
        send(new_socket, "Invalid functionality", strlen("Invalid functionality"), 0);
    }
}