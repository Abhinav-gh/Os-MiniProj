#include "../header/borrower.h"
#include "../header/book.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

#include "../header/borrower.h"
#include <stdlib.h>
#include <string.h>

// Function prototypes

// Function to create a new Borrower
struct Borrower *createBorrower(const char *username, const char *name, const char *password, long long int contact, int ID)
{
    struct Borrower *newBorrower = (struct Borrower *)malloc(sizeof(struct Borrower));
    if (newBorrower == NULL)
    {
        // Memory allocation failed
        return NULL;
    }

    strncpy(newBorrower->username, username, MAX_NAME_LENGTH - 1);
    newBorrower->username[MAX_NAME_LENGTH - 1] = '\0';

    strncpy(newBorrower->name, name, MAX_NAME_LENGTH - 1);
    newBorrower->name[MAX_NAME_LENGTH - 1] = '\0';

    strncpy(newBorrower->password, password, MAX_NAME_LENGTH - 1);
    newBorrower->password[MAX_NAME_LENGTH - 1] = '\0';

    newBorrower->contact = contact;
    newBorrower->ID = ID;
    newBorrower->numBorrowedBooks = 0;
    newBorrower->fine = 0;
    newBorrower->isLate = 0;

    // Initialize borrowedBooks array to NULL pointers
    for (int i = 0; i < 3; i++)
    {
        newBorrower->borrowedBooks[i] = NULL;
    }

    return newBorrower;
}

// Function to add a borrowed book to a borrower
int addBorrowedBook(struct Borrower *borrower, struct LibraryBook *book)
{
    if (borrower == NULL || book == NULL)
    {
        return 0; // Invalid borrower or book
    }

    if (borrower->numBorrowedBooks >= 3)
    {
        return -1; // Borrower already has maximum number of borrowed books
    }

    borrower->borrowedBooks[borrower->numBorrowedBooks++] = book;
    return 1; // Successfully added book
}

// Function to remove a borrowed book from a borrower
int removeBorrowedBook(struct Borrower *borrower, struct LibraryBook *book)
{
    if (borrower == NULL || book == NULL)
    {
        return 0; // Invalid borrower or book
    }

    int found = 0;
    for (int i = 0; i < borrower->numBorrowedBooks; i++)
    {
        if (borrower->borrowedBooks[i] == book)
        {
            borrower->borrowedBooks[i] = NULL;
            found = 1;
            break;
        }
    }

    if (!found)
    {
        return -1; // Book not found in borrower's borrowed books
    }

    // Shift remaining books to fill the gap
    for (int i = 0; i < borrower->numBorrowedBooks - 1; i++)
    {
        if (borrower->borrowedBooks[i] == NULL)
        {
            borrower->borrowedBooks[i] = borrower->borrowedBooks[i + 1];
            borrower->borrowedBooks[i + 1] = NULL;
        }
    }

    borrower->numBorrowedBooks--;
    return 1; // Successfully removed book
}

// Function to reset borrower's fines
void resetFines(struct Borrower *borrower)
{
    if (borrower == NULL)
    {
        return;
    }
    borrower->fine = 0;
}

// Function to check if a borrower has borrowed a specific book
int hasBorrowedBook(struct Borrower *borrower, struct LibraryBook *book)
{
    if (borrower == NULL || book == NULL)
    {
        return 0; // Invalid borrower or book
    }

    for (int i = 0; i < borrower->numBorrowedBooks; i++)
    {
        if (borrower->borrowedBooks[i] == book)
        {
            return 1; // Borrower has borrowed this book
        }
    }

    return 0; // Borrower hasn't borrowed this book
}

// Function to free memory allocated for a Borrower
void freeBorrower(struct Borrower *borrower)
{
    free(borrower);
}

// insert a new borrower into the BST
struct BSTNodeBorrower *insertBorrower(struct BSTNodeBorrower *root, struct Borrower *borrower)
{
    if (root == NULL)
    {
        struct BSTNodeBorrower *newNode = (struct BSTNodeBorrower *)malloc(sizeof(struct BSTNodeBorrower));
        if (newNode == NULL)
        {
            return NULL; // Memory allocation failed
        }

        newNode->data = *borrower;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (strcmp(borrower->username, root->data.username) < 0)
    {
        root->left = insertBorrower(root->left, borrower);
    }
    else
    {
        root->right = insertBorrower(root->right, borrower);
    }

    return root;
}

// Function to display all borrowers in the BST
void displayBorrowers(struct BSTNodeBorrower *root)
{
    if (root == NULL)
    {
        return;
    }

    displayBorrowers(root->left);
    printf("Username: %s\n", root->data.username);
    printf("Name: %s\n", root->data.name);
    printf("Contact: %lld\n", root->data.contact);
    printf("ID: %d\n", root->data.ID);
    printf("\n");
    displayBorrowers(root->right);
}

char *getAllBorrowersInfoWrapper(struct BSTNodeBorrower *root)
{
    // Allocate memory for the string to store all borrower information
    char *borrowersInfo = (char *)malloc(2 * BUFFER_SIZE * sizeof(char));
    if (borrowersInfo == NULL)
    {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    // Initialize the string
    strcpy(borrowersInfo, "");
    // printf("coming hre\n");
    getAllBorrowersInfo(root, borrowersInfo);
    // printf("borrowersInfo: %s\n", borrowersInfo);
    return borrowersInfo;
}

void getAllBorrowersInfo(struct BSTNodeBorrower *root, char *borrowersInfo)
{
    if (root == NULL)
    {
        return;
    }

    getAllBorrowersInfo(root->left, borrowersInfo);

    // Concatenate borrower information to the string
    sprintf(borrowersInfo + strlen(borrowersInfo), "Username: %s\n", root->data.username);
    sprintf(borrowersInfo + strlen(borrowersInfo), "Name: %s\n", root->data.name);
    sprintf(borrowersInfo + strlen(borrowersInfo), "Contact: %lld\n", root->data.contact);
    sprintf(borrowersInfo + strlen(borrowersInfo), "ID: %d\n", root->data.ID);
    sprintf(borrowersInfo + strlen(borrowersInfo), "\n");

    getAllBorrowersInfo(root->right, borrowersInfo);
}
// Function to search for a borrower in the BST
struct BSTNodeBorrower *searchBorrower(struct BSTNodeBorrower *root, const char *username)
{
    if (root == NULL || strcmp(username, root->data.username) == 0)
    {
        return root;
    }

    if (strcmp(username, root->data.username) < 0)
    {
        return searchBorrower(root->left, username);
    }
    else
    {
        return searchBorrower(root->right, username);
    }
}

// Function to delete a borrower from the BST
struct BSTNodeBorrower *deleteBorrower(struct BSTNodeBorrower *root, const char *username)
{
    if (root == NULL)
    {
        return root;
    }

    if (strcmp(username, root->data.username) < 0)
    {
        root->left = deleteBorrower(root->left, username);
    }
    else if (strcmp(username, root->data.username) > 0)
    {
        root->right = deleteBorrower(root->right, username);
    }
    else
    {
        // Node with only one child or no child
        if (root->left == NULL)
        {
            struct BSTNodeBorrower *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct BSTNodeBorrower *temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children: Get the inorder successor (smallest in the right subtree)
        struct BSTNodeBorrower *temp = root->right;
        while (temp->left != NULL)
        {
            temp = temp->left;
        }

        // Copy the inorder successor's content to this node
        root->data = temp->data;

        // Delete the inorder successor
        root->right = deleteBorrower(root->right, temp->data.username);
    }

    return root;
}

// Function to free memory allocated for the BST
void freeBSTBorrowers(struct BSTNodeBorrower *root)
{
    if (root == NULL)
    {
        return;
    }

    freeBSTBorrowers(root->left);
    freeBSTBorrowers(root->right);
    free(root);
}

// Function to read borrower data from a file and insert into BST
// Function to read borrower data from a file and insert into BST
void ReadDatabaseBorrower(struct BSTNodeBorrower **root, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file) != NULL)
    {
        // Remove newline character if present
        buffer[strcspn(buffer, "\n")] = 0;

        char *username = strtok(buffer, " ");
        char *name = strtok(NULL, " ");
        char *password = strtok(NULL, " ");
        char *contact = strtok(NULL, " ");
        char *ID = strtok(NULL, " ");

        // Skip the additional fields
        strtok(NULL, " "); // Skip the first borrowed book
        strtok(NULL, " "); // Skip the second borrowed book
        strtok(NULL, " "); // Skip the third borrowed book
        strtok(NULL, " "); // Skip numBorrowedBooks
        strtok(NULL, " "); // Skip fine
        strtok(NULL, " "); // Skip isLate

        // Check if all fields are present
        if (username == NULL || name == NULL || password == NULL || contact == NULL || ID == NULL)
        {
            fprintf(stderr, "Error: Invalid format in file\n");
            continue; // Skip this line and proceed with the next
        }

        long long int contactNum = atoll(contact);
        int IDNum = atoi(ID);

        Borrower *newBorrower = createBorrower(username, name, password, contactNum, IDNum);
        if (newBorrower == NULL)
        {
            fprintf(stderr, "Error: Memory allocation failed\n");
            continue; // Skip this line and proceed with the next
        }

        *root = insertBorrower(*root, newBorrower);
    }

    fclose(file);
}

struct BSTNodeBorrower *borrowerRoot = NULL;
int initializeBorrower()
{

    ReadDatabaseBorrower(&borrowerRoot, "../database/users/borrower.txt");

    // displayBorrowers(root);
    // char * borrowerInfo = getAllBorrowersInfoWrapper(borrowerRoot);
    // printf("%s\n", borrowerInfo);

    // freeBSTBorrowers(root);

    return 0;
}