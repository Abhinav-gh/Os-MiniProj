
#ifndef CLIENT_MENU_H
#define CLIENT_MENU_H


int loginMenu(int sock);
void BorrowerMenu(int sock);
void LibrarianMenu(int sock);
void AdminMenu(int sock);

// for library book packet
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50
#define MAX_ISBN_LENGTH 20

//for borrower packet
#define MAX_NAME_LENGTH 50

struct LibraryBookPacket {
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    char ISBN[MAX_ISBN_LENGTH];

    int numCopies;
    int isAvailable;
    int yearPublished;
};

struct BorrowerPacket {
    char username[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    long long int contact;
    int ID;
    int numBorrowedBooks;
    int fine;
    int isLate;
};
#endif
