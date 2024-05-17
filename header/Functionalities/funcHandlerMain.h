#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// includ thing which gives send()
#include <sys/socket.h>

#ifndef FUNC_HANDLER_MAIN_H
#define FUNC_HANDLER_MAIN_H
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50
#define MAX_ISBN_LENGTH 20

#define MAX_NAME_LENGTH 50

struct LibraryBookPacket {
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    char ISBN[MAX_ISBN_LENGTH];
    int numCopies;
    int isAvailable;
    int yearPublished;
};

// define BorrowerPacket
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
// #define BUFFER_SIZE 4096*4   

#endif