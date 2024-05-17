#include "../header/borrower.h"
#include "../header/book.h"
#include "../header/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>




//funtion to create a new borrower
struct Borrower* createBorrower(const char* username, const char* name, const char* password, long long int contact, int ID) {
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


//function to insert a borrower into the BST
void insertBorrower(struct BSTNodeBorrower** root, struct Borrower borrower) {
    if (*root == NULL) {
        *root = createBSTNodeBorrower(&borrower);
        return;
    }

    if (borrower.ID < (*root)->data.ID) {
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


// Packet Handler
void borrowerPacketHandler(int new_socket, MsgPacket *packet)
{
    struct BSTNodeBorrower *rootborrower = NULL;
    ReadDatabaseBorrower(&rootborrower, "../database/users/borrower.txt");

    struct BSTNodeBook *rootbook = NULL;
    ReadDatabaseBook(&rootbook, "../database/Books/books.txt");

    if(packet->choice == 1)
    {
        ReadAllGenres(new_socket ,rootbook, packet);
    }

}