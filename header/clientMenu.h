
#ifndef CLIENT_MENU_H
#define CLIENT_MENU_H

int loginMenu(int sock);
void BorrowerMenu(int sock);
void LibrarianMenu(int sock);
void AdminMenu(int sock);
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50
#define MAX_ISBN_LENGTH 20

struct LibraryBookPacket {
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    char ISBN[MAX_ISBN_LENGTH];
    int numCopies;
    int isAvailable;
    int yearPublished;
};
#endif
