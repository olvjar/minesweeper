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
    if (row >= 0 && row < customLevel->rows && col >= 0 && col < customLevel->cols &&
        customLevel->gameBoard[row][col] == 'X') {
        customLevel->gameBoard[row][col] = '.'; // Delete mine
        (*minesCount)--;
        printBoardChar(customLevel);
        printf("\n");
    } else {
        printf("Invalid position. There is no mine.\n\n");
    }
}

int checkValidity(game *customLevel, int *minesCount){
	int cells = customLevel->rows * customLevel->cols;
	
	if(cells == *minesCount){
		printf("Invalid level: Every cell contains a mine.\n");
		return 0;
	} else if(*minesCount == 0){
		printf("Invalid level: There are no mines placed.\n");
		return 0;
	} else{
		return 1;
	}
}

int editLevel(game *customLevel) {
    int minesCount = 0;
    int save;
    int quit;
    int choice;
    
    printBoardChar(customLevel);
    
    while(save == 0 || quit == 0){
    	printf("%d\n", minesCount);
    	printf("[1] PLACE mine\n[2] DELETE mine\n[3] SAVE\n[4] RETURN to main menu\n\nSelection: ");
    	scanf("%d", &choice);
    	
        switch (choice) {
            case 1:
                placeMine(customLevel, &minesCount);
                break;
            case 2:
                deleteMine(customLevel, &minesCount);
                break;
            case 3:
            	if(checkValidity(customLevel, &minesCount) == 1) {
            	save = 1;
                }
                break;
            case 4:
				quit = 1;
				break;
        }
	}
	return save;
}

void levelEditor(game *customLevel) {
    char filename[20];
    char path[] = "levels/";
    FILE *level;

    printf("Provide file name: ");
    scanf("%s", filename);
    strcat(filename, ".txt");
    strcat(path, filename);

    if (fileExists(path) == 1) {
        printf("Level cannot be created. File already exists.\n");
        return;
    } else {
        printf("Level %s will be created.\n", filename);
        level = fopen(path, "w");

		int validNum = 0;
		while(!validNum){
				printf("Enter number of rows and columns: ");
        		scanf("%d %d", &customLevel->rows, &customLevel->cols);
        		
				if((customLevel->rows < 5 || customLevel->rows > 10) && (customLevel->cols < 5 || customLevel->cols > 15)){
        			printf("Invalid number of rows and columns.\n\n");
				} else if (customLevel->rows < 5 || customLevel->rows > 10) {
            		printf("Invalid number of rows.\n\n");
        		} else if(customLevel->cols < 5 || customLevel->cols > 15){
        			printf("Invalid number of columns.\n\n");
				} else {
					validNum = 1;
				}
		}
		
		for (int i = 0; i < customLevel->rows; i++) {
        	for (int j = 0; j < customLevel->cols; j++) {
            	customLevel->gameBoard[i][j] = '.';
        	}
    	}
    	
        editLevel(customLevel);
    }
    
    if(editLevel(customLevel) == 1){
    	saveFile(level, customLevel);
        printf("Level created successfully.\n");
	}
	
	fclose(level);
}

int main() {
    game customLevel;
    levelEditor(&customLevel);
    return 0;
}