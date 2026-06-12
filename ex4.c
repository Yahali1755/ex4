/******************

Name: Yahali Mashiach
ID: 214007346
Assignment: ex4

*******************/

/******************************************
 * EX4 2026B - Paws & Pointers - The Animal Shelter
 *
 * Topics: Linked lists, Binary trees, Generic functions
 *
 * This file is given to you with:
 * - All the structs and enums (you cannot change them).
 * - Working input helpers: readLine() and readInt().
 * - A working main() with the menu loop.
 * - The signatures of all the functions used in the
 * official solution, with empty (TODO) bodies.
 *
 * You do NOT have to use the given functions (except main,
 * the structs and the enums) - as long as the output is
 * identical and the tree stays GENERIC (void* + function
 * pointers).
 ******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================== Enums ==================== */
/* 0=DOG, 1=CAT, 2=RABBIT, 3=PARROT, 4=SNAKE */
typedef enum { DOG, CAT, RABBIT, PARROT, SNAKE } Species;
#define PRE_ORDER 1
#define IN_ORDER 2
#define POST_ORDER 3

/* ==================== Structs ==================== */
/* A node in the waiting queue (linked list). */
typedef struct Visitor {
    char *name;
    struct Visitor *next;
} Visitor;

/* An animal in the shelter catalog. */
typedef struct Animal {
    char *name;
    Species species;
    int age;
} Animal;

/* One adoption: who adopted which animal. */
typedef struct AdoptionRecord {
    char *visitorName;
    char *animalName;
} AdoptionRecord;

/* A node in a generic binary search tree. */
typedef struct BSTNode {
    void *data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

/* A generic BST: the root + the functions that make it generic. */
typedef struct {
    BSTNode *root;
    int (*compare)(const void *, const void *);
    void (*print)(const void *);
    void (*freeData)(void *);
} BST;

// Signatures
char *readLine(void);
int readInt(void);
const char *speciesName(Species species);

void addVisitor(Visitor **head, char *name, int position);
int isInQueue(Visitor *head, const char *name);
int leaveQueue(Visitor **head, const char *name);
Visitor *popFront(Visitor **head);
void printQueue(Visitor *head);
void freeQueue(Visitor *head);

BSTNode* insertBSTNode(BSTNode *root, void *data, int (*compare)(const void *, const void *));
void* findBSTNode(BSTNode *root,  const void *key, int (*compare)(const void *, const void *), int *depth);
BSTNode* findMinNode(BSTNode *node);
BSTNode* deleteBSTNode(BSTNode *root,  const void *key, int (*compare)(const void *, const void *), void (*freeData)(void *));
void bstInsert(BST *tree, void *data);
void *bstFind(BST *tree, const void *key, int *depth);
void bstDelete(BST *tree, const void *key);
void preorder(BSTNode *root, void (*print)(const void *));
void inorder(BSTNode *root, void (*print)(const void *));
void postorder(BSTNode *root, void (*print)(const void *));
void freeBSTNode(BSTNode *root, void (*freeData)(void *));
void bstFree(BST *tree);

int compareAnimals(const void *a, const void *b);
void printAnimal(const void *data);
void freeAnimal(void *data);

int compareRecords(const void *a, const void *b);
void printRecord(const void *data);
void freeRecord(void *data);

void handleAddVisitor(Visitor **queue);
void handleLeaveQueue(Visitor **queue);
void handleAddAnimal(BST *catalog);
void handleAdopt(Visitor **queue, BST *catalog, BST *adoptions);
void handleFindAnimal(BST *catalog);
void handlePrintCatalog(BST *catalog);
void handlePrintAdoptions(BST *adoptions);
void printMenu(void);


/* ==================== Input helpers (given) ==================== */
/* Reads a whole line (until '\n' or EOF) into a dynamically
 * allocated string, without the '\n'. The caller must free it. */
char *readLine(void) {
    int capacity = 16, length = 0, c;

    char *line = (char *)malloc(capacity);

    if (line == NULL) {
        exit(1);
    }

    while ((c = getchar()) != EOF && c != '\n') {
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *bigger = (char *)realloc(line, capacity);
            if (bigger == NULL) {
                free(line);
                exit(1);
            }
            line = bigger;
        }
        line[length++] = (char)c;
    }

    line[length] = '\0';

    return line;
}

/* Reads an integer and consumes the rest of the line, so that a
 * readLine() that comes after it starts on a fresh line.
 * (This is the classic scanf+'\n' trap - here it is solved for you.) */
int readInt(void) {
    int value = 0;

    if (scanf("%d", &value) != 1) {
        return 9; /* malformed input - bail out via Exit */
    }

    getchar(); /* consume the '\n' left by scanf */
    return value;
}

/* Converts a Species value to its printable name (given). */
const char *speciesName(Species species) {
    switch (species) {
        case DOG:    return "DOG";
        case CAT:    return "CAT";
        case RABBIT: return "RABBIT";
        case PARROT: return "PARROT";
        case SNAKE:  return "SNAKE";
    }

    return "";
}

/* ==================== Queue (linked list) ==================== */
/* Inserts a new visitor at the given position (0 = head).
 * A position larger than the queue length means: at the end. */
void addVisitor(Visitor **head, char *name, int position) {
    Visitor* newVisitor = calloc(1, sizeof(Visitor));

    if (newVisitor == NULL) {
        exit(1); 
    }
    newVisitor->name = name;

    // The list is empty, or they want to be first in line (position 0) */
    if (*head == NULL || position == 0) {
        newVisitor->next = *head; 
        *head = newVisitor;    
        return;
    }

    Visitor* current = *head;
    int currentPositon = 0;

    /* Iterate forward until we hit the node exactly one spot before our target, 
       or till the end of the list */
    while (current->next != NULL && currentPositon < position - 1) {
        current = current->next;
        currentPositon++;
    }

    newVisitor->next = current->next;
    current->next = newVisitor;
}

/* Returns 1 if a visitor with this name is in the queue, else 0. */
int isInQueue(Visitor *head, const char *name) {
    while (head != NULL) {
        if (!strcmp(head->name, name)) {
            return 1;
        }

        head = head->next;
    }

    return 0;
}

/* Removes the visitor with the given name (frees it).
 * Returns 1 on success, 0 if no such visitor. */
int leaveQueue(Visitor **head, const char *name) {
    // if no such visitor return 0
    if (!isInQueue(*head, name)) {
        return 0;
    }

    Visitor* iterator = *head;
    Visitor* prev = NULL;

    while (strcmp(iterator->name, name) != 0) {
        prev = iterator;
        iterator = iterator->next;
    }

    if (prev == NULL) {
        // If they were first in line
        *head = iterator->next;
    } else {
        // If they were in the middle
        prev->next = iterator->next; 
    }

    free(iterator->name);
    free(iterator);

    return 1;
}

/* Detaches the FIRST visitor from the queue and returns it
 * (does not free it). Returns NULL on an empty queue. */
Visitor *popFront(Visitor **head) {
    if (*head == NULL) {
        return NULL;
    }

    Visitor* tmp = *head;

    // move the queue one visitor ahead detachging the first one
    *head = (*head)->next;

    tmp->next = NULL;

    return tmp;
}

/* Prints the queue: "Alice -> Bob -> Charlie"
 * or "The queue is empty." */
void printQueue(Visitor *head) {
    if (head == NULL) {
        printf("The queue is empty.\n");

        return;
    }

    while (head != NULL) {
        printf("%s", head->name);

        if (head->next != NULL) {
            printf(" -> ");
        }

        head = head->next;
    }

    printf("\n");
}

/* Frees the whole queue. */
void freeQueue(Visitor *head) {
    if (head == NULL) {
        return;
    }

    while (head != NULL) {
        Visitor* tmp = head;

        head = head->next;
        free(tmp->name);
        free(tmp);
    }
}

/* ==================== Generic BST ==================== */

/* Helper function to recursively find the insertion point and link the new node */
BSTNode* insertBSTNode(BSTNode *root, void *data, int (*compare)(const void *, const void *)) {
    if (root == NULL) {
        BSTNode *newNode = calloc(1, sizeof(BSTNode));

        if (newNode == NULL) {
            exit(1); 
        }
    
        newNode->data = data;

        return newNode; 
    }

    if (compare(data, root->data) < 0) {
        root->left = insertBSTNode(root->left, data, compare);
    } else {
        root->right = insertBSTNode(root->right, data, compare);
    }

    return root;
}

/* Inserts data into the tree using tree->compare.
 * You may assume no duplicates are ever inserted. */
void bstInsert(BST *tree, void *data) {
    tree->root = insertBSTNode(tree->root, data, tree->compare);
}

/* Helper function to recursively find the bst node and its depth */
void* findBSTNode(BSTNode *root, const void *key, int (*compare)(const void *, const void *), int *depth) {
    if (root == NULL) {
        return NULL; 
    }

    int compareValue = compare(key, root->data);

    if (compareValue == 0) {
        return root->data;
    }
    else if (compareValue < 0) {
        if (depth != NULL) {
            (*depth)++;
        }

        return findBSTNode(root->left, key, compare, depth);
    } else {
        if (depth != NULL) {
            (*depth)++;
        }

        return findBSTNode(root->right, key, compare, depth);
    }
}

/* Searches for key using tree->compare. On success returns the
 * stored data, and if depth != NULL writes the depth (root = 0)
 * into *depth. Returns NULL when not found. */
void *bstFind(BST *tree, const void *key, int *depth) {
        /* Initialize the depth to 0 at the root */
    if (depth != NULL) {
        *depth = 0;
    }

    return findBSTNode(tree->root, key, tree->compare, depth);
}

/* Helper to find the minimum node in a subtree */
BSTNode* findMinNode(BSTNode *node) {
    BSTNode *current = node;
    
    /* Loop down to find the leftmost leaf */
    while (current != NULL && current->left != NULL) {
        current = current->left;
    }
    
    return current;
}

/* Helper function to recursively delete the bst node  */
BSTNode* deleteBSTNode(BSTNode *root, const void *key, int (*compare)(const void *, const void *), void (*freeData)(void *)) {
    if (root == NULL) {
        return NULL; 
    }

    int compareValue = compare(key, root->data);

    // if value is 0 then we found the node to delete
    if (compareValue == 0) {
        // no children
        if (root->left == NULL && root->right == NULL) {
            free(root);

            return NULL;
        }

        //  only right child
        if (root->left != NULL && root->right == NULL) {
            BSTNode* left = root->left;
            free(root);

            return left;
        }

        //  only left child
        if (root->left == NULL && root->right != NULL) {
            BSTNode* right = root->right;
            free(root);

            return right;
        }

        // two children
        if (root->left != NULL && root->right != NULL) {
            /* Find the successor*/
            BSTNode *successor = findMinNode(root->right);

            /* Copy the successor's data into the current node. */
            root->data = successor->data;

            /* Recursively delete the successor from the right subtree */
            root->right = deleteBSTNode(root->right, successor->data, compare, freeData);
        }
    }
    else if (compareValue < 0) {
        root->left = deleteBSTNode(root->left, key, compare, freeData);
    } else {
        root->right = deleteBSTNode(root->right, key, compare, freeData);
    }

    return root;
}

/* Deletes the node holding key (which must exist) from the tree.
 * The node is freed, the data itself is NOT freed.
 * Two-children rule: replace with the in-order successor
 * (the minimum of the right subtree). */
void bstDelete(BST *tree, const void *key) {
    tree->root = deleteBSTNode(tree->root, key, tree->compare, tree->freeData);
}

/* The three classic traversals - call print on every data. */
void preorder(BSTNode *root, void (*print)(const void *)) {
    if (root == NULL) 
        return;

    print(root->data);
    preorder(root->left, print);
    preorder(root->right, print);
}

void inorder(BSTNode *root, void (*print)(const void *)) {
    if (root == NULL) 
        return;

    preorder(root->left, print);
    print(root->data);
    preorder(root->right, print);
}

void postorder(BSTNode *root, void (*print)(const void *)) {
    if (root == NULL) 
        return;
    preorder(root->left, print);
    preorder(root->right, print);
    print(root->data);
}

/* Helper function to recursively free nodes and their data */
void freeBSTNode(BSTNode *root, void (*freeData)(void *)) {
    if (root == NULL) {
        return;
    }

    // free the left subtree
    freeBSTNode(root->left, freeData);

    // free the right subtree
    freeBSTNode(root->right, freeData);

    if (root->data != NULL) {
        freeData(root->data);
    }

    // Free the node itself
    free(root);
}

/* Frees all the nodes AND their data (using tree->freeData). */
void bstFree(BST *tree) {
    if (tree == NULL) {
        return;
    }

    freeBSTNode(tree->root, tree->freeData);

    // reset the root pointer after destroying the tree
    tree->root = NULL;
}

/* ==================== Animal callbacks ==================== */
/* Animals are ordered by name only (strcmp). */
int compareAnimals(const void *a, const void *b) {
    const Animal* animalA = (const Animal*)a;
    const Animal* animalB = (const Animal*)b;

    return strcmp(animalA->name, animalB->name);
}

/* Prints: "[SPECIES] Name - Age: X\n", e.g. "[DOG] Rex - Age: 3" */
void printAnimal(const void *data) {
    const Animal* animal = (const Animal*)data;
    
    printf("[%s] %s - Age: %d\n", speciesName(animal->species), animal->name, animal->age);
}

void freeAnimal(void *data) {
    Animal* animal = (Animal*)data;

    free(animal->name);
    free(animal);
}

/* ================ AdoptionRecord callbacks ================ */
/* Records are ordered by visitor name, then by animal name. */
int compareRecords(const void *a, const void *b) {
    const AdoptionRecord* recordA = (const AdoptionRecord*)a;
    const AdoptionRecord* recordB = (const AdoptionRecord*)b;

    int compareNameValue = strcmp(recordA->visitorName, recordB->visitorName);

    if (compareNameValue != 0) {
        return compareNameValue;
    }

    return strcmp(recordA->animalName, recordB->animalName);
}

/* Prints: "Visitor adopted Animal!\n", e.g. "Alice adopted Rex!" */
void printRecord(const void *data) {
    const AdoptionRecord* record = (AdoptionRecord*)data;
    
    printf("%s adopted %s!\n", record->visitorName, record->animalName);
}

void freeRecord(void *data) {
    AdoptionRecord* record = (AdoptionRecord*)data;

    free(record->visitorName);
    free(record->animalName);
    free(record);
}

/* ==================== Menu handlers ==================== */
void handleAddVisitor(Visitor **queue) {
    printf("Enter the name of the visitor:\n");
    char *name = readLine();

    if (isInQueue(*queue, name)) {
        printf("Visitor already in queue.\n");

        /* free the name since it shouldnt be added, visitor wont be added*/
        free(name); 
        return;
    }

    printf("Enter the position:\n");
    int position = readInt();

    addVisitor(queue, name, position);
}

void handleLeaveQueue(Visitor **queue) {
    printf("Enter the name of the visitor:\n");
    char *name = readLine();

    if (!leaveQueue(queue, name)) {
        printf("Visitor not found.\n");
    }

    // free the name we just allocated
    free(name);
}

void handleAddAnimal(BST *catalog) {
    printf("Enter the name of the animal:\n");
    char *name = readLine();

    /* Temporary dummy struct for the search */
    Animal dummySearchAnimal;
    dummySearchAnimal.name = name;

    Animal* animal = (Animal*)bstFind(catalog, &dummySearchAnimal, NULL);

    if (animal) {
        printf("Animal already exists.\n");
        free(name);
        
        return;
    }

    printf("Enter the species (0-4):\n");
    int species = readInt();

    printf("Enter the age:\n");
    int age = readInt();

    animal = malloc(sizeof(Animal));
    
    if (animal == NULL) {
        exit(1);
    }

    animal->name = name;
    animal->species = (Species)species;
    animal->age = age;

    bstInsert(catalog, animal);
}

void handleAdopt(Visitor **queue, BST *catalog, BST *adoptions) {
    if (*queue == NULL) {
        printf("No visitors in queue.\n");
        return; 
    }

    printf("Enter the name of the animal:\n");
    char *name = readLine();

    /* Temporary dummy struct for the search */
    Animal dummySearchAnimal;
    dummySearchAnimal.name = name;

    Animal* animal = (Animal*)bstFind(catalog, &dummySearchAnimal, NULL);

    if (animal == NULL) {
        printf("Animal not found.\n");
        free(name);
        return;
    }

    bstDelete(catalog, &dummySearchAnimal);

    Visitor* firstVisitor = popFront(queue);
    AdoptionRecord* record = malloc(sizeof(AdoptionRecord));

    if (record == NULL) {
        exit(1);
    }

    record->visitorName = firstVisitor->name;
    record->animalName = animal->name;

    bstInsert(adoptions, record);

    adoptions->print(record);
    free(name);
    // free visitor and animal without freeing their names, because of adoption record
    free(firstVisitor);
    free(animal);
}

void handleFindAnimal(BST *catalog) {
    printf("Enter the name of the animal:\n");
    char *name = readLine();
    int depth = 0;

    /* Temporary dummy struct for the search */
    Animal dummySearchAnimal;
    dummySearchAnimal.name = name;

    Animal* animal = (Animal*)bstFind(catalog, &dummySearchAnimal, &depth);
     

    if (animal == NULL) {
        printf("Animal not found.\n");
    } else {
        printf("Found at depth %d: ", depth);
        catalog->print(animal);
    }

    free(name);
}

void handlePrintCatalog(BST *catalog) {
    if (catalog->root == NULL) {
        printf("The catalog is empty.\n");

        return; 
    }

    printf("Choose an option:\n"
           "1. Preorder\n"
           "2. Inorder\n"
           "3. Postorder\n");

    int choice = readInt();

    switch (choice) {
        case PRE_ORDER:
            preorder(catalog->root, catalog->print);
            break;
        case IN_ORDER:
            inorder(catalog->root, catalog->print);
            break;
        case POST_ORDER:
            postorder(catalog->root, catalog->print);
            break;
        default:
            break;
    }
}

void handlePrintAdoptions(BST *adoptions) {
    if (adoptions->root == NULL) {
        printf("No adoptions yet.\n");

        return; 
    }

    inorder(adoptions->root, adoptions->print);
}

/* ==================== Main (given) ==================== */
void printMenu(void) {
    printf("Choose an option:\n"
           "1. Add a visitor\n"
           "2. Leave the queue\n"
           "3. Print the queue\n"
           "4. Add an animal\n"
           "5. Adopt an animal\n"
           "6. Find an animal\n"
           "7. Print the catalog\n"
           "8. Print the adoptions\n"
           "9. Exit\n");
}

int main(void) {
    Visitor *queue = NULL;
    BST catalog = { NULL, compareAnimals, printAnimal, freeAnimal };
    BST adoptions = { NULL, compareRecords, printRecord, freeRecord };

    while (1) {
        printMenu();
        int choice = readInt();
        if (choice == 1) {
            handleAddVisitor(&queue);
        } else if (choice == 2) {
            handleLeaveQueue(&queue);
        } else if (choice == 3) {
            printQueue(queue);
        } else if (choice == 4) {
            handleAddAnimal(&catalog);
        } else if (choice == 5) {
            handleAdopt(&queue, &catalog, &adoptions);
        } else if (choice == 6) {
            handleFindAnimal(&catalog);
        } else if (choice == 7) {
            handlePrintCatalog(&catalog);
        } else if (choice == 8) {
            handlePrintAdoptions(&adoptions);
        } else {
            printf("Goodbye!\n");
            break;
        }
    }

    freeQueue(queue);
    bstFree(&catalog);
    bstFree(&adoptions);
    return 0;
}