
# Online Library Management System

This is my course Project for Operating Systems course.
##  Run Locally

- Clone the remote repository 
Using SSH
```
git clone -b main git@github.com:Abhinav-gh/Os-MiniProj-Library-Management.git
```
Using HTTP
```
git clone -b main https://github.com/Abhinav-gh/Os-MiniProj-Library-Management.git
```
- Navigate into the project directory
```
cd Os-MiniProj
```
- Open 2 terminals at location. Preferably split terminals.
- Navigate into the source code folder in both the terminals
```
cd src
```
- Make the executables using the makefile provided
```
make clean
```
```
make all
```
- Run the the client in one and the server in other terminal  
```
./server_exec
```
```
./client_exec
```
- You can refer to the **Database/Books/books.txt** file for fields like ISBN and Genre name. Do not add/delete/modify these files. View them as a reference to how to run the project.
## Features and Functionalities implemented

- User Management. 
    - User Authentication 
    - User Registration and login
- Role based access control
    - Server provides authentication and identification of 3 roles  and serves the client accordingly. 
    - 3 roles: Borrower, Librarian, Admin

- Very efficient operations by utilizing **data structures** like the **Binary Search Tree**. We have made the BSTs for books and borrowers. The BST is pre-processed with the database files to get it ready for handling user queries. The corresponding helper functions then update the database files accordingly.
    - Borrwing and Returning books with automatic **fine** calculation by the server.
    - Efficient Book Management. Librarians can **manage** (add/remove/modify) all the books in library.
    - Search and Browse. Users can **search** for books by their unique ISBN number. Searching implemented using BST as mentioned above.
- Multithreading and Concurrent Operations
    - Concurrent user handling. Server support for **multiple clients** performing actions **concurrently** (simultaneously) without performance degradation.
    - Database operation performed by the server in a **separate thread** to prevent blocking. Improves server response time.
    
- Real Time database Management
    - Database implemented through data structures such as BST for fast processing of client queries.
    - Database Integrity by implementing **read-write locks** on database files at the time of database access by the server.
    



## Authors

- [@Abhinav-gh](https://www.github.com/Abhinav-gh)
- [@Transyltooniaa](https://github.com/Transyltooniaa)

