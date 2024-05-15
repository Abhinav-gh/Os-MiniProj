#ifndef ADMIN_H
#define ADMIN_H


typedef struct Admin {
    char username[MAX_USERNAME_LENGTH];                
    char name[MAX_NAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} Admin;


#endif /* ADMIN_H */