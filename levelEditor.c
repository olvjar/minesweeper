#include <stdio.h>
#include <string.h>
#include <time.h>

#include "interface.c"

#define LVL_PATH "levels/"
#define LVL_DIR "levels/level_list.txt"

struct level {
    int rows;
    int cols;
    int mines;
	char mode[11];
    int gameBoard[100][100];
    char board[100][100];
};

struct fileInfo{
	char filename[20];
	char path[100];
};

typedef struct level game;
typedef struct fileInfo file;
typedef file customLevelList[100];

void menuEditLevel(int *choice){
	printf("[1] PLACE mine\n[2] DELETE mine\n[3] SAVE\n[4] RETURN to level editor menu\n\nSelection: ");
	scanf("%d", choice);
}

void menuLevelEditor(int *choice){
	printf("[1] CREATE a new level\n[2] EDIT an existing level\n[3] DELETE an existing level\n[4] RETURN to main menu\n\nSelection: ");
	scanf("%d", choice);
}

void printCustomBoard(game *customLevel) {
    for (int i = 0; i < customLevel->rows; i++) {
        for (int j = 0; j < customLevel->cols; j++) {
            printf(" %c ", customLevel->board[i][j]);
        }
        printf("\n");
    }
}

int fileExists(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1; // exists
    } else {
        return 0; // null
    }
}

void checkLevels(customLevelList *cLevels){
	int i, numFiles;
	FILE *dir;

	dir = fopen(LVL_DIR, "r");
	fscanf(dir, "%d", &numFiles);

	for(i = 0; i < numFiles; i++){
		fscanf(dir, "%s", cLevels[i]->filename);
		printf("%d. ", i + 1);
		printf("%s\n", cLevels[i]->filename);
	}

	fclose(dir);
}

void placeMine(game *customLevel, int *minesCount) {
    int row, col;

    printf("Enter row and column for mine %d: ", (*minesCount) + 1);
    scanf("%d %d", &row, &col);
    if (row >= 0 && row < customLevel->rows && col >= 0 && col < customLevel->cols && customLevel->board[row][col] == '.') {
        customLevel->board[row][col] = 'X'; // Place mine
        (*minesCount)++;
        printf("\n");
    } else {
        printf("Invalid position. Mine not placed.\n\n");
    }
}

void deleteMine(game *customLevel, int *minesCount) {
    int row, col;

    printf("Enter row and column of the mine to delete: ");
    scanf("%d %d", &row, &col);
    if (row >= 0 && row < customLevel->rows && col >= 0 && col < customLevel->cols && customLevel->board[row][col] == 'X') {
        customLevel->board[row][col] = '.'; // Delete mine
        (*minesCount)--;
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

void deleteFile(customLevelList *cLevels){
	char filename[21];
	char path[100] = LVL_PATH;
	int num, i;
	FILE *dir;

	printf("EXISTING CUSTOM LEVELS:\n");
	checkLevels(cLevels);

	printf("\nProvide level to delete (do not include .txt): ");
	scanf("%s", filename);
	strcat(filename, ".txt");
    strcat(path, filename);
    printf("%s\n", path);

	if(fileExists(path) == 0) {
        printf("\nLevel does not exist. Try again.\n");
        return;
    } else {
		// read list
	    dir = fopen(LVL_DIR, "r");
	    fscanf(dir, "%d", &num);
	    for(i = 0; i < num; i++){
			fscanf(dir, "%s", cLevels[i]->filename);
		}
		fclose(dir);

		// update list
	    dir = fopen(LVL_DIR, "w");
	    fprintf(dir, "%d\n", num - 1);
	    for(i = 0; i < num; i++){
	    	if(strcmp(cLevels[i]->filename, filename) == 0){
	    		fprintf(dir, "%s", "");
			} else fprintf(dir, "%s\n", cLevels[i]->filename);
		}
	    fclose(dir);

	    // delete level file
	    remove(path);
	    printf("%s deleted successfully.", filename);
	}
}

void saveFile(int mode, FILE *file, game *customLevel, customLevelList *cLevels, char *filename) {
	fprintf(file, "%d %d\n", customLevel->rows, customLevel->cols);

    for (int i = 0; i < customLevel->rows; i++) {
        for (int j = 0; j < customLevel->cols; j++) {
            fprintf(file, "%c", customLevel->board[i][j]);
        }
        fprintf(file, "\n");
    }

	FILE *dir;
    int i, num;
    if(mode == 0){
    	// read list
    	dir = fopen(LVL_DIR, "r");
    	fscanf(dir, "%d", &num);
    	for(i = 0; i < num; i++){
			fscanf(dir, "%s", cLevels[i]->filename);
		}
		fclose(dir);

		// write list
    	dir = fopen(LVL_DIR, "w");
    	fprintf(dir, "%d\n", num + 1);
    	for(i = 0; i < num; i++){
			fprintf(dir, "%s\n", cLevels[i]->filename);
		}
		fprintf(dir, "%s\n", filename);
    	fclose(dir);
	}
}

int editLevel(game *customLevel) {
    int minesCount;
    int save;
    int quit;
    int choice;

    while(quit != 1){
    	printCustomBoard(customLevel);
    	printf("GRID: %dx%d\t", customLevel->rows, customLevel->cols);
    	printf("MINES: %d\n", minesCount);
		menuEditLevel(&choice);

        switch(choice){
            case 1:
                placeMine(customLevel, &minesCount);
                break;
            case 2:
                deleteMine(customLevel, &minesCount);
                break;
            case 3:
            	if(checkValidity(customLevel, &minesCount) == 1) {
            		save = 1;
            		quit = 1;
                }
                break;
            case 4:
				printf("Returned to main menu.\n");
				quit = 1;
				break;
			default:
            	printf("Invalid selection. Please choose again.\n");
        }

    	iClear(0,0,100,30);
	}

	return save;
}

void loadLevel(game *customLevel, customLevelList *cLevels){
	char filename[20];
    char path[100] = LVL_PATH;
    FILE *level;

	printf("EXISTING CUSTOM LEVELS:\n");
	checkLevels(cLevels);

	printf("\nProvide level name to edit: ");
	scanf("%s", filename);
    strcat(filename, ".txt");
    strcat(path, filename);
    printf("%s\n", path);

    if(fileExists(path) == 0) {
        printf("\nLevel does not exist. Try again.\n");
        return;
    } else {
    	level = fopen(path, "r");

		fscanf(level, "%d %d", &customLevel->rows, &customLevel->cols);
		for(int i = 0; i < customLevel->rows; i++) {
			fscanf(level, "%s", customLevel->board[i]);
    	}

    	editLevel(customLevel); 

    	if(editLevel(customLevel) == 1){
    		level = fopen(path, "w");
    		saveFile(1, level, customLevel, cLevels, filename);
    		fclose(level);
        	printf("Level successfully edited.\n\n");
		} else{
			printf("Level was not saved.\n\n");
		}

		fclose(level);
	}
}

void createLevel(game *customLevel, customLevelList *cLevels){
	char filename[20];
    char path[100] = LVL_PATH;
    FILE *level;

	printf("Provide file name: ");
    scanf("%s", filename);
    strcat(filename, ".txt");
    strcat(path, filename);

    if (fileExists(path) == 1) {
        printf("Level cannot be created. File already exists.\n");
    } else {
        printf("Level %s will be created.\n", filename);

		int validNum = 0;
		while(!validNum){
				printf("Enter number of rows (5 to 10) and columns (5 to 15): ");
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
            	customLevel->board[i][j] = '.';
        	}
    	}

        editLevel(customLevel);

        if(editLevel(customLevel) == 1){
    		level = fopen(path, "w");
    		saveFile(0, level, customLevel, cLevels, filename);
    		fclose(level);
        	printf("Level created successfully.\n\n");
		} else{
			printf("Level was not saved.\n\n");
		}
    }
}

void levelEditor(game *customLevel, customLevelList *cLevels) {
    int quit, choice;

	while(quit != 1){
		printf("What would you like to do?\n");
		menuLevelEditor(&choice);

        switch (choice) {
            case 1:
                createLevel(customLevel, cLevels);
                break;
            case 2:
                loadLevel(customLevel, cLevels);
                break;
            case 3:
            	deleteFile(cLevels);
                break;
            case 4:
				printf("Returned to main menu.\n");
				quit = 1;
				break;
			default:
            	printf("Invalid selection. Please choose again.\n");
        }

    	iClear(0,0,100,30);
	}
}


int main() {
    game customLevel;
    customLevelList customLvls;
    levelEditor(&customLevel, &customLvls);
    return 0;
}
