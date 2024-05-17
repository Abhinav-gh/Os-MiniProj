#include "../header/book.h"
#include "../header/borrower.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>


void ReadAllGenres(int socket, struct BSTNodeBook *root, MsgPacket *packet) {
    if (root == NULL)
        return;

    ReadAllGenres(socket, root->left, packet);

    if (send(socket, root->genre.name, strlen(root->genre.name) + 1, 0) == -1) { // +1 to include the null terminator
        perror("send");
        return;
    }
    usleep(10000); // Optional, consider removing or reducing the sleep duration

    ReadAllGenres(socket, root->right, packet);

    // Send end-of-transmission message
    if (root->left == NULL && root->right == NULL) {
        const char *endOfTransmission = "END_OF_TRANSMISSION";
        send(socket, endOfTransmission, strlen(endOfTransmission) + 1, 0);
    }
}




void ReadAllBooks(int socket, struct BSTNodeBook *root, MsgPacket *packet) {
    if (root == NULL)
        return;

    ReadAllBooks(socket, root->left, packet);

    char buffer[300]; 
    for (int i = 0; i < root->genre.numBooks; i++) {
        struct LibraryBook *book = &(root->genre.books[i]);
        snprintf(buffer, sizeof(buffer), "%s\nISBN : %s\n\n", book->title, book->ISBN);
        
        if (send(socket, buffer, strlen(buffer) + 1, 0) == -1) { 
            perror("send");
            return;
        }
        usleep(10000); 
    }

    ReadAllBooks(socket, root->right, packet);

    const char *endOfTransmission = "END_OF_TRANSMISSION";
    send(socket, endOfTransmission, strlen(endOfTransmission) + 1, 0);
}



// Function to create a new book
struct LibraryBook* createBook(const char* title, const char* author, const char* ISBN, int numCopies, int isAvailable, int yearPublished, time_t issueDate, time_t returnDate,char *username) {
    struct LibraryBook* book = (struct LibraryBook*)malloc(sizeof(struct LibraryBook));
    if (book == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    strcpy(book->title, title);
    strcpy(book->author, author);
    strcpy(book->ISBN, ISBN);
    book->numCopies = numCopies;
    book->isAvailable = isAvailable;
    book->yearPublished = yearPublished;
    book->issueDate = issueDate;
    book->returnDate = returnDate;
    book->borrowerUsername = username;

    return book;
}


// Function to create a new BST node
struct BSTNodeBook* createBSTNodeBook(struct LibraryBook* book) {
    struct BSTNodeBook* newNode = (struct BSTNodeBook*)malloc(sizeof(struct BSTNodeBook));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    newNode->genre.numBooks = 0;
    strcpy(newNode->genre.name, "");
    newNode->left = newNode->right = NULL;

    return newNode;
}

// Function to insert a book into the BST
void insertBook(struct BSTNodeBook** root, const char* genreName, struct LibraryBook* book) {
    if (*root == NULL) {
        *root = createBSTNodeBook(book);
        strcpy((*root)->genre.name, genreName);
        (*root)->genre.books[0] = *book;
        (*root)->genre.numBooks++;
        return;
    }

    if (strcmp(genreName, (*root)->genre.name) < 0) {
        insertBook(&((*root)->left), genreName, book);
    } else if (strcmp(genreName, (*root)->genre.name) > 0) {
        insertBook(&((*root)->right), genreName, book);
    } else {
        (*root)->genre.books[(*root)->genre.numBooks] = *book;
        (*root)->genre.numBooks++;
    }
}



// Function to display all books in the BST
void displayAllBooks(struct BSTNodeBook* root) {
    if (root == NULL) {
        return;
    }

    displayAllBooks(root->left);

    printf("Genre: %s\n", root->genre.name);
    for (int i = 0; i < root->genre.numBooks; i++) {
        struct LibraryBook* book = &(root->genre.books[i]);
        printf("Title: %s\n", book->title);
        printf("Author: %s\n", book->author);
        printf("ISBN: %s\n", book->ISBN);
        printf("Number of copies: %d\n", book->numCopies);
        printf("Availability: %s\n", book->isAvailable ? "Yes" : "No");
        printf("Year Published: %d\n", book->yearPublished);
        printf("Issue Date: %s", ctime(&book->issueDate));
        printf("Return Date: %s", ctime(&book->returnDate));
        printf("Borrower Username: %s\n", book->borrowerUsername);
        printf("\n");
    }

    displayAllBooks(root->right);
}


// Helper function to read a string enclosed in double quotes
void ReadDatabaseBook(struct BSTNodeBook **root, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char genreName[MAX_GENRE_LENGTH];
    struct LibraryBook book;

    while (fscanf(file, "%s", genreName) == 1) {
        fscanf(file, "%s", book.title);
        fscanf(file, "%s", book.author);
        fscanf(file, "%s", book.ISBN);
        fscanf(file, "%d", &book.numCopies);
        fscanf(file, "%d", &book.isAvailable);
        fscanf(file, "%d", &book.yearPublished);
        fscanf(file, "%ld", &book.issueDate);
        fscanf(file, "%ld", &book.returnDate);
        char borrowerID[MAX_BORROWER_ID_LENGTH];
        fscanf(file, "%s", borrowerID);
        book.borrowerUsername = strdup(borrowerID);

        insertBook(root, genreName, &book);
    }

    fclose(file);
}



// Helper function to perform in-order traversal and write to file
void writeBSTToFileHelperBook(struct BSTNodeBook *root, FILE *file) {
    if (root == NULL)
        return;

    writeBSTToFileHelperBook(root->left, file);

    // Write the current node to the file
    for (int i = 0; i < root->genre.numBooks; i++) {
        struct LibraryBook *book = &(root->genre.books[i]);
        fprintf(file, "%s %s %s %s %d %d %d %ld %ld %s\n", 
                root->genre.name, book->title, book->author, 
                book->ISBN, book->numCopies, book->isAvailable, 
                book->yearPublished, book->issueDate, book->returnDate, book->borrowerUsername);
    }

    writeBSTToFileHelperBook(root->right, file);
}



// Function to write BST to file
void writeBSTToFileBook(struct BSTNodeBook *root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    writeBSTToFileHelperBook(root, file);

    fclose(file);
}



int borrowBook(int socket, struct BSTNodeBook *root, const char *ISBN, char *username) {


    if (root == NULL) {
        send(socket, "Book not found", 14, 0);
        usleep(100000);
        const char *eot = "END_OF_TRANSMISSION";
        send(socket, eot, strlen(eot) + 1, 0);
        return 0;
    }


    for (int i = 0; i < root->genre.numBooks; i++)
     {
        struct LibraryBook *book = &(root->genre.books[i]);

        if (strcmp(book->ISBN, ISBN) == 0) 
        {
            if (book->numCopies == 0) 
            {
                book->isAvailable = 0;
                send(socket, "Book not available", 18, 0);
                usleep(100000);
                const char *eot = "END_OF_TRANSMISSION";
                send(socket, eot, strlen(eot) + 1, 0);
                return 0;
            } 
    
            else 
            {
                book->numCopies--;
                book->isAvailable = book->numCopies > 0;
                book->issueDate = time(NULL);
                book->returnDate = book->issueDate + 604800; // 7 days
                strncpy(book->borrowerUsername, username, sizeof(book->borrowerUsername) - 1);
                book->borrowerUsername[sizeof(book->borrowerUsername) - 1] = '\0';
                send(socket, "Book borrowed successfully", 26, 0);
                usleep(100000);
                const char *eot = "END_OF_TRANSMISSION";
                send(socket, eot, strlen(eot) + 1, 0);
                usleep(100000);
                return 1; 
            }
        }
    }


    if (borrowBook(socket, root->left, ISBN, username)) {
        return 1;
    }


    if (borrowBook(socket, root->right, ISBN, username)) {
        return 1;
    }

    const char *eot = "END_OF_TRANSMISSION";
    send(socket, eot, strlen(eot) + 1, 0);
    return 0;
}



void FetchBookNameFromISBN(struct BSTNodeBook *root, const char *ISBN, char *bookName) {
    if (root == NULL) {
        return;
    }

    FetchBookNameFromISBN(root->left, ISBN, bookName);

    for (int i = 0; i < root->genre.numBooks; i++) {
        struct LibraryBook *book = &(root->genre.books[i]);
        if (strcmp(book->ISBN, ISBN) == 0) {
            strcpy(bookName, book->title);
            return;
        }
    }

    FetchBookNameFromISBN(root->right, ISBN, bookName);
}



// Check remaining time for book to return in days with respect to current time in days using bookName
int CheckRemainingTimeForBookReturn(struct BSTNodeBook *root, const char *bookName) {
    if (root == NULL) {
        return INT_MAX;
    }

    int left = CheckRemainingTimeForBookReturn(root->left, bookName);

    for (int i = 0; i < root->genre.numBooks; i++) {
        struct LibraryBook *book = &(root->genre.books[i]);
        if (strcmp(book->title, bookName) == 0) {
            time_t currentTime = time(NULL);
            int remainingTime = (int)((book->returnDate - currentTime) / 86400); // Convert seconds to days
            return remainingTime;
        }
    }

    int right = CheckRemainingTimeForBookReturn(root->right, bookName);

    return left < right ? left : right;
}
