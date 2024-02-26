#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct level{
	int rows;
	int cols;
	int mines;
	int board[100][100];
} customLevel, classicLevel;

void printBoardChar(customLevel *lvl){
    for (int i = 0; i < lvl->rows; i++) {
        for (int j = 0; j < lvl->cols; j++) {
			printf(" %c ", lvl->board[i][j]);
        }
        printf("\n");
    }
}

int fileExists(char *filename){
	FILE *level = fopen(filename, "r");
	if(level){
		fclose(level);
		return 1;
	} else{
		return 0;
	}
}

void saveFile(FILE *file, customLevel *lvl){
	fprintf(file, "%d %d\n", lvl->rows, lvl->cols);
	
	for (int i = 0; i < lvl->rows; i++) {
        for (int j = 0; j < lvl->cols; j++) {
            fprintf(file, "%c", lvl->board[i][j]);
        }
        fprintf(file, "\n");
    }
}


void editLevel(customLevel *lvl){
	int row, col, minesCount;
	
   	for (int i = 0; i < lvl->rows; i++) {
        for (int j = 0; j < lvl->cols; j++) {
            lvl->board[i][j] = '.';
        }
    }
    
    for(minesCount = 0; minesCount < lvl->mines; minesCount++) {
        printf("Enter row and column for mine %d: ", minesCount + 1);
        scanf("%d %d", &row, &col);
        if (row >= 0 && row < lvl->rows && col >= 0 && col < lvl->cols) {
            lvl->board[row][col] = 'X'; // Mark as mine
			printBoardChar(lvl);
    		printf("\n");
        } else {
            printf("Invalid position. Mine not placed.\n");
            minesCount--;
        }
    }
}

void levelEditor(customLevel *lvl){
 	//int rows, cols, mines;
	char filename[16];
	char path[] = "levels/";
	FILE *level;
	
	printf("Provide file name: ");
	scanf("%s", filename);
	strcat(filename, ".txt");
	strcat(path, filename);
	printf("%s\n", filename);
	printf("%s\n", path);
	
	if(fileExists(filename)){
			printf("Level cannot be created. File already exists.\n");
			return;
	} else {
			printf("Level %s will be created.\n", filename);
			level = fopen(path, "w");
	

	printf("Enter number of rows and columns: ");
	scanf("%d %d", &lvl->rows,& lvl->cols);

	printf("Enter number of mines: ");
	scanf("%d", &lvl->mines);
	
	/*
	lvl->rows = rows;
	lvl->cols = cols;
	lvl->mines = mines;
	*/
	
	editLevel(lvl);
	saveFile(level, lvl);
	fclose(level);
	printf("Level created successfully.\n");
	}
}

int main(){
    customLevel lvl;
    levelEditor(&lvl);
}