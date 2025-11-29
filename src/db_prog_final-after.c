#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/////////////////////////////////////
// BOOK STRUCT (BST)
/////////////////////////////////////
pthread_mutex_t data_lock;
struct Book {
    int isbn;
    char book_name[50];
    int shelf_id;
    struct Book* left;
    struct Book* right;
};

struct Book* bookRoot = NULL;

struct Book* createBookNode(int isbn, char* name, int shelf_id) {
    struct Book* newNode = (struct Book*)malloc(sizeof(struct Book));
    newNode->isbn = isbn;
    strcpy(newNode->book_name, name);
    newNode->shelf_id = shelf_id;
    newNode->left = newNode->right = NULL;
    return newNode;
}

struct Book* insertBookNode(struct Book* root, int isbn, char* name, int shelf_id) {
    if (root == NULL)
        return createBookNode(isbn, name, shelf_id);

    if (isbn < root->isbn)
        root->left = insertBookNode(root->left, isbn, name, shelf_id);
    else if (isbn > root->isbn)
        root->right = insertBookNode(root->right, isbn, name, shelf_id);
    else
        printf("Duplicate ISBN. Book not added.\n");

    return root;
}

void insertBook() {
    int isbn, shelf_id;
    char name[50];

    printf("\n--- Insert Book ---\n");
    printf("Enter ISBN: ");
    scanf("%d", &isbn);
    printf("Enter Book Name: ");
    scanf(" %[^\n]", name);
    printf("Enter Shelf ID: ");
    scanf("%d", &shelf_id);

    bookRoot = insertBookNode(bookRoot, isbn, name, shelf_id);
    printf("Book added successfully.\n");
}

struct Book* findMin(struct Book* node) {
    while (node && node->left != NULL)
        node = node->left;
    return node;
}

struct Book* deleteBookNode(struct Book* root, int isbn) {
    if (root == NULL)
        return NULL;

    if (isbn < root->isbn) {
        root->left = deleteBookNode(root->left, isbn);
    } else if (isbn > root->isbn) {
        root->right = deleteBookNode(root->right, isbn);
    } else {
        // Node found
        if (root->left == NULL) {
            struct Book* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Book* temp = root->left;
            free(root);
            return temp;
        } else {
            // Node with two children
            struct Book* successor = findMin(root->right);
            root->isbn = successor->isbn;
            strcpy(root->book_name, successor->book_name);
            root->shelf_id = successor->shelf_id;
            root->right = deleteBookNode(root->right, successor->isbn);
        }
    }
    return root;
}

void deleteBook() {
    if (bookRoot == NULL) {
        printf("No books to delete.\n");
        return;
    }

    int isbn;
    printf("\n--- Delete Book ---\n");
    printf("Enter ISBN to delete: ");
    scanf("%d", &isbn);

    bookRoot = deleteBookNode(bookRoot, isbn);
    printf("Book deleted if it existed.\n");
}


void displayBooksInOrder(struct Book* root) {
    if (root == NULL) return;

    displayBooksInOrder(root->left);
    printf("ISBN: %d | Name: %s | Shelf ID: %d\n", root->isbn, root->book_name, root->shelf_id);
    displayBooksInOrder(root->right);
}

void displayBooks() {
    if (bookRoot == NULL) {
        printf("No books found.\n");
        return;
    }

    printf("\n--- Book List ---\n");
    displayBooksInOrder(bookRoot);
}

void saveBooksToCSV(FILE* file, struct Book* root) {
    if (root == NULL) return;

    saveBooksToCSV(file, root->left);
    fprintf(file, "%d,%s,%d\n", root->isbn, root->book_name, root->shelf_id);
    saveBooksToCSV(file, root->right);
}

void saveAllBooks() {
    FILE* file = fopen("books.csv", "w");
    if (file == NULL) {
        printf("Error saving books to CSV.\n");
        return;
    }

    // Write CSV header
    fprintf(file, "ISBN,Book Name,Shelf ID\n");

    // Save each book node
    saveBooksToCSV(file, bookRoot);

    fclose(file);
    printf("Books saved to books.csv successfully.\n");
}

void loadBooksFromCSV(FILE* file) {
    char line[256];
    int isHeader = 1;

    while (fgets(line, sizeof(line), file)) {
        if (isHeader) {
            isHeader = 0;
            continue;  // Skip the header row
        }

        int isbn, shelf_id;
        char name[50];

        // each line is formatted as: 1234,Book Title,5
        if (sscanf(line, "%d,%49[^,],%d", &isbn, name, &shelf_id) == 3) {
            bookRoot = insertBookNode(bookRoot, isbn, name, shelf_id);
        } else {
            printf("Malformed line in CSV: %s", line);
        }
    }
}


void loadAllBooks() {
    FILE* file = fopen("books.csv", "r");
    if (file == NULL) {
        printf("No CSV file found for books.\n");
        return;
    }

    loadBooksFromCSV(file);
    fclose(file);
    printf("Books loaded from books.csv successfully.\n");
}

/////////////////////////////////////
// MEMBER STRUCT (LINKED LIST)
/////////////////////////////////////

struct LibraryMember {
    int member_id;
    char name[50];
    char address[100];
    struct LibraryMember* next;
};
#define MEMBER_HASH_SIZE 100
struct LibraryMember* memberIndex[MEMBER_HASH_SIZE] = { NULL };

struct LibraryMember* memberHead = NULL;

struct LibraryMember* createMemberNode(int id, char* name, char* address) {
    struct LibraryMember* newNode = (struct LibraryMember*)malloc(sizeof(struct LibraryMember));
    newNode->member_id = id;
    strcpy(newNode->name, name);
    strcpy(newNode->address, address);
    newNode->next = NULL;
    return newNode;
}

int memberHash(int member_id) {
    return member_id % MEMBER_HASH_SIZE;
}

int isMemberIDExists(int id) {
    struct LibraryMember* temp = memberHead;
    while (temp != NULL) {
        if (temp->member_id == id)
            return 1; // found
        temp = temp->next;
    }
    return 0;
}


void insertMember() {
    int id;
    char name[50], address[100];


    printf("\n--- Insert Member ---\n");
    printf("Enter Member ID: ");
    scanf("%d", &id);

        if (isMemberIDExists(id)) {
        printf(" Member ID already exists. Cannot insert duplicate.\n");
        return;
    }
    printf("Enter Member Name: ");
    scanf(" %[^\n]", name);
    printf("Enter Address: ");
    scanf(" %[^\n]", address);

    struct LibraryMember* newNode = createMemberNode(id, name, address);

    if (memberHead == NULL) {
        memberHead = newNode;
    } else {
        struct LibraryMember* temp = memberHead;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }

    // hashing with linear probing
    int hash = memberHash(id);
    int originalHash = hash;
    while (memberIndex[hash] != NULL) {
        hash = (hash + 1) % MEMBER_HASH_SIZE;
        if (hash == originalHash) {
        printf("Hash table is full. Cannot insert member.\n");
        return;
    }
}
    memberIndex[hash] = newNode;


    printf("Member added successfully.\n");
}

void searchMemberByID() {
    int id;
    printf("\n--- Search Member by ID (Hash Index) ---\n");
    printf("Enter Member ID: ");
    scanf("%d", &id);

  int hash = memberHash(id);
    int originalHash = hash;

    while (memberIndex[hash] != NULL) {
        if (memberIndex[hash]->member_id == id) {
            struct LibraryMember* result = memberIndex[hash];
            printf("Member found: ID: %d | Name: %s | Address: %s\n",
               result->member_id, result->name, result->address);
        return;
    }
    hash = (hash + 1) % MEMBER_HASH_SIZE;
    if (hash == originalHash)
        break;
}
printf("Member not found in hash index.\n");

}

void displayMembers() {
    if (memberHead == NULL) {
        printf("No members found.\n");
        return;
    }

    printf("\n--- Member List ---\n");
    struct LibraryMember* temp = memberHead;
    while (temp != NULL) {
        printf("ID: %d | Name: %s | Address: %s\n", temp->member_id, temp->name, temp->address);
        temp = temp->next;
    }
}


void updateMember() {
    if (memberHead == NULL) {
        printf("No members found.\n");
        return;
    }

    int id;
    printf("\n--- Update Member ---\n");
    printf("Enter Member ID to update: ");
    scanf("%d", &id);

    struct LibraryMember* temp = memberHead;
    while (temp != NULL) {
        if (temp->member_id == id) {
            printf("Current Name: %s | Address: %s\n", temp->name, temp->address);

            char newName[50];
            char newAddress[100];

            printf("Enter New Name: ");
            while ((getchar()) != '\n');
            scanf(" %[^\n]", newName);

            printf("Enter New Address: ");
            scanf(" %[^\n]", newAddress);

            strcpy(temp->name, newName);
            strcpy(temp->address, newAddress);

            printf("Member updated successfully.\n");
            return;
        }
        temp = temp->next;
    }

    printf("Member with ID %d not found.\n", id);
}

void deleteMember() {
    if (memberHead == NULL) {
        printf("No members to delete.\n");
        return;
    }

    int id;
    printf("\n--- Delete Member ---\n");
    printf("Enter Member ID to delete: ");
    scanf("%d", &id);

    struct LibraryMember* temp = memberHead;
    struct LibraryMember* prev = NULL;

    while (temp != NULL) {
        if (temp->member_id == id) {
            if (prev == NULL) {
                memberHead = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            printf("Member deleted successfully.\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    printf("Member with ID %d not found.\n", id);
}


void saveMembersToFile() {
    FILE* file = fopen("members.csv", "w");
    if (file == NULL) {
        printf("Error saving members.\n");
        return;
    }

    // Write header
    fprintf(file, "ID,Name,Address\n");

    struct LibraryMember* temp = memberHead;
    while (temp != NULL) {
        fprintf(file, "%d,%s,%s\n", temp->member_id, temp->name, temp->address);
        temp = temp->next;
    }

    fclose(file);
    printf("Members saved to members.csv successfully.\n");
}

void loadMembersFromFile() {
    FILE* file = fopen("members.csv", "r");
    if (file == NULL) {
        printf("members.csv not found.\n");
        return;
    }

    char line[200];
    int isHeader = 1;

    while (fgets(line, sizeof(line), file)) {
        if (isHeader) {
            isHeader = 0;
            continue;  // Skip header
        }

        int id;
        char name[50];
        char address[100];

        if (sscanf(line, "%d,%49[^,],%99[^\n]", &id, name, address) == 3) {
            struct LibraryMember* newNode = createMemberNode(id, name, address);
            newNode->next = memberHead;
            memberHead = newNode;
            int hash = memberHash(id);
            int originalHash = hash;

        while (memberIndex[hash] != NULL) {
            hash = (hash + 1) % MEMBER_HASH_SIZE;
        if (hash == originalHash) {
            printf("Hash table is full. Cannot insert member.\n");
            break;
    }
}
        } else {
            printf("Skipping malformed line: %s", line);
        }
    }

    fclose(file);
    printf("Members loaded from members.csv successfully.\n");
}

// Library Card
// ------------------------------------------------------------------------

struct LibraryCard {
    int card_id;
    int member_id;
    char issue_date[20];
    char expiry_date[20];
    struct LibraryCard* next;
};

struct LibraryCard* cardHead = NULL;

struct LibraryCard* createCardNode(int card_id, int member_id, char* issue, char* expiry) {
    struct LibraryCard* newNode = (struct LibraryCard*)malloc(sizeof(struct LibraryCard));
    newNode->card_id = card_id;
    newNode->member_id = member_id;
    strcpy(newNode->issue_date, issue);
    strcpy(newNode->expiry_date, expiry);
    newNode->next = NULL;
    return newNode;
}

void insertLibraryCard() {
    int card_id, member_id;
    char issue[20], expiry[20];


    printf("\n--- Insert Library Card ---\n");
    printf("Enter Card ID: ");
    scanf("%d", &card_id);

    // check if library id already exists

    struct LibraryCard* temp = cardHead;
        while (temp != NULL) {
            if (temp->card_id == card_id) {
            printf("Card ID already exists. Library card not added.\n");
            return;
        }
        temp = temp->next;
    }

    printf("Enter Member ID: ");
    scanf("%d", &member_id);


       // check if member_id exists
    struct LibraryMember* memberTemp = memberHead;
    int found = 0;
        while (memberTemp != NULL) {
            if (memberTemp->member_id == member_id) {
            found = 1;
            break;
        }
        memberTemp = memberTemp->next;
    }

        if (!found) {
        printf("Member ID does not exist. Library card not added.\n");
        return;
    }

       // Check if this member already has a card
    temp = cardHead;
    while (temp != NULL) {
        if (temp->member_id == member_id) {
            printf("This member already has a library card. Not allowed to add another.\n");
            return;
        }
        temp = temp->next;
    }


    printf("Enter Issue Date (dd-mm-yyyy): ");
    scanf(" %[^\n]", issue);
    printf("Enter Expiry Date (dd-mm-yyyy): ");
    scanf(" %[^\n]", expiry);

    struct LibraryCard* newNode = createCardNode(card_id, member_id, issue, expiry);

    newNode->next = cardHead;
    cardHead = newNode;

    printf("Library card added successfully.\n");
}

void updateLibraryCard() {
    int card_id;
    char new_issue[20], new_expiry[20];

    printf("\n--- Update Library Card ---\n");
    printf("Enter Card ID to update: ");
    scanf("%d", &card_id);

    struct LibraryCard* temp = cardHead;
    while (temp != NULL) {
        if (temp->card_id == card_id) {
            printf("Enter new Issue Date (dd-mm-yyyy): ");
            scanf(" %[^\n]", new_issue);
            printf("Enter new Expiry Date (dd-mm-yyyy): ");
            scanf(" %[^\n]", new_expiry);

            strcpy(temp->issue_date, new_issue);
            strcpy(temp->expiry_date, new_expiry);

            printf("Library card updated successfully.\n");
            return;
        }
        temp = temp->next;
    }

    printf("Card ID not found.\n");
}

void deleteLibraryCard() {
    int card_id;

    printf("\n--- Delete Library Card ---\n");
    printf("Enter Card ID to delete: ");
    scanf("%d", &card_id);

    struct LibraryCard *temp = cardHead, *prev = NULL;

    while (temp != NULL) {
        if (temp->card_id == card_id) {
            if (prev == NULL) {
                cardHead = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            printf("Library card deleted successfully.\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    printf("Card ID not found.\n");
}




void displayLibraryCards() {
    if (cardHead == NULL) {
        printf("No library cards found.\n");
        return;
    }

    printf("\n--- Library Cards ---\n");
    struct LibraryCard* temp = cardHead;
    while (temp != NULL) {
        printf("Card ID: %d | Member ID: %d | Issue: %s | Expiry: %s\n",
               temp->card_id, temp->member_id, temp->issue_date, temp->expiry_date);
        temp = temp->next;
    }
}



/////////////////////////////////////
// BORROWEDBOOK STRUCT AND LOGIC
/////////////////////////////////////

struct BorrowedBook {
    int member_id;
    int isbn;
    struct BorrowedBook* next;
};

struct BorrowedBook* borrowedHead = NULL;

struct BorrowedBook* createBorrowedNode(int member_id, int isbn) {
    struct BorrowedBook* newNode = (struct BorrowedBook*)malloc(sizeof(struct BorrowedBook));
    newNode->member_id = member_id;
    newNode->isbn = isbn;
    newNode->next = NULL;
    return newNode;
}

int isBookAlreadyBorrowed(int member_id, int isbn) {
    struct BorrowedBook* temp = borrowedHead;
    while (temp != NULL) {
        if (temp->member_id == member_id && temp->isbn == isbn)
            return 1;
        temp = temp->next;
    }
    return 0;
}


void borrowBook() {
    pthread_mutex_lock(&data_lock);  // Lock at the beginning

    int member_id, isbn;
    printf("\n--- Borrow Book ---\n");
    printf("Enter Member ID: ");
    scanf("%d", &member_id);
    printf("Enter ISBN of Book: ");
    scanf("%d", &isbn);

    // Check if member exists
    struct LibraryMember* mptr = memberHead;
    int memberExists = 0;
    while (mptr != NULL) {
        if (mptr->member_id == member_id) {
            memberExists = 1;
            break;
        }
        mptr = mptr->next;
    }

    if (!memberExists) {
        printf("Member ID not found. Cannot borrow.\n");
        pthread_mutex_unlock(&data_lock);  //  unlock before return
        return;
    }

    // Check if book exists
    struct Book* bptr = bookRoot;
    int bookExists = 0;
    while (bptr != NULL) {
        if (bptr->isbn == isbn) {
            bookExists = 1;
            break;
        }
        if (isbn < bptr->isbn)
            bptr = bptr->left;
        else
            bptr = bptr->right;
    }

    if (!bookExists) {
        printf("Book ISBN not found. Cannot borrow.\n");
        pthread_mutex_unlock(&data_lock);  //  unlock before return
        return;
    }

    // Check if book already borrowed by this member
    if (isBookAlreadyBorrowed(member_id, isbn)) {
        printf("This book is already borrowed by the member.\n");
        pthread_mutex_unlock(&data_lock);  //  unlock before return
        return;
    }

    // All good — proceed to borrow
    struct BorrowedBook* newNode = createBorrowedNode(member_id, isbn);
    newNode->next = borrowedHead;
    borrowedHead = newNode;

    printf("Book borrowed successfully.\n");

    pthread_mutex_unlock(&data_lock);  //  Always unlock before exiting
}



void returnBook() {
    pthread_mutex_lock(&data_lock);  //  Lock at the start

    if (borrowedHead == NULL) {
        printf("No borrowed books to return.\n");
        pthread_mutex_unlock(&data_lock);  //  Unlock before return
        return;
    }

    int member_id, isbn;
    printf("\n--- Return Book ---\n");
    printf("Enter Member ID: ");
    scanf("%d", &member_id);
    printf("Enter ISBN of Book to return: ");
    scanf("%d", &isbn);

    struct BorrowedBook* temp = borrowedHead;
    struct BorrowedBook* prev = NULL;

    while (temp != NULL) {
        if (temp->member_id == member_id && temp->isbn == isbn) {
            if (prev == NULL) {
                borrowedHead = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            printf("Book returned successfully.\n");
            pthread_mutex_unlock(&data_lock);  //  Unlock before return
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    printf("No matching borrowed book found.\n");
    pthread_mutex_unlock(&data_lock);  //  Unlock before return
}



void displayBorrowedBooks() {
    if (borrowedHead == NULL) {
        printf("No borrowed books.\n");
        return;
    }

    printf("\n--- Borrowed Books ---\n");
    struct BorrowedBook* temp = borrowedHead;
    while (temp != NULL) {
        printf("Member ID: %d | ISBN: %d\n", temp->member_id, temp->isbn);
        temp = temp->next;
    }
}

void saveBorrowedToFile() {
    FILE* file = fopen("borrowed.csv", "w");
    if (file == NULL) {
        printf("Error saving borrowed books.\n");
        return;
    }

    // Write header
    fprintf(file, "MemberID,ISBN\n");

    struct BorrowedBook* temp = borrowedHead;
    while (temp != NULL) {
        fprintf(file, "%d,%d\n", temp->member_id, temp->isbn);
        temp = temp->next;
    }

    fclose(file);
    printf("Borrowed books saved to borrowed.csv successfully.\n");
}

void loadBorrowedFromFile() {
    FILE* file = fopen("borrowed.csv", "r");
    if (file == NULL) {
        printf("borrowed.csv not found.\n");
        return;
    }

    char line[100];
    int isHeader = 1;

    while (fgets(line, sizeof(line), file)) {
        if (isHeader) {
            isHeader = 0;
            continue; // Skip header
        }

        int member_id, isbn;
        if (sscanf(line, "%d,%d", &member_id, &isbn) == 2) {
            struct BorrowedBook* newNode = createBorrowedNode(member_id, isbn);
            newNode->next = borrowedHead;
            borrowedHead = newNode;
        } else {
            printf("Skipping malformed line: %s", line);
        }
    }

    fclose(file);
    printf("Borrowed books loaded from borrowed.csv successfully.\n");
}

void resetAllData() {
    if (remove("books.csv") == 0)
        printf("books.csv removed successfully.\n");
    else
        printf("books.csv not found or failed to delete.\n");

    if (remove("members.csv") == 0)
        printf("members.csv removed successfully.\n");
    else
        printf("members.csv not found or failed to delete.\n");

    if (remove("borrowed.csv") == 0)
        printf("borrowed.csv removed successfully.\n");
    else
        printf("borrowed.csv not found or failed to delete.\n");

    // Clear in-memory data too
    bookRoot = NULL;
    memberHead = NULL;
    borrowedHead = NULL;

    printf("All data has been reset.\n");
}


//////////////////////////////////
// concurrency test
//////////////////////////////
void* testConcurrentBorrow(void* arg) {
    int* args = (int*)arg;
    int member_id = args[0];
    int isbn = args[1];
    free(arg);  // avoid memory leak

    pthread_mutex_lock(&data_lock);

    struct BorrowedBook* newNode = createBorrowedNode(member_id, isbn);
    newNode->next = borrowedHead;
    borrowedHead = newNode;
    printf("Thread: Member %d borrowed Book %d\n", member_id, isbn);

    pthread_mutex_unlock(&data_lock);
    return NULL;
}

void runConcurrencyTest() {
    printf("\n--- Concurrency Test: Simultaneous Borrowing ---\n");

    pthread_t threads[5];
    for (int i = 0; i < 5; i++) {
        int* args = malloc(2 * sizeof(int));
        args[0] = 101;           // Example member ID
        args[1] = 1000 + i;      // Example book ISBNs (1000, 1001, ...)

        pthread_create(&threads[i], NULL, testConcurrentBorrow, args);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Concurrency test completed.\n");
}




/////////////////////////////////////
// MAIN MENU
/////////////////////////////////////

int main() {
    pthread_mutex_init(&data_lock, NULL);
    int choice;

    loadAllBooks();
    loadMembersFromFile();
    loadBorrowedFromFile();

    do {
        printf("\n==== Library Management System ====\n");

        // Book Management
        printf("\n--- Book Operations ---\n");
        printf("1.  Insert Book\n");
        printf("2.  Display Books\n");
        printf("3.  Delete Book\n");

        // Member Management
        printf("\n--- Member Operations ---\n");
        printf("4.  Insert Member\n");
        printf("5.  Display Members\n");
        printf("6.  Update Member\n");
        printf("7.  Delete Member\n");

        // Library Card Management
        printf("\n--- Library Card Operations ---\n");
        printf("8.  Insert Library Card\n");
        printf("9.  Display Library Cards\n");
        printf("10. Update Library Card\n");
        printf("11. Delete Library Card\n");

        // Borrowing Operations
        printf("\n--- Borrowing & Returning ---\n");
        printf("12. Borrow Book\n");
        printf("13. Return Book\n");
        printf("14. Display Borrowed Books\n");

        // Utilities & Search
        printf("\n--- Utility & Testing ---\n");
        printf("15. Search Member by ID (Index)\n");
        printf("16. Reset All Data\n");
        printf("17. Simulate Borrowing (Concurrency Test)\n");

        // Exit
        printf("\n18. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: insertBook();
                    break;
            case 2: displayBooks();
                    break;
            case 3: deleteBook();
                    break;

            case 4: insertMember();
                    break;
            case 5: displayMembers();
                    break;
            case 6: updateMember();
                    break;
            case 7: deleteMember();
                    break;

            case 8: insertLibraryCard();
                    break;
            case 9: displayLibraryCards();
                    break;
            case 10: updateLibraryCard();
                    break;
            case 11: deleteLibraryCard();
                    break;

            case 12: borrowBook();
                    break;
            case 13: returnBook();
                    break;
            case 14: displayBorrowedBooks();
                    break;

            case 15: searchMemberByID();
                    break;
            case 16: resetAllData();
                    break;
            case 17: runConcurrencyTest();
                    break;

            case 18:
                saveAllBooks();
                saveMembersToFile();
                saveBorrowedToFile();
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 18);

    pthread_mutex_destroy(&data_lock);
   return 0;
}
