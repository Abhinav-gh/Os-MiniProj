#include "../../header/Functionalities/funcHandlerMain.h"
#include <pthread.h>

// Declare a global read-write lock
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
void initializeLocks() {
    if (pthread_rwlock_init(&rwlock, NULL) != 0) {
        perror("Failed to initialize read-write lock");
        exit(EXIT_FAILURE);
    }
}

#include "../DataStructures/books.c"
#include "../DataStructures/borrower.c"
#include "./BorrowFunc.c"
#include "./AdminFunc.c"
#include "./LibraryFunc.c"


void funcHandler(int new_socket){
    initializeBooks();
    initializeBorrower();
    initializeLocks();
    printf("Pthreads Read-write locks initialized and can now be used\n");
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
        librarianFunc(new_socket,root,borrowerRoot);
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