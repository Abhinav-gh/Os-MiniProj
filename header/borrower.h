#ifndef BORROWER_H
#define BORROWER_H

#define MAX_NAME_LENGTH 50
#define MAX_ID_LENGTH 20
#define MAX_CONTACT_LENGTH 20

struct LibraryBook; // Forward declaration of LibraryBook struct

typedef struct Borrower {
    char username[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH]; 
    long long int contact;
    int ID;
    struct LibraryBook* borrowedBooks[3]; // Now using pointers to LibraryBook
    int numBorrowedBooks;
    int fine;
    int isLate;
} Borrower;

// BST node structure
struct BSTNodeBorrower {
    struct Borrower data;
    struct BSTNodeBorrower* left;
    struct BSTNodeBorrower* right;
};

// Function prototypes
struct Borrower* createBorrower(const char* username, const char* name, const char* password, long long int contact, int ID);



#endif /* BORROWER_H */