#ifndef BORROWER_H
#define BORROWER_H

#define MAX_NAME_LENGTH 50
#define MAX_ID_LENGTH 20
#define MAX_CONTACT_LENGTH 20
#define MAX_ISBN_LENGTH 20

#include "server.h"


struct LibraryBook; // Forward declaration of LibraryBook struct

typedef struct Borrower {
    int ID;
    char username[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH]; 
    long long int contact;
    char borrowedBooks[MAX_ISBN_LENGTH][MAX_NAME_LENGTH];
    int numBorrowedBooks;
    int fine;
    int isLate;
    int LoginStatus;
} Borrower;

// BST node structure
struct BSTNodeBorrower {

    struct Borrower data;
    struct BSTNodeBorrower* left;
    struct BSTNodeBorrower* right;
};

// Function prototypes
struct Borrower* createBorrower(const char* username, const char* name, const char* password, long long int contact, int ID);
struct BSTNodeBorrower* createBSTNodeBorrower(struct Borrower* borrower);
void insertBorrower(struct BSTNodeBorrower** root, struct Borrower borrower);
void borrowerPacketHandler(int new_socket, MsgPacket *packet);
void ReadDatabaseBorrower(struct BSTNodeBorrower  **root, const char *filename);



#endif /* BORROWER_H */