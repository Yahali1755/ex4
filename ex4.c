#include <stdio.h>
#include <string.h>


/***************************
******** Menu Items ********
****************************/

#define REVERSE 1
#define PALINDROME 2
#define SENTENCES 3
#define ZIP 4
#define SUDOKU 5
#define EXIT 6


/***************************
*** DIMENSION PARAMETERS ***
****************************/

#include <stdio.h>
#include <string.h>


/***************************
******** Menu Items ********
****************************/

#define REVERSE 1
#define PALINDROME 2
#define SENTENCES 3
#define ZIP 4
#define SUDOKU 5
#define EXIT 6


/***************************
*** DIMENSION PARAMETERS ***
****************************/

#define LONGEST_TERM 20
#define LONGEST_SENTENCE 62
#define MAX_NUMBER_OF_TERMS 10

#define ZIP_MAX_GRID_SIZE 20

#define SUDOKU_GRID_SIZE 9
#define SUDOKU_SUBGRID_SIZE 3


/***************************
* USER INTEFACE PROTOTYPES *
****************************/

void task1ReversePhrase();
void task2CheckPalindrome();
void task3GenerateSentences();
void task4SolveZipBoard();
void task5SolveSudoku();


/****************************
* IMPLEMENTATION PROTOTYPES *
*****************************/

void task1ReversePhraseImplementation();
int task2CheckPalindromeImplementation(int);
void task3GenerateSentencesImplementation(char[][LONGEST_TERM+1], int, char[][LONGEST_TERM+1], int,
                                            char[][LONGEST_TERM+1], int);
int task4SolveZipBoardImplementation(int[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
                                        char[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE], int, int, int, int);
int task5SolveSudokuImplementation(int[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE]);


/******************************
* HELPER FUNCTIONS PROTOTYPES *
*******************************/

int readTerms(char[][LONGEST_TERM+1], int, char[]);
void printSudoku(int[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE]);

void generateObjectsHelper(char subjects[][LONGEST_TERM + 1], char verbs[][LONGEST_TERM + 1], 
    char objects[][LONGEST_TERM + 1], int subjectIndex, int verbIndex, 
    int objectsCount, int objectIndex, int lineIndex[]);

void generateVerbsHelper(char subjects[][LONGEST_TERM + 1], char verbs[][LONGEST_TERM + 1], 
    char objects[][LONGEST_TERM + 1], int subjectIndex, 
    int verbsCount, int objectsCount, int verbIndex, int lineIndex[]);

void generateSubjectsHelper(char subjects[][LONGEST_TERM + 1], char verbs[][LONGEST_TERM + 1], 
    char objects[][LONGEST_TERM + 1], int subjectsObject, int verbsCount, int objectsCount, int subjectIndex, 
    int lineIndex[]);

int solveZipBoardHelper(int board[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE], 
    char solution[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE], int size, int currentRow, int currentColumn,
     int nextTileNumberToFind, int stepCount, int highest);

int checkRowRecursive(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE], int row, int number, int columnIndex);

int checkColumnRecursive(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE], int column, int number, int rowIndex);

int checkSquareRecursive(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE], int squareStartRow, int squareStartColumn,
     int number, int squareIndex);

int isValidCell(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE], int row, int column, int number);

int tryNumbersForCellRecursive(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE], int row, int column, int numberToTry);

int solveSudokuHelper(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE], int row, int column);

/******************************
********** MAIN MENU **********
*******************************/

int main()
{
    int task;
    do
    {
        printf("Please choose a task (1-5) or 6 to exit:\n");
        printf("%d. Reverse a phrase\n", REVERSE);
        printf("%d. Check Palindrome\n", PALINDROME);
        printf("%d. Generate sentences\n", SENTENCES);
        printf("%d. Solve Zip Board\n", ZIP);
        printf("%d. Solve Sudoku\n", SUDOKU);
        printf("%d. Exit\n", EXIT);

        scanf(" %d", &task);
        scanf("%*c");
        switch (task)
        {
        case REVERSE:
            task1ReversePhrase();
            break;
        case PALINDROME:
            task2CheckPalindrome();
            break;
        case SENTENCES:
            task3GenerateSentences();
            break;
        case ZIP:
            task4SolveZipBoard();
            break;
        case SUDOKU:
            task5SolveSudoku();
            break;
        case EXIT:
            printf("Goodbye!\n");
            break;
        default:
            printf("Please choose a task number from the list.\n");
            break;
        }
        printf("\n=============================\n\n");
    } while (task != EXIT);

    return 0;
}


/***************************
****** USER INTERFACE ******
****************************/


void task1ReversePhrase()
{
    printf("Please insert the phrase to reverse:\n");
    task1ReversePhraseImplementation();

    printf("\n");
}


void task2CheckPalindrome()
{
    printf("Please insert the phrase length:\n");
    int n;
    scanf("%d", &n);
    printf("Please insert the phrase to check:\n");
    scanf(" ");
    if (task2CheckPalindromeImplementation(n))
        printf("The phrase is a palindrome.\n");
    else
        printf("The phrase is not a palindrome.\n");
}


void task3GenerateSentences()
{
    char subjects[MAX_NUMBER_OF_TERMS][LONGEST_TERM+1];
    char verbs[MAX_NUMBER_OF_TERMS][LONGEST_TERM+1];
    char objects[MAX_NUMBER_OF_TERMS][LONGEST_TERM+1];
    int subjectsCount, verbsCount, objectsCount;

    subjectsCount=readTerms(subjects, MAX_NUMBER_OF_TERMS, "subjects");
    verbsCount=readTerms(verbs, MAX_NUMBER_OF_TERMS, "verbs");
    objectsCount=readTerms(objects, MAX_NUMBER_OF_TERMS, "objects");
    printf("List of Sentences:\n");
    task3GenerateSentencesImplementation(subjects, subjectsCount, verbs, verbsCount, objects, objectsCount);
}


void task4SolveZipBoard()
{
    int size;
    int board[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE] = {0};
    char solution[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE] = {0};
    int row, col;
    int highest=0;
    printf("Please enter the board size:\n");
    scanf("%d", &size);
    if (size < 1 || size > ZIP_MAX_GRID_SIZE)
    {
        printf("Invalid board size.\n");
        return;
    }

    printf("Please enter the grid:\n");
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            scanf("%d", &board[i][j]);
            if (board[i][j] == 1)
            {
                row = i;
                col = j;
            }
            if(board[i][j]>highest){
                highest=board[i][j];
            }
        }
    }
    if (task4SolveZipBoardImplementation(board, solution, size, row, col, highest))
    {
        printf("Solution:\n");
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                printf("%c ", solution[i][j] ? solution[i][j] : 'X');
            }
            printf("\n");
        }
    }
    else
    {
        printf("No solution exists.\n");
    }
}


void task5SolveSudoku() {
    printf("Please enter the sudoku board:\n");
    int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE] = {0};
    for (int i = 0; i < SUDOKU_GRID_SIZE; i++)
    {
        for (int j = 0; j < SUDOKU_GRID_SIZE; j++)
        {
            scanf("%d", &board[i][j]);
        }
    }
    if (task5SolveSudokuImplementation(board))
    {
        printf("Solution found:\n");
        printSudoku(board);
    }
    else
    {
        printf("No solution exists.\n");
    }
}


/***************************
********* HELPERS **********
****************************/

int readTerms(char terms[][LONGEST_TERM+1], int maxNumOfTerms, char type[]){
    int termsCount;
    printf("Please insert number of %s:\n", type);
    scanf("%d", &termsCount);
    if(termsCount < 1 || termsCount > maxNumOfTerms){
        termsCount = maxNumOfTerms;
    }
    printf("Please insert the list of %s:\n", type);
    for(int i=0;i<termsCount;i++){
        printf("%d. ",i+1);
        scanf(" %[^\n]",terms[i]);
    }
    return termsCount;
}


void printSudoku(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE])
{
    printf("+-------+-------+-------+\n");
    for (int i = 0; i < SUDOKU_GRID_SIZE; i++)
    {
        printf("| ");
        for (int j = 0; j < SUDOKU_GRID_SIZE; j++)
        {
            if (board[i][j] == 0)
                printf(". ");
            printf("%d ", board[i][j]);

            if ((j + 1) % SUDOKU_SUBGRID_SIZE == 0)
                printf("| ");
        }
        printf("\n");
        if ((i + 1) % SUDOKU_SUBGRID_SIZE == 0)
            printf("+-------+-------+-------+\n");
    }
}

void generateObjectsHelper(char subjects[][LONGEST_TERM + 1], char verbs[][LONGEST_TERM + 1], 
    char objects[][LONGEST_TERM + 1], int subjectIndex, int verbIndex, 
    int objectsCount, int objectIndex, int lineIndex[]) {
        if (objectIndex == objectsCount) {
            return;
        }

        printf("%d. %s %s %s\n", lineIndex[0], subjects[subjectIndex], verbs[verbIndex], objects[objectIndex]);
        lineIndex[0]++;

        generateObjectsHelper(subjects, verbs, objects, subjectIndex, verbIndex, objectsCount, 
            objectIndex + 1, lineIndex);
    }

void generateVerbsHelper(char subjects[][LONGEST_TERM + 1], char verbs[][LONGEST_TERM + 1], 
    char objects[][LONGEST_TERM + 1], int subjectIndex, int verbsCount, int objectsCount,
     int verbIndex, int lineIndex[]) {
    if (verbIndex == verbsCount) {
        return;
    }

    generateObjectsHelper(subjects, verbs, objects, subjectIndex, verbIndex, objectsCount, 
        0, lineIndex);

    generateVerbsHelper(subjects, verbs, objects, subjectIndex, verbsCount, objectsCount, 
        verbIndex + 1, lineIndex);
}

void generateSubjectsHelper(char subjects[][LONGEST_TERM + 1], char verbs[][LONGEST_TERM + 1], 
    char objects[][LONGEST_TERM + 1], int subjectsCount, int verbsCount, int objectsCount, int subjectIndex, 
    int lineIndex[]) {
    if (subjectIndex == subjectsCount) {
        return;
    }

    generateVerbsHelper(subjects, verbs, objects, subjectIndex, verbsCount, objectsCount, 
        0, lineIndex);

    generateSubjectsHelper(subjects, verbs, objects, subjectsCount, verbsCount, objectsCount, 
        subjectIndex + 1, lineIndex);
}

int solveZipBoardHelper(int board[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE], 
    char solution[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE], int size, int currentRow, int currentColumn,
    int nextTileNumberToFind, int stepCount, int highest) {

    if (currentRow < 0 || currentColumn < 0 || currentColumn == size || currentRow == size) {
        return 0;
    }

    // Return 0 if cell was already used in solution
    if (solution[currentRow][currentColumn] != 0) {
        return 0;
    }              

    if (board[currentRow][currentColumn] != 0) {
        // if the cell is not 0 but is not the next tile then this path is invalid
        if (board[currentRow][currentColumn] != nextTileNumberToFind) {
            return 0; 
        }

        if (board[currentRow][currentColumn] == highest) {
            if (stepCount == size * size) {
                solution[currentRow][currentColumn] = 'X';

                return 1;
            }

            return 0;
        }
        
        nextTileNumberToFind++; 
    }

    // Temporally set the cell to 1 so its "used" then neighbors wont cause infinite recursion
    solution[currentRow][currentColumn] = 1;

    // Up
    if (solveZipBoardHelper(board, solution, size, currentRow - 1, currentColumn,
         nextTileNumberToFind, stepCount + 1, highest)) {
        solution[currentRow][currentColumn] = 'U';

        return 1; 
    }

    // Down
    if (solveZipBoardHelper(board, solution, size, currentRow + 1, currentColumn,
         nextTileNumberToFind, stepCount + 1, highest)) {
        solution[currentRow][currentColumn] = 'D';

        return 1; 
    }

    // Left
    if (solveZipBoardHelper(board, solution, size, currentRow, currentColumn - 1,
         nextTileNumberToFind, stepCount + 1, highest)) {
        solution[currentRow][currentColumn] = 'L';

        return 1; 
    }

    // Right
    if (solveZipBoardHelper(board, solution, size, currentRow, currentColumn + 1,
         nextTileNumberToFind, stepCount + 1, highest)) {
        solution[currentRow][currentColumn] = 'R';

        return 1; 
    }

    // If no solution was found for current path reset cell and return 0
    solution[currentRow][currentColumn] = 0;
    
    return 0;
}

int checkRowRecursive(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE], int row, int number, int columnIndex) {
    if (columnIndex >= SUDOKU_GRID_SIZE) {
        return 1;
    } 

    if (board[row][columnIndex] == number) {
        return 0;
    }

    return checkRowRecursive(board, row, number, columnIndex + 1);
}

int checkColumnRecursive(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE], int column, int number, int rowIndex) {
    if (rowIndex >= SUDOKU_GRID_SIZE) {
        return 1;
    } 

    if (board[rowIndex][column] == number) {
        return 0;
    }

    return checkColumnRecursive(board, column, number, rowIndex + 1);
};

int checkSquareRecursive(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE], int squareStartRow, 
    int squareStartColumn, int number, int squareIndex) {
    if (squareIndex >= SUDOKU_GRID_SIZE) {
        return 1;
    } 

    int currentRow = squareStartRow + (squareIndex / 3);
    int currentColumn = squareStartColumn + (squareIndex % 3);

    if (board[currentRow][currentColumn] == number) {
        return 0;
    }

    return checkSquareRecursive(board, squareStartRow, squareStartColumn, number, squareIndex + 1);
}

int isValidCell(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE], int row, int column, int number) {
    // Start from the top left cell of each square
    int squareStartRow = (row / 3) * 3;
    int squareStartColumn = (column / 3) * 3;

    return checkRowRecursive(board, row, number, 0) && checkColumnRecursive(board, column, number, 0) &&
        checkSquareRecursive(board, squareStartRow, squareStartColumn, number, 0);
}

int tryNumbersForCellRecursive(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE], int row, int column, 
    int numberToTry) {
    if (numberToTry > 9) {
        return 0;
    }

    if (isValidCell(board, row, column, numberToTry)) {
        // Try adding the number to the cell
        board[row][column] = numberToTry;

        if (solveSudokuHelper(board, row, column)) {
            return 1;
        }

        // If adding the cell didnt make a valid solution then adding this number causes a wrong solution, a dead end so it should be reseted
        board[row][column] = 0;
    }

    return tryNumbersForCellRecursive(board, row, column, numberToTry + 1);
}

int solveSudokuHelper(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE], int row, int column) {
    if (row == SUDOKU_GRID_SIZE) {
        return 1;
    }

    if (column >= SUDOKU_GRID_SIZE) {
        return solveSudokuHelper(board, row + 1, 0);
    }

    int nextRow = (column == 8) ? row + 1 : row;
    int nextColumn = (column == 8) ? 0 : column + 1;

    if (board[row][column] != 0) {
        return solveSudokuHelper(board, nextRow, nextColumn);
    }

    return tryNumbersForCellRecursive(board, row, column, 1);
}

/***************************
*********** TODO ***********
****************************/

void task1ReversePhraseImplementation(){
    char character;

    scanf("%c", &character);

    if (character != '\n') {
        task1ReversePhraseImplementation();

        printf("%c", character);
    } else {
       printf("The reversed phrase is:\n");
    }
}


int task2CheckPalindromeImplementation(int length) {
    if (length == 1) {
        // Clear buffer when length is 1, always a palindrom
        char redundantChar;
        scanf("%c", &redundantChar); 

        return 1;
    }

    if (length == 0) return 1;

    char firstCharacter;
    scanf("%c", &firstCharacter);

    // Get the first character, then recursively check the phrase excluding first and last character
    int isInnerCharactersInPhrasePalindrom = task2CheckPalindromeImplementation(length - 2);

    // After recursively checking the middle of the phrase, compare first and last character each iteration
    char lastCharacter;
    scanf("%c", &lastCharacter);

    if (isInnerCharactersInPhrasePalindrom == 1 && firstCharacter == lastCharacter) {
        return 1;
    }
        
    return 0;
}


void task3GenerateSentencesImplementation(char subjects[][LONGEST_TERM+1], int subjectsCount,
                                            char verbs[][LONGEST_TERM+1], int verbsCount,
                                            char objects[][LONGEST_TERM+1], int objectsCount){
    // counter for the printed line
    int lineIndex[1] = {1};

    generateSubjectsHelper(subjects, verbs, objects, subjectsCount, verbsCount, objectsCount, 0, lineIndex);
}


int task4SolveZipBoardImplementation(int board[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
                                    char solution[ZIP_MAX_GRID_SIZE][ZIP_MAX_GRID_SIZE],
                                    int size, int startR, int startC, int highest) {
    return solveZipBoardHelper(board, solution, size, startR, startC, 1, 1, highest);
}


int task5SolveSudokuImplementation(int board[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE]) {
    return solveSudokuHelper(board, 0, 0);
}