
#ifndef CLIENT_MENU_H
#define CLIENT_MENU_H


#define MAX_NAME_LENGTH 50
#define MAX_ID_LENGTH 20
#define MAX_CONTACT_LENGTH 20
#define MAX_ISBN_LENGTH 20
#define MAX_GENRE_LENGTH 50
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50

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


struct Genre {
    char name[MAX_GENRE_LENGTH];
    struct LibraryBook books[100];
    int numBooks;
};


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



void loginMenu(int sock);

#endif
