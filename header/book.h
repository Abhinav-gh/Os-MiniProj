#ifndef BST_GENRE_H
#define BST_GENRE_H

#define MAX_GENRE_LENGTH 50
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50
#define MAX_ISBN_LENGTH 20
#define MAX_DATE_LENGTH 20
#define MAX_BORROWER_ID_LENGTH 50


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
    char *borrowerUsername;
};


// Structure for a Genre
struct Genre {
    char name[MAX_GENRE_LENGTH];
    struct LibraryBook books[100]; // Assuming each genre can have at most 100 books
    int numBooks; // Number of books in the genre
};

// Structure for a BST node
struct BSTNodeBook {
    struct Genre genre;
    struct BSTNodeBook *left;
    struct BSTNodeBook *right;
};


void insertBook(struct BSTNodeBook** root, const char* genreName, struct LibraryBook* book);
struct BSTNodeBook* createBSTNodeBook(struct LibraryBook* book);
struct LibraryBook* createBook(const char* title, const char* author, const char* ISBN, int numCopies, int isAvailable, int yearPublished, time_t issueDate, time_t returnDate, char *username);
void ReadDatabaseBook(struct BSTNodeBook **root, const char *filename);
void ReadAllGenres(int socket , struct BSTNodeBook *root , MsgPacket* packet);
void ReadAllBooks(int socket, struct BSTNodeBook *root, MsgPacket *packet);
int borrowBook(int socket, struct BSTNodeBook *root, const char *ISBN, char *username);
void FetchBookNameFromISBN(struct BSTNodeBook *root, const char *ISBN, char *bookName);
void writeBSTToFileBook(struct BSTNodeBook *root, const char *filename);
int CheckRemainingTimeForBookReturn(struct BSTNodeBook *root, const char *bookName);
int returnBook(int socket, struct BSTNodeBook *root, const char *ISBN, char *username);
int validateISBN(const char *ISBN);

#endif /* BST_GENRE_H */
