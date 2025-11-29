# Library-DBMS-project
This project is a complete Library Management System built in C, designed to simulate how a simple database management system operates internally. Instead of relying on a traditional SQL database, the system uses dynamic data structures, indexing techniques, and file storage to manage books, members, library cards, and borrowing operations.
The project  focuses on applying database concepts at a low level, including storage management, indexing, concurrency control, and CRUD operations.
Key Features
Book Management

Add, delete, update, and display books

Stored in a binary search tree for sorted access

Persistent storage using CSV files

Member Management

Register, update, delete, and display members

Fast lookup using hash-based indexing (linear probing)

Linked list storage for member records

Duplicate ID validation

Library Card System

Issue a library card to an existing member

Enforces one-card-per-member rule

Update and delete card records

Borrow and Return Operations

Maintain a list of borrowed books

Validate member and book existence before borrowing

Prevent multiple borrowing of the same book by the same member

Concurrency Control

Basic concurrency implemented using pthread mutex locks

Ensures thread-safe operations during borrow and return actions

Persistent Storage

All records stored as CSV:

books.csv

members.csv

borrowed.csv

cards.csv

Data is loaded at program startup and saved at termination



The project simulates several key DBMS components:

Execution Engine: Each menu option maps to a function that performs the required operation (insert, delete, update, search).

Storage Manager: CSV file handling using standard C file I/O.

Buffer/Data Manager: In-memory structures such as BSTs, linked lists, and hash tables.

Indexing: Linear probing hash table for member lookups.

Concurrency Control: Mutex-based locking during critical operations.

Logging and Recovery (Simplified): Data is persisted to files after updates.

Although not all components of a commercial DBMS are implemented, this project provides a practical demonstration of how database operations can be simulated at a low level.

Binary search trees for book management

Linked lists for members, cards, and borrow records

Hash table indexing for fast member searches

Mutex locks for basic concurrency control

CSV-based persistent storage

Manual implementation of constraints (unique IDs, foreign key checks)

Dynamic memory management using malloc and free


How to Compile and Run

Compile the program using GCC with pthread support:
gcc db_prog_final-after.c -o library_system -lpthread

Run the program:

./library_system


CSV files will be created automatically if they do not exist.
