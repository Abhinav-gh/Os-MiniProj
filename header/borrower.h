#ifndef BORROWER_H
#define BORROWER_H

#define MAX_NAME_LENGTH 50
#define MAX_ID_LENGTH 20
#define MAX_CONTACT_LENGTH 20

struct LibraryBook; // Forward declaration of LibraryBook struct

struct Borrower {
    char name[MAX_NAME_LENGTH];
    char ID[MAX_ID_LENGTH];
    char contact[MAX_CONTACT_LENGTH];
    struct LibraryBook* borrowedBooks[3]; // Now using pointers to LibraryBook
    int numBorrowedBooks;
    int fine;
    int isLate;
};

#endif /* BORROWER_H */