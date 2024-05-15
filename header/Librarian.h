#ifndef LIBRARIAN_H
#define LIBRARIAN_H


#define MAX_NAME_LENGTH 50
#define MAX_EMAIL_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_USERNAME_LENGTH 50

typedef struct Librarian {               
    char username[MAX_USERNAME_LENGTH];                
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} Librarian;

#endif 