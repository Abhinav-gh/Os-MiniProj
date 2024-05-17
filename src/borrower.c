#include "../header/borrower.h"
#include "../header/book.h"
#include "../header/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>



int eligible =0;
int res = 0;


//funtion to create a new borrower
struct Borrower* createBorrower(char* username, const char* name, const char* password, long long int contact, int ID) {
    struct Borrower* borrower = (struct Borrower*)malloc(sizeof(struct Borrower));
    if (borrower == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    strcpy(borrower->username, username);
    strcpy(borrower->name, name);
    strcpy(borrower->password, password);
    borrower->contact = contact;
    borrower->ID = ID;
    borrower->numBorrowedBooks = 0;
    borrower->fine = 0;
    borrower->isLate = 0;
    borrower->LoginStatus = 0;

    return borrower;
}



//function to create a new BST node
struct BSTNodeBorrower* createBSTNodeBorrower(struct Borrower* borrower) {
    struct BSTNodeBorrower* newNode = (struct BSTNodeBorrower*)malloc(sizeof(struct BSTNodeBorrower));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    newNode->data = *borrower;
    newNode->left = newNode->right = NULL;

    return newNode;
}




//function to insert a borrower into the BST on the basis of username
void insertBorrower(struct BSTNodeBorrower** root, struct Borrower borrower) {
    if (*root == NULL) {
        *root = createBSTNodeBorrower(&borrower);
        return;
    }

    if (strcmp(borrower.username, (*root)->data.username) < 0) {
        insertBorrower(&((*root)->left), borrower);
    } else {
        insertBorrower(&((*root)->right), borrower);
    }
}

// funtion to read the database from a file
void ReadDatabaseBorrower(struct BSTNodeBorrower  **root, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    struct Borrower borrower;

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        sscanf(buffer, "%d %s %s %s %lld %s %s %s %d %d %d %d",&borrower.ID ,borrower.username, borrower.name, borrower.password, &borrower.contact,borrower.borrowedBooks[0],borrower.borrowedBooks[1],borrower.borrowedBooks[2] ,&borrower.numBorrowedBooks, &borrower.fine, &borrower.isLate, &borrower.LoginStatus);
        insertBorrower(root, borrower);
    }
    fclose(file);
}

// Helper function to write the BST to a file
void writeBSTToFileHelperBorrower(struct BSTNodeBorrower *root, FILE *file) {
    if (root == NULL) {
        return;
    }

    writeBSTToFileHelperBorrower(root->left, file);
    fprintf(file, "%d %s %s %s %lld %s %s %s %d %d %d %d\n", root->data.ID, root->data.username, root->data.name, root->data.password, root->data.contact, root->data.borrowedBooks[0], root->data.borrowedBooks[1], root->data.borrowedBooks[2], root->data.numBorrowedBooks, root->data.fine, root->data.isLate, root->data.LoginStatus);
    writeBSTToFileHelperBorrower(root->right, file);
}


// funtion to write the database to a file
void WriteDatabaseBorrower(struct BSTNodeBorrower *root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    writeBSTToFileHelperBorrower(root, file);

    fclose(file);
}

//funtion to set the login status of a borrower
void setLoginStatus(struct BSTNodeBorrower *root, char *username, int status) {
    if (root == NULL) {
        return;
    }

    if (strcmp(root->data.username, username) == 0) {
        root->data.LoginStatus = status;
        WriteDatabaseBorrower(root, "../database/users/borrower.txt");
        return;
    }

    if (strcmp(username, root->data.username) < 0) {
        setLoginStatus(root->left, username, status);
    } else {
        setLoginStatus(root->right, username, status);
    }


}


void showBorrowedBooks(int socket, struct BSTNodeBorrower *root, MsgPacket *packet) {

    if (root == NULL) {
        const char *eot = "END_OF_TRANSMISSION";
        send(socket, eot, strlen(eot) + 1, 0);
        return;
    }

    if (strcmp(root->data.username, packet->username) == 0) {
        char buffer[BUFFER_SIZE];
        strcpy(buffer, "Borrowed Books:\n");
        for (int i = 0; i < root->data.numBorrowedBooks; i++) {
            strcat(buffer, root->data.borrowedBooks[i]);
            strcat(buffer, "\n");
        }
        send(socket, buffer, strlen(buffer), 0);
        usleep(10000);
        const char *eot = "END_OF_TRANSMISSION";
        send(socket, eot, strlen(eot) + 1, 0);
        return;
    }

    if (strcmp(packet->username, root->data.username) < 0) {
        showBorrowedBooks(socket, root->left, packet);
    } else {
        showBorrowedBooks(socket, root->right, packet);
    }

    if(root->left == NULL && root->right == NULL)
    {
        const char *eot = "END_OF_TRANSMISSION";
        send(socket, eot, strlen(eot) + 1, 0);
    }
}


void showMyInfo(int socket, struct BSTNodeBorrower *root, MsgPacket *packet) {
    if (root == NULL) {
        const char *eot = "END_OF_TRANSMISSION";
        send(socket, eot, strlen(eot) + 1, 0);
        return;
    }

    if (strcmp(root->data.username, packet->username) == 0) {
        char buffer[BUFFER_SIZE];
        sprintf(buffer, "Name: %s\nContact: %lld\nFine: %d Rs\nLate: %d\nNumber of Books borrwed: %d\n ", root->data.name, root->data.contact, root->data.fine, root->data.isLate, root->data.numBorrowedBooks);
        send(socket, buffer, strlen(buffer), 0);
        usleep(10000);
        send(socket, "END_OF_TRANSMISSION", strlen("END_OF_TRANSMISSION") + 1, 0);
    }
}

void changePassword(int socket, struct BSTNodeBorrower *root, MsgPacket *packet) {
    if (root == NULL) {
        const char *eot = "END_OF_TRANSMISSION";
        send(socket, eot, strlen(eot) + 1, 0);
        return;
    }

    if (strcmp(root->data.username, packet->username) == 0) {
        strcpy(root->data.password, packet->payload[0]);
        WriteDatabaseBorrower(root, "../database/users/borrower.txt");
        send(socket, "Password changed successfully", strlen("Password changed successfully") + 1, 0);
        usleep(10000);
        send(socket, "END_OF_TRANSMISSION", strlen("END_OF_TRANSMISSION") + 1, 0);
        return;
    }

    if (strcmp(packet->username, root->data.username) < 0) {
        changePassword(socket, root->left, packet);
    } else {
        changePassword(socket, root->right, packet);
    }
}

void updateContact(int socket, struct BSTNodeBorrower *root, MsgPacket *packet) {
    if (root == NULL) {
        const char *eot = "END_OF_TRANSMISSION";
        send(socket, eot, strlen(eot) + 1, 0);
        return;
    }

    if (strcmp(root->data.username, packet->username) == 0) {
        root->data.contact = atoll(packet->payload[0]);
        WriteDatabaseBorrower(root, "../database/users/borrower.txt");
        send(socket, "Contact updated successfully", strlen("Contact updated successfully") + 1, 0);
        usleep(10000);
        send(socket, "END_OF_TRANSMISSION", strlen("END_OF_TRANSMISSION") + 1, 0);
        return;
    }

    if (strcmp(packet->username, root->data.username) < 0) {
        updateContact(socket, root->left, packet);
    } else {
        updateContact(socket, root->right, packet);
    }
}   

void checkDueDate(int socket, struct BSTNodeBorrower *root, MsgPacket *packet) {
    if (root == NULL) {
        const char *eot = "END_OF_TRANSMISSION";
        send(socket, eot, strlen(eot) + 1, 0);
        return;
    }

    if (strcmp(root->data.username, packet->username) == 0) {
        char buffer[BUFFER_SIZE];
        if (root->data.isLate) {
            strcpy(buffer, "You have a fine of Rs. ");
            char fine[10];
            sprintf(fine, "%d", root->data.fine);
            strcat(buffer, fine);
            strcat(buffer, "\n");
        } else {
            strcpy(buffer, "You have no fine\n");
        }
        send(socket, buffer, strlen(buffer), 0);
        usleep(10000);
        send(socket, "END_OF_TRANSMISSION", strlen("END_OF_TRANSMISSION") + 1, 0);
        return;
    }

    if (strcmp(packet->username, root->data.username) < 0) {
        checkDueDate(socket, root->left, packet);
    } else {
        checkDueDate(socket, root->right, packet);
    }
}



void logout(int socket) {
    const char *msg = "You have been logged out successfully";
    send(socket, msg, strlen(msg)+1, 0); // Removed +1 from strlen(msg)
    usleep(10000);
    const char *eot = "END_OF_TRANSMISSION";
    send(socket, eot, strlen(eot)+1, 0); // Removed +1 from strlen(eot)
    close(socket); // Close the socket after sending logout message
}


void borrowBookUserUpdate(struct BSTNodeBorrower *root, char *username, const char *ISBN, struct BSTNodeBook *rootbook) {
    if (root == NULL) {
        return;
    }

    char bookName[MAX_TITLE_LENGTH];
    FetchBookNameFromISBN(rootbook, ISBN, bookName);

    if (strcmp(root->data.username, username) == 0) {
        for (int i = 0; i < 3; i++) {
            if (strcmp(root->data.borrowedBooks[i], "NULL") == 0) {
                strcpy(root->data.borrowedBooks[i], bookName);
                root->data.numBorrowedBooks++;
                break;
            }
        }
        return;
    }

    if (strcmp(username, root->data.username) < 0) {
        borrowBookUserUpdate(root->left, username, ISBN, rootbook);
    } else {
        borrowBookUserUpdate(root->right, username, ISBN, rootbook);
    }
}      


int isEligibleToBorrow(struct BSTNodeBorrower *root, char *username) {
    if (root == NULL) {
        return 0;
    }

    if (strcmp(root->data.username, username) == 0) {
        if (root->data.numBorrowedBooks < 3) {
            return 1;
        } else {
            return 0;
        }
    }

    if (strcmp(username, root->data.username) < 0) {
        return isEligibleToBorrow(root->left, username);
    } else {
        return isEligibleToBorrow(root->right, username);
    }
}


// Send Due dates along with the book title of all books borrowed by the user
void sendDueDates(int socket, struct BSTNodeBorrower *root, MsgPacket *packet, struct BSTNodeBook *rootbook) {
    if (root == NULL) {
        const char *eot = "END_OF_TRANSMISSION";
        send(socket, eot, strlen(eot) + 1, 0);
        return;
    }

    if (strcmp(root->data.username, packet->username) == 0) {
        char buffer[BUFFER_SIZE];
        for (int i = 0; i < 3; i++) {
            if (strcmp(root->data.borrowedBooks[i], "NULL") != 0) {
                int remainingTime = CheckRemainingTimeForBookReturn(rootbook, root->data.borrowedBooks[i]);
                sprintf(buffer, "%s: %d days remaining\n", root->data.borrowedBooks[i], remainingTime);
                send(socket, buffer, strlen(buffer), 0);
                usleep(10000);
            }
        }
        send(socket, "END_OF_TRANSMISSION", strlen("END_OF_TRANSMISSION") + 1, 0);
        return;
    }

    if (strcmp(packet->username, root->data.username) < 0) {
        sendDueDates(socket, root->left, packet, rootbook);
    } else {
        sendDueDates(socket, root->right, packet, rootbook);
    }
}


// Packet Handler
void borrowerPacketHandler(int new_socket, MsgPacket *packet)
{
    struct BSTNodeBorrower *rootborrower = NULL;
    ReadDatabaseBorrower(&rootborrower, "../database/users/borrower.txt");

    struct BSTNodeBook *rootbook = NULL;
    ReadDatabaseBook(&rootbook, "../database/Books/books.txt");


    setLoginStatus(rootborrower, packet->username, 1);

    switch (packet->choice)
    {
    case 1:
        ReadAllGenres(new_socket,rootbook ,packet);
        break;
    case 2:
        ReadAllBooks(new_socket,rootbook ,packet);
        break;

    case 3:
        eligible = isEligibleToBorrow(rootborrower, packet->username);
        if(eligible == 0)
        {
            send(new_socket, "You have already borrowed 3 books", strlen("You have already borrowed 3 books") + 1, 0);
            usleep(10000);
            send(new_socket, "END_OF_TRANSMISSION", strlen("END_OF_TRANSMISSION") + 1, 0);
            break;
        }

        res = borrowBook(new_socket,rootbook ,packet->payload[0], packet->username);
        if(res == 1)
        {
            writeBSTToFileBook(rootbook, "../database/books/books.txt");
            borrowBookUserUpdate(rootborrower, packet->username, packet->payload[0],rootbook);
            WriteDatabaseBorrower(rootborrower, "../database/users/borrower.txt");
        }

        break;

    case 4:
        //return book
        break;
    
    //show borrowed books
    case 5:
        showBorrowedBooks(new_socket,rootborrower ,packet);
        break;

    case 6:
        showMyInfo(new_socket,rootborrower ,packet);
        break;

    case 7:
        changePassword(new_socket,rootborrower ,packet);
        break;

    case 8:
        updateContact(new_socket,rootborrower ,packet);
        break;

    case 9:
        sendDueDates(new_socket,rootborrower ,packet,rootbook);
        break;
    
    case 10:
        setLoginStatus(rootborrower, packet->username, 0);
        logout(new_socket);
        break;

    default:
        break;

    }
}



