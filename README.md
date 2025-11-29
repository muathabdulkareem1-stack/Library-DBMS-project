
# Library-DBMS-project

This project is a complete Library Management System built in C, designed to simulate how a simple database management system operates internally. Instead of relying on a traditional SQL database, the system uses dynamic data structures, indexing techniques, and file storage to manage books, members, library cards, and borrowing operations.

The project focuses on applying database concepts at a low level, including storage management, indexing, concurrency control, and CRUD operations.

## Key Features

### Book Management

* Add, delete, update, and display books
* Stored in a binary search tree for sorted access
* Persistent storage using CSV files

### Member Management

* Register, update, delete, and display members
* Fast lookup using hash-based indexing (linear probing)
* Linked list storage for member records
* Duplicate ID validation

### Library Card System

* Issue a library card to an existing member
* Enforces one-card-per-member rule
* Update and delete card records

### Borrow and Return Operations

* Record borrowing operations
* Validate member and book existence
* Prevent duplicate borrowing
* Record book returns

## System Architecture

The system is implemented using:

* Binary Search Trees (BST)
* Singly Linked Lists
* Hash Table Indexing with linear probing
* Mutex-based concurrency for borrow/return operations
* CSV file I/O for persistence

## Project Structure

```
Library-DBMS-project/
│
├── src/
│   └── db_prog_final-after.c
│
├── documentation/
│   └── Report.docx
│
└── README.md
```

## How to Compile and Run

Compile the program using GCC with pthread:

```
gcc db_prog_final-after.c -o library_system -lpthread
```

Run the program:

```
./library_system
```

CSV files will be created automatically if they do not exist.

## Documentation

The full written report detailing system design, data structures, and DBMS concepts is available in:

```
documentation/Report.docx
```
