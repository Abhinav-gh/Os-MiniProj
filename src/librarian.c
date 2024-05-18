#include "../header/librarian.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../header/borrower.h"
#include "../header/book.h"


int isAvailable = 0;


//function to create a new librarian
struct Librarian* createLibrarian(const char* username, const char* name, const char* email, const char* password) {
    struct Librarian* librarian = (struct Librarian*)malloc(sizeof(struct Librarian));
    if (librarian == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    strcpy(librarian->username, username);
    strcpy(librarian->name, name);
    strcpy(librarian->email, email);
    strcpy(librarian->password, password);
    librarian->LoginStatus = 0;

    return librarian;
}


//function to create a new BST node
struct BSTNodeLibrarian* createBSTNodeLibrarian(struct Librarian* librarian) {
    struct BSTNodeLibrarian* newNode = (struct BSTNodeLibrarian*)malloc(sizeof(struct BSTNodeLibrarian));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    newNode->data = *librarian;
    newNode->left = newNode->right = NULL;

    return newNode;
}


// Funtion to insert a librarian into the BST
void insertLibrarian(struct BSTNodeLibrarian** root, struct Librarian librarian) {
    if (*root == NULL) {
        *root = createBSTNodeLibrarian(&librarian);
        return;
    }

    if (strcmp(librarian.username, (*root)->data.username) < 0) {
        insertLibrarian(&((*root)->left), librarian);
    } else {
        insertLibrarian(&((*root)->right), librarian);
    }
}


// function to display all librarians in the BST
void displayLibrarians(struct BSTNodeLibrarian* root) {
    if (root == NULL) {
        return;
    }

    displayLibrarians(root->left);
    printf("Username: %s, Name: %s, Email: %s, LoginStatus: %d\n",
           root->data.username, root->data.name, root->data.email, root->data.LoginStatus);
    displayLibrarians(root->right);
}


// function to free the memory allocated for the BST
void freeLibrarianBST(struct BSTNodeLibrarian* root) {
    if (root == NULL) {
        return;
    }

    freeLibrarianBST(root->left);
    freeLibrarianBST(root->right);
    free(root);
}

// funtion to read the database from a file
void ReadDatabaseLibrarian(struct BSTNodeLibrarian  **root, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    struct Librarian librarian;

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        sscanf(buffer, "%s %s %s %s %d", librarian.username, librarian.name, librarian.email, librarian.password, &librarian.LoginStatus);
        insertLibrarian(root, librarian);
    }
    fclose(file);
}

// function to write the BST to a file
void WriteLibrarianHelper(struct BSTNodeLibrarian* root, FILE* file) {
    if (root == NULL) {
        return;
    }

    WriteLibrarianHelper(root->left, file);
    fprintf(file, "%s %s %s %s %d\n", root->data.username, root->data.name, root->data.email, root->data.password, root->data.LoginStatus);
    WriteLibrarianHelper(root->right, file);
}


void WriteLibrarian(struct BSTNodeLibrarian  **root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    WriteLibrarianHelper(*root, file);
    fclose(file);
}



void librarianPacketHandler(int new_socket, MsgPacket *packet) {

    struct BSTNodeBorrower *rootborrower = NULL;
    ReadDatabaseBorrower(&rootborrower, "../database/users/borrower.txt");

    struct BSTNodeBook *rootbook = NULL;
    ReadDatabaseBook(&rootbook, "../database/Books/books.txt");

    switch(packet->choice)
    {
        case 1:
            isAvailable = atoi(packet->payload[5]) > 0 ? 1 : 0;
            insertBook(&rootbook,packet->payload[3] ,createBook(new_socket,packet->payload[0], packet->payload[2], packet->payload[1],atoi(packet->payload[5]) , isAvailable , atoi(packet->payload[4]) ,0 , 0 , "NULL")); 
            writeBSTToFileBook(rootbook, "../database/Books/books.txt");
            break;

        case 2:
            //Delete book
            deleteBook(new_socket, &rootbook, packet->payload[0]);
            writeBSTToFileBook(rootbook, "../database/Books/books.txt");
            break;

        case 3:
            break;
            
        case 4:
            ReadAllBooks(new_socket, rootbook, packet);
            break;

        case 5:
            ReadAllGenres(new_socket, rootbook, packet);
            break;

        case 6:
            showAllBorrowers(new_socket, rootborrower);
            break;

        case 7:
            //show fines of a borrower
            break;




    }
}


    