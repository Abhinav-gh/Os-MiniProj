#ifndef AUTH_H
#define AUTH_H

#define MAX_LINE_LENGTH 1024
#define MAX_FILENAME_LENGTH 128
#define MAX_USERS 100
#define BUFFER_SIZE 1024    


typedef struct {
    char username[MAX_LINE_LENGTH];
    char password[MAX_LINE_LENGTH];
} User;



int authenticate_user(const char *username, const char *password, const char *role);
void authHandler(int new_socket);

#endif /* AUTH_H */
