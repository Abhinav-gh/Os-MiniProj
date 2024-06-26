#include "../../header/book.h"
#include "../../header/borrower.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_DATE_LENGTH 20
#define MAX_BORROWER_ID_LENGTH 20

// Add genre to the BST
void addGenre(struct BSTNodeBook **root, const char *genreName)
{
    struct BSTNodeBook *newNode = (struct BSTNodeBook *)malloc(sizeof(struct BSTNodeBook));
    if (newNode == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    strcpy(newNode->genre.name, genreName);
    newNode->genre.numBooks = 0;
    newNode->left = newNode->right = NULL;

    *root = newNode;
}

// Add book to the BST
void addBook(struct BSTNodeBook **root, const char *genreName, struct LibraryBook *book)
{
    if (*root == NULL)
    {
        addGenre(root, genreName);
    }

    struct BSTNodeBook *current = *root;
    struct BSTNodeBook *parent = NULL;

    while (current != NULL)
    {
        parent = current;
        if (strcmp(genreName, current->genre.name) < 0)
        {
            current = current->left;
        }
        else if (strcmp(genreName, current->genre.name) > 0)
        {
            current = current->right;
        }
        else
        {
            // Add book to the genre
            current->genre.books[current->genre.numBooks++] = *book;
            return;
        }
    }

    struct BSTNodeBook *newNode = (struct BSTNodeBook *)malloc(sizeof(struct BSTNodeBook));
    if (newNode == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    strcpy(newNode->genre.name, genreName);
    newNode->genre.books[newNode->genre.numBooks++] = *book;
    newNode->left = newNode->right = NULL;

    if (strcmp(genreName, parent->genre.name) < 0)
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }
}

// Display all books in the BST
void displayAllBooks(struct BSTNodeBook *root)
{
    if (root != NULL)
    {
        displayAllBooks(root->left);

        printf("Genre: %s\n\n", root->genre.name);
        for (int i = 0; i < root->genre.numBooks; i++)
        {
            struct LibraryBook *book = &root->genre.books[i];
            printf("Title: %s\n", book->title);
            printf("Author: %s\n", book->author);
            printf("ISBN: %s\n", book->ISBN);
            printf("Year Published: %d\n", book->yearPublished);
            printf("Issue Date: %s", asctime(localtime(&book->issueDate)));
            printf("Return Date: %s", asctime(localtime(&book->returnDate)));
            printf("Borrower ID: %d\n", book->borrower.ID);
            printf("\n");
        }

        displayAllBooks(root->right);
    }
}

// Get all books information in the BST in a string
void getAllBooksInfo(struct BSTNodeBook *root, char *booksInfo)
{
    if (root != NULL)
    {
        getAllBooksInfo(root->left, booksInfo);

        // Concatenate genre information to the string
        sprintf(booksInfo + strlen(booksInfo), "Genre: %s\n\n", root->genre.name);
        for (int i = 0; i < root->genre.numBooks; i++)
        {
            struct LibraryBook *book = &root->genre.books[i];
            // Concatenate book information to the string
            sprintf(booksInfo + strlen(booksInfo), "Title: %s\n", book->title);
            sprintf(booksInfo + strlen(booksInfo), "Author: %s\n", book->author);
            sprintf(booksInfo + strlen(booksInfo), "ISBN: %s\n", book->ISBN);
            sprintf(booksInfo + strlen(booksInfo), "Year Published: %d\n", book->yearPublished);
            sprintf(booksInfo + strlen(booksInfo), "Issue Date: %s", asctime(localtime(&book->issueDate)));
            sprintf(booksInfo + strlen(booksInfo), "Return Date: %s", asctime(localtime(&book->returnDate)));
            sprintf(booksInfo + strlen(booksInfo), "Borrower ID: %d\n", book->borrower.ID);
            sprintf(booksInfo + strlen(booksInfo), "\n");
        }

        getAllBooksInfo(root->right, booksInfo);
    }
}
char *getAllBooksInfoWrapper(struct BSTNodeBook *root)
{
    // Allocate memory for the string to store all book information
    char *booksInfo = (char *)malloc(2 * BUFFER_SIZE * sizeof(char));
    if (booksInfo == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    // Initialize the string
    strcpy(booksInfo, "");

    getAllBooksInfo(root, booksInfo);
    return booksInfo;
}

// Remove all copies of the Book from the BST
void removeBook(struct BSTNodeBook **root, const char *genreName, const char *ISBN)
{
    if (*root == NULL)
    {
        return;
    }

    struct BSTNodeBook *current = *root;
    struct BSTNodeBook *parent = NULL;

    while (current != NULL)
    {
        if (strcmp(genreName, current->genre.name) < 0)
        {
            parent = current;
            current = current->left;
        }
        else if (strcmp(genreName, current->genre.name) > 0)
        {
            parent = current;
            current = current->right;
        }
        else
        {
            // Search for the book in the genre
            for (int i = 0; i < current->genre.numBooks; i++)
            {
                if (strcmp(ISBN, current->genre.books[i].ISBN) == 0)
                {
                    // Remove the book by shifting all books to the left
                    for (int j = i; j < current->genre.numBooks - 1; j++)
                    {
                        current->genre.books[j] = current->genre.books[j + 1];
                    }
                    current->genre.numBooks--;
                    return;
                }
            }
            return;
        }
    }
}

// Search for a book in the BST
struct LibraryBook *searchBook(struct BSTNodeBook *root, const char *genreName, const char *ISBN)
{
    if (root == NULL)
    {
        return NULL;
    }

    struct BSTNodeBook *current = root;

    while (current != NULL)
    {
        if (strcmp(genreName, current->genre.name) < 0)
        {
            current = current->left;
        }
        else if (strcmp(genreName, current->genre.name) > 0)
        {
            current = current->right;
        }
        else
        {
            // Search for the book in the genre
            for (int i = 0; i < current->genre.numBooks; i++)
            {
                if (strcmp(ISBN, current->genre.books[i].ISBN) == 0)
                {
                    return &current->genre.books[i];
                }
            }
            return NULL;
        }
    }

    return NULL;
}
// Display info about a particular book in the BST
void displayBookInfo(struct BSTNodeBook *root, const char *genreName, const char *ISBN)
{
    struct LibraryBook *book = searchBook(root, genreName, ISBN);
    if (book == NULL)
    {
        printf("Book not found\n");
        return;
    }

    printf("Title: %s\n", book->title);
    printf("Author: %s\n", book->author);
    printf("ISBN: %s\n", book->ISBN);
    printf("Year Published: %d\n", book->yearPublished);
    printf("Issue Date: %s", asctime(localtime(&book->issueDate)));
    printf("Return Date: %s", asctime(localtime(&book->returnDate)));
    printf("Borrower ID: %d\n", book->borrower.ID);
}

void getBookInfo(struct LibraryBook *book, char *bookInfo)
{
    if (book == NULL)
    {
        strcpy(bookInfo, "Book not found");
        return;
    }
    // Initialize the string
    strcpy(bookInfo, "");
    // Concatenate book information to the string
    sprintf(bookInfo, "Title: %s\n", book->title);
    sprintf(bookInfo + strlen(bookInfo), "Author: %s\n", book->author);
    sprintf(bookInfo + strlen(bookInfo), "ISBN: %s\n", book->ISBN);
    sprintf(bookInfo + strlen(bookInfo), "Year Published: %d\n", book->yearPublished);
    sprintf(bookInfo + strlen(bookInfo), "Issue Date: %s", asctime(localtime(&book->issueDate)));
    sprintf(bookInfo + strlen(bookInfo), "Return Date: %s", asctime(localtime(&book->returnDate)));
    sprintf(bookInfo + strlen(bookInfo), "Borrower ID: %d\n", book->borrower.ID);
    // printf("Book Info: %s\n", bookInfo);
}
// check if the book is available
int isBookAvailable(struct BSTNodeBook *root, const char *genreName, const char *ISBN)
{
    struct LibraryBook *book = searchBook(root, genreName, ISBN);
    if (book == NULL)
    {
        return 0;
    }

    return book->isAvailable;
}

int borrowBook(struct BSTNodeBook *root, const char *genreName, const char *ISBN, struct Borrower *borrower)
{
    struct LibraryBook *book = searchBook(root, genreName, ISBN);
    if (book == NULL)
    {
        printf("Book not found\n");
        return -1;
    }

    if (book->isAvailable)
    {
        if (book->numCopies > 0)
        {
            // if the user has already borrowed this book then dont allow to borrow again
            for (int i = 0; i < borrower->numBorrowedBooks; i++)
            {
                if (borrower->borrowedBooks[i] == book)
                {
                    printf("Book already borrowed\n");
                    return -4;
                }
            }
            book->numCopies--; // Decrement number of copies
            if (book->numCopies == 0)
            {
                book->isAvailable = 0; // Set availability to false if no copies left
            }
            if (borrower->numBorrowedBooks < 3)
            {
                book->borrower = *borrower;
                book->issueDate = time(NULL);
                borrower->borrowedBooks[borrower->numBorrowedBooks++] = book;
                printf("Book borrowed successfully\n");
                return 0;
            }
            else
            {
                printf("You have already borrowed 3 books\n");
                return 1;
            }
        }
        else
        {
            printf("No copies available for borrowing\n");
            return -2;
        }
    }
    else
    {
        printf("Book is not available\n");
        return -3;
    }
}

// Display all books borrowed by a borrower
void displayBorrowedBooks(struct BSTNodeBook *root, struct Borrower *borrower)
{
    for (int i = 0; i < borrower->numBorrowedBooks; i++)
    {
        struct LibraryBook *book = borrower->borrowedBooks[i];
        printf("Title: %s\n", book->title);
        printf("Author: %s\n", book->author);
        printf("ISBN: %s\n", book->ISBN);
        printf("Year Published: %d\n", book->yearPublished);
        printf("Issue Date: %s", asctime(localtime(&book->issueDate)));
        printf("Return Date: %s", asctime(localtime(&book->returnDate)));
        printf("\n");
    }
}

// Calculate fine for a borrower
int calculateFine(struct BSTNodeBook *root, struct Borrower *borrower)
{
    int fine = 0;
    time_t currentTime = time(NULL);

    for (int i = 0; i < borrower->numBorrowedBooks; i++)
    {
        struct LibraryBook *book = borrower->borrowedBooks[i];
        if (currentTime > book->returnDate)
        {
            fine += (currentTime - book->returnDate) / (24 * 60 * 60) * 10; // 10 rupees per day
        }
    }

    return fine;
}

// Add fine to the borrower's data
void addFineToBorrower(struct BSTNodeBook *root, struct Borrower *borrower)
{
    int fine = calculateFine(root, borrower);
    borrower->fine += fine;
}

// Function to return a book. The return date should be put and the book should be returned Should also check for fine. if fine then return -1


// Return a book
int returnBook(struct BSTNodeBook *root, const char *genreName, const char *ISBN, struct Borrower *borrower)
{
    struct LibraryBook *book = searchBook(root, genreName, ISBN);
    if (book == NULL)
    {
        printf("Book not found\n");
        return 1;
    }

    
        for (int i = 0; i < borrower->numBorrowedBooks; i++)
        {
            if (borrower->borrowedBooks[i] == book)
            {
                time_t currentTime = time(NULL);
                int fine = calculateFine(root, borrower); // Calculate fine
                if (fine > 0)
                {
                    printf("You have a fine of %d rupees. Please pay after returning the book.\n", fine);
                    // add fine to the borrower
                    addFineToBorrower(root, borrower);
                    // return -1;
                }
                book->isAvailable = 1;
                book->numCopies++;
                book->issueDate = 0;  // Reset issue date
                book->returnDate = 0; // Reset return date
                borrower->borrowedBooks[i] = NULL;
                borrower->numBorrowedBooks--;
                printf("Book returned successfully\n");
                return 0;
            }
        }
        printf("You have not borrowed this book\n");
        return 2;
    
    
}

// Check if a borrower is late in returning books
int isBorrowerLate(struct BSTNodeBook *root, struct Borrower *borrower)
{
    time_t currentTime = time(NULL);

    for (int i = 0; i < borrower->numBorrowedBooks; i++)
    {
        struct LibraryBook *book = borrower->borrowedBooks[i];
        if (currentTime > book->returnDate)
        {
            return 1;
        }
    }

    return 0;
}

// Display all genres in the BST
void displayAllGenres(struct BSTNodeBook *root)
{
    if (root != NULL)
    {
        displayAllGenres(root->left);
        printf("Genre: %s\n", root->genre.name);
        displayAllGenres(root->right);
    }
}

// Helper function to read a string enclosed in double quotes
void ReadDatabaseBooks(struct BSTNodeBook **root, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char genreName[MAX_GENRE_LENGTH];
    struct LibraryBook book;

    while (fscanf(file, "%s", genreName) == 1)
    {
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

        addBook(root, genreName, &book);
    }

    fclose(file);
}

// Helper function to perform in-order traversal and write to file
void writeBSTToFileHelper(struct BSTNodeBook *root, FILE *file)
{
    if (root == NULL)
        return;

    writeBSTToFileHelper(root->left, file);

    // Write the current node to the file
    for (int i = 0; i < root->genre.numBooks; i++)
    {
        struct LibraryBook *book = &(root->genre.books[i]);
        fprintf(file, "%s %s %s %s %d %d %d %ld %ld %d\n",
                root->genre.name, book->title, book->author,
                book->ISBN, book->numCopies, book->isAvailable,
                book->yearPublished, book->issueDate, book->returnDate, book->borrower.ID);
    }

    writeBSTToFileHelper(root->right, file);
}

// Function to write BST to file
int writeBSTToFile(struct BSTNodeBook *root, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return -1;
    }

    writeBSTToFileHelper(root, file);

    fclose(file);
    return 0;
}

struct BSTNodeBook *root = NULL;

void initializeBooks()
{
    ReadDatabaseBooks(&root, "../database/Books/books.txt");
    printf("📚 Books Database: Initialized. Data repository online and ready for access. 🚀\n");
    // displayAllBooks(root);
    writeBSTToFile(root, "../database/Books/books.txt");

    // return 0;
}