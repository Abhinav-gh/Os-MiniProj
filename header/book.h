#ifndef BST_GENRE_H
#define BST_GENRE_H

#define MAX_GENRE_LENGTH 50
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50
#define MAX_ISBN_LENGTH 20
#define MAX_DATE_LENGTH 20
#define MAX_BORROWER_ID_LENGTH 20


#include <time.h>
#include "borrower.h"


struct Borrower; 

struct LibraryBook {
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    char ISBN[MAX_ISBN_LENGTH];
    int numCopies;
    int isAvailable;
    int yearPublished;
    time_t issueDate;
    time_t returnDate;
    struct Borrower borrower;
};


// Structure for a Genre
struct Genre {
    char name[MAX_GENRE_LENGTH];
    struct LibraryBook books[100]; // Assuming each genre can have at most 100 books
    int numBooks;
};

// Structure for a BST node
struct BSTNodeBook {
    struct Genre genre;
    struct BSTNodeBook *left;
    struct BSTNodeBook *right;
};

// Function prototypes


#endif /* BST_GENRE_H */
