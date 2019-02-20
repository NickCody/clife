#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ALIVE '0'
#define DEAD '.'
#define OUTSIDE 'x'

#define NUM_ROWS 10
#define NUM_COLS 10
#define NUM_GENERATIONS 100
#define MAX_MESSAGE_LENGTH 256

#define clear() printf("\033[H\033[J")

// Board Structure
//
// char* = column (array of cells, aka column)
// char** array of columns (array of columns, aka board)
// char*** array of rows (array of boards)
//
// first index = row
// second index = col
// board[0] = [xxxxxxxx]
// board[1] = [xxxxxxxx]
// ..
// board[n] = [xxxxxxxx]
//

char** create_board();
void free_board(char** board);

// Operations on boards
void set_cell(char** board, char setchar, int row, int col);
void clear_board(char** board, char clearchar);
void print_board(char** board, char* message);
void calc_generation(char** nextGen, char** currentGen);
void copy_board(char** src, char** dest);
void invert_board(char** board);
int check_board(char** board);
char cell_next(char** board, int row, int col);
int num_neighbors(char** board, int row, int col);
void read_file(char* filename, char** board);
int compare_boards(char** a, char** b);

int main(int argc, char** argv) {

    char message[MAX_MESSAGE_LENGTH];

    // Create all boards we need
    char*** allBoards = (char***)malloc(NUM_GENERATIONS * sizeof(char**));
    for (int b=0; b < NUM_GENERATIONS; b++) {
        allBoards[b] = create_board();
    }

    char** currentGen=allBoards[0];
    read_file("initial.txt", currentGen);
    print_board(currentGen, "Initial generation");

    // Start at 1, since 0 was set speciallt
    for (int i=1; i < NUM_GENERATIONS; i++ ) {

        calc_generation(currentGen, allBoards[i]);

        clear();
        sprintf(message, "Generation %d", i);
        print_board(allBoards[i], message);

        if (compare_boards(currentGen, allBoards[i]) == 0) {
            printf("Next generation identical, aborting!\n");
            exit(0);
        }

        // Make the new board currentGen
        currentGen = allBoards[i];

        if (!check_board(currentGen)) {
            printf("All dead, aborting!\n");
            break;
        }

        sleep(1);
    }

    // Free all boards
    for (int b=0; b < NUM_GENERATIONS; b++) {
        free_board(allBoards[b]);
    }

    return 0;
}

char** create_board() {
    char** board = (char**)malloc(NUM_ROWS * sizeof(char*)); // size of char* (array)
    for(int i = 0; i < NUM_ROWS; i++) {
        board[i] = (char*)malloc(NUM_COLS * sizeof(char)); // size of single char
    }

    clear_board(board, DEAD);

    return board;
}

void set_cell(char** board, char setchar, int row, int col) {
    if (row < 0 || row >= NUM_ROWS || col < 0 || col > NUM_COLS) {
        printf("Error: invalid coordinate specified (%d,%d)", row, col);
        exit(1);
    }

    board[row][col] = setchar;
}

void clear_board(char** board, char clearchar) {
    for(int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            board[i][j] = clearchar;
        }
    }
}

void free_board(char** board) {
    for (int i=0; i < NUM_ROWS; i++) {
        free(board[i]);
    }

    free (board);
}

void print_board(char** board, char* message) {
    printf("%s (%dx%d)\n", message, NUM_ROWS, NUM_COLS);
    for(int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

void copy_board(char** src, char** dest) {
    for(int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

void invert_board(char** board) {
    for(int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (board[i][j] == ALIVE)
                board[i][j] = DEAD;
            else
                board[i][j] = ALIVE;
        }
    }
}

int check_board(char** board) {
    for(int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (board[i][j] == ALIVE)
                return 1;
        }
    }

    return 0;
}

char get_cell(char** board, int row, int col) {
    if (row < 0 || row >= NUM_ROWS || col < 0 || col >= NUM_COLS)
        return OUTSIDE;
    else
        return board[row][col];
}

int num_neighbors(char** board, int row, int col) {
    int count = 0;

    if (get_cell(board, row-1, col-1) == ALIVE)
        count++;
    if (get_cell(board, row-1, col) == ALIVE)
        count++;
    if (get_cell(board, row-1, col+1) == ALIVE)
        count++;
    if (get_cell(board, row, col-1) == ALIVE)
        count++;
    if (get_cell(board, row, col+1) == ALIVE)
        count++;
    if (get_cell(board, row+1, col-1) == ALIVE)
        count++;
    if (get_cell(board, row+1, col) == ALIVE)
        count++;
    if (get_cell(board, row+1, col+1) == ALIVE)
        count++;

    return count;
}

char cell_next(char** board, int row, int col) {
    int n = num_neighbors(board, row, col);
    char cell = get_cell(board, row, col);
    if (cell == ALIVE) {
        if (n == 2 || n == 3)
            return ALIVE;
        else
            return DEAD;

    } else if (cell == DEAD) {
        if (n == 3)
            return ALIVE;
        else
            return DEAD;
    }

    return OUTSIDE;
}

void calc_generation(char** currentGen, char** nextGen) {
    for(int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            nextGen[i][j] = cell_next(currentGen, i, j);
        }
    }
}

int compare_boards(char** a, char** b) {
    for(int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
            if (a[i][j] != b[i][j])
                return 1;
        }
    }

    return 0;
}

void read_file(char* filename, char** board) {
    int BUF_LEN = 1000;
    char* buf = malloc(BUF_LEN);

    FILE* h = fopen(filename, "r");

    if (!h) {
        printf("Error, bad file. Aborting.");
        exit(1);
    }

    int row = 0;
    fgets(buf, BUF_LEN, h);
    while (!feof(h)) {

        if (buf[NUM_COLS] == '\n')
            buf[NUM_COLS] = '\0';

        if (strlen(buf) > NUM_COLS) {
            printf("%s has %d columns (max %d)\n", filename, (int)strlen(buf), NUM_COLS);
            exit(1);
        }

        for (int j=0; j < NUM_COLS; j++)
            board[row][j] = buf[j];

        row++;
        fgets(buf, BUF_LEN, h);
    }

    if (row != NUM_ROWS) {
        printf("%s has %d rows (must be %d)\n", filename, row, NUM_ROWS);
        exit(1);
    }

    fclose(h);
}
