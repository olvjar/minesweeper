#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct level {
    int rows;
    int cols;
    int mines;
    int gameBoard[100][100];
    char board[100][100];
};

typedef struct level game;

void printBoardChar(game *customLevel) {
    for (int i = 0; i < customLevel->rows; i++) {
        for (int j = 0; j < customLevel->cols; j++) {
            printf(" %c ", customLevel->gameBoard[i][j]);
        }
        printf("\n");
    }
}

int fileExists(char *filename) {
    FILE *level = fopen(filename, "r");
    if (level) {
        fclose(level);
        return 1;
    } else {
        return 0;
    }
}

void saveFile(FILE *file, game *customLevel) {
    fprintf(file, "%d %d\n", customLevel->rows, customLevel->cols);

    for (int i = 0; i < customLevel->rows; i++) {
        for (int j = 0; j < customLevel->cols; j++) {
            fprintf(file, "%c", customLevel->gameBoard[i][j]);
        }
        fprintf(file, "\n");
    }
}

void placeMine(game *customLevel, int *minesCount) {
    int row, col;

    printf("Enter row and column for mine %d: ", *minesCount + 1);
    scanf("%d %d", &row, &col);
    if (row >= 0 && row < customLevel->rows && col >= 0 && col < customLevel->cols && customLevel->gameBoard[row][col] == '.') {
        customLevel->gameBoard[row][col] = 'X'; // Place mine
        (*minesCount)++;
        printBoardChar(customLevel);
        printf("\n");
    } else {
        printf("Invalid position. Mine not placed.\n\n");
    }
}

void deleteMine(game *customLevel, int *minesCount) {
    int row, col;

    printf("Enter row and column of the mine to delete: ");
    scanf("%d %d", &row, &col);
    if (row >= 0 && row < customLevel->rows && col >= 0 && col < customLevel->cols && customLevel->gameBoard[row][col] == 'X') {
        customLevel->gameBoard[row][col] = '.'; // Delete mine
        (*minesCount)--;
        printBoardChar(customLevel);
        printf("\n");
    } else {
        printf("Invalid position. There is no mine.\n\n");
    }
}

void editLevel(game *customLevel) {
    int minesCount = 0;
    int choice;
    
    for (int i = 0; i < customLevel->rows; i++) {
        for (int j = 0; j < customLevel->cols; j++) {
            customLevel->gameBoard[i][j] = '.';
        }
    }
    
    while (minesCount < customLevel->mines) {
        printf("%d\n", minesCount);
        printf("[1] PLACE mine\n[2] DELETE mine\n[3] FINISH\n\nSelection: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                placeMine(customLevel, &minesCount);
                break;
            case 2:
                deleteMine(customLevel, &minesCount);
                break;
            case 3:
                return;
        }
    }
}

void levelEditor(game *customLevel) {
    char filename[16];
    char path[] = "levels/";
    FILE *level;

    printf("Provide file name: ");
    scanf("%s", filename);
    strcat(filename, ".txt");
    strcat(path, filename);
    printf("%s\n", filename);
    printf("%s\n", path);

    if (fileExists(path) == 1) {
        printf("Level cannot be created. File already exists.\n");
        return;
    } else {
        printf("Level %s will be created.\n", filename);
        level = fopen(path, "w");

        printf("Enter number of rows and columns: ");
        scanf("%d %d", &customLevel->rows, &customLevel->cols);

        printf("Enter number of mines: ");
        scanf("%d", &customLevel->mines);

        editLevel(customLevel);
        saveFile(level, customLevel);
        fclose(level);
        printf("Level created successfully.\n");
    }
}

int main() {
    game customLevel;
    levelEditor(&customLevel);
    return 0;
}