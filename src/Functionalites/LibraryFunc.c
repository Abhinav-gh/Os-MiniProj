#include "../../header/Functionalities/funcHandlerMain.h"

void writeBSTToFileWrapper(void *args)
{
    WriteArgs *writeArgs = (WriteArgs *)args;
    struct BSTNodeBook *root = writeArgs->root;
    char *filepath = writeArgs->filepath;
    pthread_rwlock_wrlock(&rwlock);
    int result = writeBSTToFile(root, filepath);
    pthread_rwlock_wrlock(&rwlock);
    if (result == -1)
    {
        perror("Failed to write to file");
    }
    free(writeArgs); // Free the allocated memory for arguments
    return NULL;
}
void librarianFunc(int new_socket, struct BSTNodeBook *root, struct BSTNodeBorrower *borrowerRoot)
{
    printf("🔧 Control Interface switched to: src/Functionalities/LibraryFunc.c \n");

    // Get the functionality requested by the borrower
    char requestedFunc[BUFFER_SIZE] = {0};
    while (strcmp(requestedFunc, "logout") != 0)
    {
        char buffer[BUFFER_SIZE] = {0};
        int valread = read(new_socket, requestedFunc, BUFFER_SIZE);
        requestedFunc[valread] = '\0';

        printf("Requested Functionality: %s\n", requestedFunc);
        if (strcmp(requestedFunc, "add") == 0)
        {
            char genre[BUFFER_SIZE] = {0};
            valread = read(new_socket, buffer, BUFFER_SIZE);
            buffer[valread] = '\0';
            strncpy(genre, buffer, BUFFER_SIZE - 1);
            printf("Genre: %s\n", genre);
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

                // Allocate memory for arguments
                WriteArgs *writeArgs = malloc(sizeof(WriteArgs));
                writeArgs->root = root;
                strncpy(writeArgs->filepath, "../database/Books/books.txt", sizeof(writeArgs->filepath));

                // Create a thread to write to the file
                pthread_t tid;
                pthread_create(&tid, NULL, writeBSTToFileWrapper, (void *)writeArgs);

                // Wait for the thread to finish
                pthread_join(tid, NULL);

                // Send the response after the write operation is done
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
            char arr[2][BUFFER_SIZE] = {0};
            int valread = read(new_socket, arr[0], sizeof(arr));
            // handle error
            if (valread < 0)
            {
                printf("Failed to receive data\n");
                recv(new_socket, arr[1], sizeof(arr), 0); // to clear the client send
                send(new_socket, "Failed to remove book", strlen("Failed to remove book"), 0);
            }
            valread = read(new_socket, arr[1], sizeof(arr));
            arr[0][BUFFER_SIZE - 1] = '\0';
            arr[1][BUFFER_SIZE - 1] = '\0';
            // now remove the book
            removeBook(&root, arr[1], arr[0]);
            //THIS IS A DATABASE OPERATION
            // Acquire the lock
            pthread_rwlock_wrlock(&rwlock);
            writeBSTToFile(root, "../database/Books/books.txt");
            // Release the lock
            pthread_rwlock_unlock(&rwlock);
            // printf("Book removed\n");
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
            // view all borrowers
            // printf("Viewing all borrowers\n");
            char *borrowersInfo = getAllBorrowersInfoWrapper(borrowerRoot);
            // printf("Borrowers Info: %s\n", borrowersInfo);
            send(new_socket, borrowersInfo, strlen(borrowersInfo), 0);
            free(borrowersInfo);
        }

        else if (strcmp(requestedFunc, "addBorrower") == 0)
        {
            struct BorrowerPacket borrowerPacket;
            // recieve the borrower packet
            int valread = recv(new_socket, &borrowerPacket, sizeof(borrowerPacket), 0);
            // print the data recieved
            // printf("Borrower Packet: %s %s %s %lld %d %d %d %d\n", borrowerPacket.username, borrowerPacket.name, borrowerPacket.password, borrowerPacket.contact, borrowerPacket.ID, borrowerPacket.numBorrowedBooks, borrowerPacket.fine, borrowerPacket.isLate);
            struct Borrower newBorrower;
            // get bookpacket data into new
            strcpy(newBorrower.username, borrowerPacket.username);
            strcpy(newBorrower.name, borrowerPacket.name);
            strcpy(newBorrower.password, borrowerPacket.password);
            newBorrower.contact = borrowerPacket.contact;
            // Assign the id
            int max_id = getMaxID(borrowerRoot);
            newBorrower.ID = max_id + 1;

            newBorrower.numBorrowedBooks = borrowerPacket.numBorrowedBooks;
            newBorrower.fine = borrowerPacket.fine;
            newBorrower.isLate = borrowerPacket.isLate;
            // set the borrowed books to NULL
            for (int i = 0; i < 3; i++)
            {
                newBorrower.borrowedBooks[i] = NULL;
            }
            // add the borrower
            borrowerRoot = insertBorrower(borrowerRoot, &newBorrower);
            //THIS IS A DATABASE OPERATION
            
            // Acquire the lock
            pthread_rwlock_wrlock(&rwlock);
            writeBSTToFileBorrower(borrowerRoot, "../database/users/borrower.txt");
            // Release the lock
            pthread_rwlock_unlock(&rwlock);
            send(new_socket, "Borrower added successfully", strlen("Borrower added successfully"), 0);
        }
        else if (strcmp(requestedFunc, "removeBorrower") == 0)
        {
            // take the username of the borrower to be removed
            char username[BUFFER_SIZE] = {0};
            int valread = read(new_socket, username, BUFFER_SIZE);
            username[valread] = '\0';
            // printf("Username: %s\n", username);
            borrowerRoot = deleteBorrower(borrowerRoot, username);

            //THIS IS A DATABASE OPERATION
            // Acquire the lock
            pthread_rwlock_wrlock(&rwlock);
            writeBSTToFileBorrower(borrowerRoot, "../database/users/borrower.txt");
            // Release the lock
            pthread_rwlock_unlock(&rwlock);
            send(new_socket, "Borrower removed successfully", strlen("Borrower removed successfully"), 0);
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