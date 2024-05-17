#ifndef AUTH_H
#define AUTH_H

#define MAX_LINE_LENGTH 1024
#define MAX_FILENAME_LENGTH 128
#define MAX_USERS 100
#define BUFFER_SIZE 4096 


typedef struct {
    char username[MAX_LINE_LENGTH];
    char password[MAX_LINE_LENGTH];
} User;


int authHandler(int new_socket);
User *read_user_data(const char *filename, const char *role);
int authenticate_user(const char *username, const char *password, const char *role);
void free_user(User *user);
void free_users(User *users, int num_users);
void send_auth_response(int new_socket, int status);



#endif /* AUTH_H */
