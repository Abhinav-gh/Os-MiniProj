#include "../header/book.h"
#include "../header/borrower.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>



// Function to create a new book
struct LibraryBook* createBook(const char* title, const char* author, const char* ISBN, int numCopies, int isAvailable, int yearPublished, time_t issueDate, time_t returnDate, long int borrowerID) {
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
    book->borrower.ID = borrowerID;

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
        printf("Borrower ID: %d\n", book->borrower.ID);
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
        book.borrower.ID = strtol(borrowerID, NULL, 10);

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
        fprintf(file, "%s %s %s %s %d %d %d %ld %ld %d\n", 
                root->genre.name, book->title, book->author, 
                book->ISBN, book->numCopies, book->isAvailable, 
                book->yearPublished, book->issueDate, book->returnDate, book->borrower.ID);
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



int main(){
    struct BSTNodeBook *root = NULL;
    ReadDatabaseBook(&root, "../database/Books/books.txt");
    displayAllBooks(root);
    writeBSTToFileBook(root, "../database/Books/books.txt");
 
    return 0;
}