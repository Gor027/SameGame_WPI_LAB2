// First Name: Gor
// Last Name: Stepanyan
// Nr. Indeksu: 404865
// Email: gor.stepanyan027@gmail.com
// USOS Email: gs404865@students.mimuw.edu.pl

/*
 *This program is very similar to a popular game "SameGame" or "Swell Foop" in Linux Games.
 *Its input is a text, where each character besides space is considered to be a visible sign.
 *The characters will fall down if there is a an empty space(the empty space is considered to be character space itself)
 *The columns also shift left when there is an empty column
 *After the transformation of input data the leftmost bottom chain of the same characters are removed.
 *Note that the chain may include only one character.
*/

#include <stdio.h>
#include <stdlib.h>

char **getInput(int *rows, int *columns) {
    char **board = NULL;

    char *col = NULL;
    int *row_lengths = NULL; // Array to keep column size for each row

    int columnSize = 0;
    int rowSize = 0;

    int sign;
    while ((sign = getchar()) != EOF) {
        if (sign != '\n') {
            col = realloc(col, ++columnSize * sizeof(char));
            col[columnSize - 1] = sign;
        } else if (col != NULL) {
            board = realloc(board, ++rowSize * sizeof(char *));
            board[rowSize - 1] = col;

            row_lengths = realloc(row_lengths, rowSize * sizeof(int));
            row_lengths[rowSize - 1] = columnSize;

            *columns = (columnSize > *columns ? columnSize : *columns); // columns is maximum column size

            col = NULL;
            columnSize = 0;
        }
    }
    *rows = rowSize; //  rows is maximum row size

    // make board rectangle with space in empty places
    for (int i = 0; i < *rows; i++) {
        board[i] = realloc(board[i], *columns * sizeof(char));
        for (int j = row_lengths[i]; j < *columns; j++) {
            board[i][j] = ' ';
        }
    }

    free(row_lengths);
    return board;
}

void removeCell(char **board, int rows, int columns, int i, int j) {

    char cellValue = board[i][j];

    board[i][j] = ' ';

    // Check Bottom
    if (i + 1 < rows && board[i + 1][j] == cellValue && board[i + 1][j] != ' ') {
        removeCell(board, rows, columns, i + 1, j);
    }

    // Check Up
    if (i - 1 >= 0 && board[i - 1][j] == cellValue && board[i - 1][j] != ' ') {
        removeCell(board, rows, columns, i - 1, j);
    }

    // Check Right
    if (j + 1 < columns && board[i][j + 1] == cellValue && board[i][j + 1] != ' ') {
        removeCell(board, rows, columns, i, j + 1);
    }

    // Check Left
    if (j - 1 >= 0 && board[i][j - 1] == cellValue && board[i][j - 1] != ' ') {
        removeCell(board, rows, columns, i, j - 1);
    }
}

void shiftRowDown(char **board, int columnIndex, int rows) {
    int floor = rows;
    int index = floor;

    while (--index >= 0) {
        if (board[index][columnIndex] != ' ') {
            if (index != --floor) {
                board[floor][columnIndex] = board[index][columnIndex];
                board[index][columnIndex] = ' ';
            }
        }
    }
}

int emptyColumn(char **board, int rows, int columnIndex) {
    for (int i = rows - 1; i >= 0; i--) {
        if (board[i][columnIndex] != ' ') {
            return 0;
        }
    }

    return 1;
}

void swapColumns(char **board, int rows, int emptyColumnIndex, int nonEmptyColumnIndex) {
    for (int i = rows - 1; i >= 0; i--) {
        if (board[i][nonEmptyColumnIndex] != ' ') {
            board[i][emptyColumnIndex] = board[i][nonEmptyColumnIndex];
            board[i][nonEmptyColumnIndex] = ' ';
        }
    }
}

void shiftColumnLeft(char **board, int rows, int columns) {
    int emptyColumnIndex = 0;
    while (emptyColumnIndex < columns && !emptyColumn(board, rows, emptyColumnIndex)) {
        emptyColumnIndex++;
    }

    for (int j = emptyColumnIndex + 1; j < columns; j++) {
        if (!emptyColumn(board, rows, j)) {
            swapColumns(board, rows, emptyColumnIndex, j);
            emptyColumnIndex++;
        }
    }
}

void gravity(char **board, int rows, int columns) {
    for (int columnIndex = 0; columnIndex < columns; columnIndex++) {
        shiftRowDown(board, columnIndex, rows);
    }

    shiftColumnLeft(board, rows, columns);
}

void printBoard(char **board, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        int k = columns - 1;
        while (k >= 0 && board[i][k] == ' ') {
            k--;
        }
        for (int j = 0; j <= k; j++) {
            printf("%c", board[i][j]);
        }
        if (k >= 0) {
            printf("\n");
        }
    }
}

int main() {

    int rows = 0;
    int columns = 0;

    char **board = getInput(&rows, &columns);

    if (board != NULL) {
        gravity(board, rows, columns);
        removeCell(board, rows, columns, rows - 1, 0);
        gravity(board, rows, columns);
    }
    printBoard(board, rows, columns);

    // Free allocated memories to prevent memory leaks
    for (int i = 0; i < rows; i++) {
        char *curr = board[i];
        free(curr);
    }
    free(board);

    return 0;
}