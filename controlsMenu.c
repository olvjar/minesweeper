/*
	Description: <describe what this program does briefly >
	Author: GRINO, Mary Eunice E. and TAMONDONG, Mariel M.
	Section: S17B
	Last Modified: February 07, 2024
	Acknowledgments: Thomas Tiam-Lee
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "interface.c"

#define MAX_PROFILES 10

#define HIDDEN 10
#define FLAG 100
#define CLASSIC_EASY "CLASSIC-EASY"
#define CLASSIC_DIFFICULT "CLASSIC-DIFFICULT"
#define CUSTOM "CUSTOM"

#define LVL_PATH "levels/"
#define LVL_DIR "levels/level_list.txt"
#define USER_PATH "profiles/"
#define USER_DIR "profiles/profile_list.txt"
#define GAME_PATH "snapshots/"

// PREPROCESSOR DIRECTIVES
struct level {
    int rows;
    int cols;
    int mines;
	char mode[30];
    int gameBoard[100][100];
    char board[100][100];
};

struct fileInfo{
	char filename[21];
	char path[100];
};

typedef struct level game;
typedef struct fileInfo file;
typedef file customLevelList[100];
typedef game recentGame;

struct recent_games{
	char path[100];
	char outcome[50];
	char mode[50];
	int time;
    int rows;
    int cols;
	char snapshot[100][100];
};

struct player{
	char name[21];
	int games_won_classic;
	int games_lost_classic;
	int games_won_custom;
	int games_lost_custom;
	struct recent_games recentgame[3];
};

typedef struct player profile;
typedef profile profileList[MAX_PROFILES];

// FUNCTIONS

void delay(int time){
	clock_t delay;
	delay = clock() + time;
	while(delay > clock());
}

/* board */

void printBoard(game level){
    int i, j;
    
    printf("\n");
	printf("     ");
	for (i = 0; i < 2; i++) {
		for(j = 0; j <  level.cols; j++) {
			
			if(i == 0){
				iSetColor(I_COLOR_PURPLE);
    			printf("%2d ", j);
    				if(j == level.cols - 1){
    					printf("\n");
					}
    		}
    		
    		else if (i == 1){
    			if (j == 0){
					printf("   ");
				}
    			else printf("---");
			}
		}
	}
	printf("-------");
	
	printf("\n");
	
	for (i = 0; i < level.rows; i++) {
        for (j = -1; j <= level.cols; j++) {
			if (j == -1){
				iSetColor(I_COLOR_PURPLE);
				printf(" %d | ", i);
				iSetColor(I_COLOR_WHITE);
				}
			else if (j == level.cols){
				iSetColor(I_COLOR_PURPLE);
				printf(" | ");
			}
			else if (level.gameBoard[i][j] == HIDDEN){ //not revealed
				iSetColor(I_COLOR_WHITE);
				printf(" . ");
			}
			else if (level.gameBoard[i][j] == FLAG){ //flag
				iSetColor(I_COLOR_RED);
				printf(" F ");
				iSetColor(I_COLOR_WHITE);
			}
			else{
				iSetColor(I_COLOR_GREEN);
				printf(" %d ", level.gameBoard[i][j]); //valid space & revealed
				iSetColor(I_COLOR_WHITE);
			}
				
        }
        printf("\n");
    }
    
    for (i = 0; i < level.cols; i++) {
    	if (i == 0){
    		printf("   ");	
		} else printf("---");
	}
	printf("-------\n");
	iSetColor(I_COLOR_WHITE);
}

//for testing of made board/text file
void printBoardChar(game level){
    printf("\n");
	for (int i = 0; i < level.rows; i++) {
        for (int j = 0; j < level.cols; j++) {
			printf(" %c ", level.board[i][j]);
        }
        printf("\n");
    }
}

void makeBoard(game *level){
    srand(time(0));
	int minesCount;
	
   	for (int i = 0; i < level->rows; i++) {
        for (int j = 0; j < level->cols; j++) {
            level->board[i][j] = '.';
        }
    }
    
    for(minesCount = 0; minesCount < level->mines; minesCount++) {
        int row = rand() % level->rows;
        int col = rand() % level->cols;
        if (level->board[row][col] == 'X') {
            minesCount--;
        } else {
            level->board[row][col] = 'X';
        }
    }
    printf("\n");
}

int mineCount(game level, int i, int j){
	int count = 0;
	
	if(level.board[i][j] != 'X'){
		if(level.board[i-1][j] == 'X')
		count++;
		if(level.board[i+1][j] == 'X')
		count++;
		if(level.board[i][j-1] == 'X')
		count++;
		if(level.board[i][j+1] == 'X')
		count++;
		if(level.board[i-1][j-1] == 'X')
		count++;
		if(level.board[i-1][j+1] == 'X')
		count++;
		if(level.board[i+1][j-1] == 'X')
		count++;
		if(level.board[i+1][j+1] == 'X')
		count++;
			
		return count;
		}
	else return -1;
}

void cascade(game *level, int i, int j){
    if (i < 0 || i >= level->rows || j < 0 || j >= level->cols || 
        level->board[i][j] == 'X' || level->gameBoard[i][j] == FLAG || !(level->gameBoard[i][j] == HIDDEN)) {
        return; // Base case: out of bounds or already revealed/flagged
    }
    
    level->gameBoard[i][j] = mineCount(*level, i, j); // reveal mine count
    
    if (level->gameBoard[i][j] == 0) { // if no neighboring mines
        cascade(level, i - 1, j); // up
        cascade(level, i + 1, j); // down
        cascade(level, i, j - 1); // left
        cascade(level, i, j + 1); // right
    }
}

void placeFlag(game *level){
	int i, j;
	
	printf("\nEnter row to flag: ");
	scanf(" %d", &i);
	printf("Enter column to flag: ");
	scanf(" %d", &j);
	
	if(level->gameBoard[i][j] == HIDDEN){
		level->gameBoard[i][j] = FLAG;
	}
	else if (level->gameBoard[i][j] == FLAG)
		printf("Tile is already flagged. Try again.\n");
	else if (i >= level->rows || j >= level->cols)
		printf("Input is out of bounds\n");
	else printf("Tile is already revealed. Try again.\n");
}

void removeFlag(game *level){
	int i, j;
	
	printf("\nEnter row to remove flag: ");
	scanf(" %d", &i);
	printf("Enter column to remove flag: ");
	scanf(" %d", &j);
	
	if(level->gameBoard[i][j] == FLAG){
		level->gameBoard[i][j] = HIDDEN;
	}
	else if (i >= level->rows || j >= level->cols)
		printf("Input is out of bounds\n");
	else printf("Tile is not flagged. Try again.\n");
}

int inspectBoard(game *level, char outcome[]) {
    int i, j;
    
    printf("\nEnter row to inspect: ");
    scanf(" %d", &i);
    printf("Enter column to inspect: ");
    scanf(" %d", &j);
    
    if (i < level->rows && j < level->cols) {
        if (mineCount(*level, i, j) == -1) {
            printf("\nYou have hit a mine. Game over.\n");
            strcpy(outcome, "lose");
            level->gameBoard[i][j] = 999;
            printBoardChar(*level);
            return 0;    
        } else {
            cascade(level, i, j);
            return 1;
        }
    } else {
        printf("Invalid input. Try again.\n");
        return 1;
    }
}

int gameChecker(game level, char outcome[]){
	int totalSquares = (level.rows*level.cols)-level.mines;
	int revealedCount = 0;
	int i, j;
	
		for(i = 0; i < level.rows; i++){
			for(j = 0; j < level.cols; j++){
				if (level.gameBoard[i][j] != HIDDEN && level.board[i][j] != 'X'){
					revealedCount++;
				}
			}
		}
		
	if(revealedCount == totalSquares){
		printBoard(level);
		printf("\nAll non-mine tiles revealed.\nYou win!\n");
		strcpy(outcome, "win");
		printBoardChar(level);
		return 0;
	}
	else return 1;
}

void transferSnapshot(char destFile[], char sourceFile[]){
	FILE *fsource;
	FILE *fdest;
	char mode[21];
	char outcome[51];
	int rows, cols, i ,j;
	char val[100][100];
	
	fsource = fopen(sourceFile, "r");
	fdest = fopen(destFile, "w");
	
	fscanf(fsource, "%s", outcome);
	fscanf(fsource, "%s", mode);
	fscanf(fsource, "%d", &rows);
	fscanf(fsource, "%d", &cols);

	fprintf(fdest, "%s\n", outcome);
	fprintf(fdest, "%s ", mode);
	fprintf(fdest, "%d ", rows);
	fprintf(fdest, "%d\n", cols);

	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			fscanf(fsource, " %c", &val[i][j]);
		}
	}
	
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			fprintf(fdest, "%c ", val[i][j]);
		}
		fprintf(fdest, "\n");
	}
	
	fclose(fsource);
	fclose(fdest);
}

int saveSnapshot(game level, char outcome[], profile currentUser){
	int i, j;
    FILE *fgame;
	
	transferSnapshot(currentUser.recentgame[2].path, currentUser.recentgame[1].path); // overwrites 2 with 1
	transferSnapshot(currentUser.recentgame[1].path, currentUser.recentgame[0].path); // overwrites 1 with 0
    
	// 0 will now be overwritten with latest game
    fgame = fopen(currentUser.recentgame[0].path, "w");
    
    if (strcmp(outcome, "win") == 0){
		fprintf(fgame, "WON\n");
		fprintf(fgame, "%s %d %d\n", level.mode, level.rows, level.cols);
		
		for (i = 0; i < level.rows; i++){
    		for (j = 0; j < level.cols; j++){
    			
    			if (level.gameBoard[i][j] != FLAG && level.board[i][j] != 'X'){ // tile != flag && tile != bomb
					fprintf(fgame, "%d ", level.gameBoard[i][j]);
				}
				else if (level.board[i][j] == 'X' || level.gameBoard[i][j] == FLAG){ // tile == bomb || tile == flag (game only wins if all tiles are shown, so a flag == bomb)
					fprintf(fgame, "X ");
				}
		}
		fprintf(fgame, "\n");
	}
}
	
	else if (strcmp(outcome, "lose") == 0){
		fprintf(fgame, "LOST\n");
		fprintf(fgame, "%s %d %d\n", level.mode, level.rows, level.cols);
		
		for (i = 0; i < level.rows; i++){
    		for (j = 0; j < level.cols; j++){
    			if (level.gameBoard[i][j] != HIDDEN && level.gameBoard[i][j] != FLAG && level.gameBoard[i][j] != 999){ // tile != hidden && tile != flag && tile != bombExploded
					fprintf(fgame, "%d ", level.gameBoard[i][j]);
				}
				else if(level.gameBoard[i][j] == 999){ // bomb == exploded
					fprintf(fgame, "X ");
				}
				else if (level.board[i][j] == 'X'){ // tile == bomb
					fprintf(fgame, "x ");
				}
				else if (level.gameBoard[i][j] == FLAG){ // tile == flag
    				fprintf(fgame, "F ");
				}
				else if (level.gameBoard[i][j] == HIDDEN){ // tile == not revealed
					fprintf(fgame, ". ");
    			}
			}
		fprintf(fgame, "\n");
		}
}
	
	else if (strcmp(outcome, "quit") == 0){
		fprintf(fgame, "QUIT\n");
		fprintf(fgame, "%s %d %d\n", level.mode, level.rows, level.cols);
 
		
		for(i = 0; i < level.rows; i++){
			for(j = 0; j < level.cols; j++){
				
				if(level.gameBoard[i][j] == HIDDEN){
					fprintf(fgame, ". ");
				}
				else if (level.gameBoard[i][j] == FLAG){
					fprintf(fgame, "F ");
				}
				else fprintf(fgame, "%d ", level.gameBoard[i][j]);
			}
		fprintf(fgame, "\n");
		}
	}
    
    fclose(fgame);
    
    return 1;
}

void updateStatistics(game level, char outcome[], profile *currentUser){
	char path[101] = USER_PATH;
	FILE *user;

	strcat(path, currentUser->name);
	strcat(path, ".txt");

	if (strcmp(outcome, "win") == 0){
		if(strcmp(level.mode, CLASSIC_EASY) == 0){
			currentUser->games_won_classic++;
		}
		if(strcmp(level.mode, CLASSIC_DIFFICULT) == 0){
			currentUser->games_won_classic++;
		}
		if(strcmp(level.mode, CUSTOM) == 0){
			currentUser->games_won_custom++;
		}
	}
	
	else if (strcmp(outcome, "lose") == 0){
		if(strcmp(level.mode, CLASSIC_EASY) == 0){
			currentUser->games_lost_classic++;
		}
		if(strcmp(level.mode, CLASSIC_DIFFICULT) == 0){
			currentUser->games_lost_classic++;
		}
		if(strcmp(level.mode, CUSTOM) == 0){
			currentUser->games_lost_custom++;
		}
	}
	
	else if (strcmp(outcome, "quit") == 0){
		if(strcmp(level.mode, CLASSIC_EASY) == 0){
			currentUser->games_lost_classic++;
		}
		if(strcmp(level.mode, CLASSIC_DIFFICULT) == 0){
			currentUser->games_lost_classic++;
		}
		if(strcmp(level.mode, CUSTOM) == 0){
			currentUser->games_lost_custom++;
		}
	}

	user = fopen(path, "w");
	fprintf(user, "%s\n", currentUser->name);
	fprintf(user, "%d\n", currentUser->games_won_classic);
	fprintf(user, "%d\n", currentUser->games_lost_classic);
	fprintf(user, "%d\n", currentUser->games_won_custom);
	fprintf(user, "%d\n", currentUser->games_lost_custom);
	for(int i = 0; i < 3; i ++){
		fprintf(user, "%s\n", currentUser->recentgame[i].path);
	}

	fclose(user);
}

void gameProper(game level, profile *currentUser){
	int i, j, alive = 1;
	int choice;
	char outcome[11];

	//PLAYER BOARD
	for(i = 0; i < level.rows; i++){
		for (j = 0; j < level.cols; j++){
            level.gameBoard[i][j] = HIDDEN; 
        }
    }
    
    system("cls");
    printBoardChar(level); // FOR TESTING
	time_t timeStart, timeEnd;
	int timeElapsed, hours, minutes, seconds;
	
	time(&timeStart);
	while(alive){
		time(&timeEnd);
        timeElapsed = difftime(timeEnd, timeStart);
        
        hours = timeElapsed / 3600;
        minutes = (timeElapsed % 3600) / 60;
        seconds = timeElapsed % 60;
        
		printf("\n\tTIME: %02d:%02d:%02d", hours, minutes, seconds);
		delay(300);
		printBoard(level);
		printf("\n[1] INSPECT\n[2] FLAG\n[3] REMOVE FLAG\n[0] QUIT\n\nSELECTION: ");
		scanf(" %d", &choice);
		
		switch (choice){
			case 1:
				alive = inspectBoard(&level, outcome);
				break;
			case 2:
				placeFlag(&level);
				break;
			case 3:
				removeFlag(&level);
				break;
			case 0:
				strcpy(outcome, "quit");
				printf("Game QUIT\n");
				alive = 0;
				break;
			default:
				printf("Invalid input. Please try again.\n");
		}
		
		if (alive){
		alive = gameChecker(level, outcome);
		}
		system("cls");
	}
	saveSnapshot(level, outcome, *currentUser);
	updateStatistics(level, outcome, currentUser);
}

/* level edit */

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

int editLevel(game *customLevel, int minesCount){
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

    	system("cls");
	}

	return save;
}

void loadLevel(game *customLevel, customLevelList *cLevels){
	char filename[20];
    char path[100] = LVL_PATH;
    FILE *level;
    int minesCount = 0;

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

		// read level data
		fscanf(level, "%d %d", &customLevel->rows, &customLevel->cols);
		for(int i = 0; i < customLevel->rows; i++) {
			fscanf(level, "%s", customLevel->board[i]);
    	}
    	
    	// count mines
	    for (int i = 0; i < customLevel->rows; i++) {
	        for (int j = 0; j < customLevel->cols; j++) {
	            if (customLevel->board[i][j] == 'X') {
        			minesCount++;
    			}
	        }
	    }

    	editLevel(customLevel, minesCount); 

    	if(editLevel(customLevel, minesCount) == 1){
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
    int minesCount = 0;

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

        editLevel(customLevel, minesCount);

        if(editLevel(customLevel, minesCount) == 1){
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

    	system("cls");
	}
}

/* statistics */

void getStatistics(profile *currentUser){
	FILE *user;
	char path[] = USER_PATH;
	
	strcat(path, currentUser->name);
	strcat(path, ".txt");
	
    user = fopen(path, "r");
    fscanf(user, "%s", currentUser->name);
    fscanf(user, "%d\n%d\n%d\n%d", &currentUser->games_won_classic, &currentUser->games_lost_classic, &currentUser->games_won_custom, &currentUser->games_lost_custom);
 	for(int i = 0; i < 3; i++){
   		fscanf(user, "%s\n", currentUser->recentgame[i].path);
	}
}

void viewStatistics(profile *currentUser){
    int i, j, k;
    FILE *recentgames;
	
	getStatistics(currentUser);
	
	printf("Name: %s\n", currentUser->name);
    printf("Classic games - Won: %d Lost: %d\n", currentUser->games_won_classic, currentUser->games_lost_classic);
    printf("Custom games - Won: %d Lost: %d\n", currentUser->games_won_custom, currentUser->games_lost_custom);
	printf("Recent Games:\n");
    
    for(i = 0; i < 3; i++) {
	    recentgames = fopen(currentUser->recentgame[i].path, "r");
	    
		fscanf(recentgames, "%s", currentUser->recentgame[i].outcome);
	    fscanf(recentgames, "%s", currentUser->recentgame[i].mode);
	    fscanf(recentgames, "%d %d", &currentUser->recentgame[i].rows, &currentUser->recentgame[i].cols);
	    
	  	for (int b = 0; b < currentUser->recentgame[i].rows; b++) {
	       for (int c = 0; c < currentUser->recentgame[i].cols; c++) {
	            fscanf(recentgames, " %c", &currentUser->recentgame[i].snapshot[b][c]);
	        }
	    }
	    
	    fclose(recentgames);
	        
	    printf("\nGAME %s\n", currentUser->recentgame[i].outcome);
	    printf("%s\n", currentUser->recentgame[i].mode);
	    printf("%d %d\n", currentUser->recentgame[i].rows, currentUser->recentgame[i].cols);
	    for (j = 0; j < currentUser->recentgame[i].rows; j++) {
	        for (k = 0; k < currentUser->recentgame[i].cols; k++) {
	            if(currentUser->recentgame[i].snapshot[j][k] == 'X'){
					printf("%c ", currentUser->recentgame[i].snapshot[j][k]);
	        }
	    printf("\n");
	    }
	    iSetColor(I_COLOR_WHITE);
    }
}
}


/* profile */

void menuProfile(profile currentUser, int *choice){
	printf("CURRENT USER: %s\n\n[1] SELECT profile\n[2] CREATE NEW profile\n[3] DELETE profile\n[4] RETURN to main menu\n\nSELECTION: ", currentUser.name);
	scanf("%d", choice);
}
int checkCapital(char name[]){
	int i;
	
	for(i = 0; i<strlen(name); i++){
		if(name[i] >= 'A' && name [i] <= 'Z'){
		} else return 0;
	}
	return 1;
}

void sortProfiles(profileList users){
	int i, j, num;
	int low;
	char temp[21];
	FILE *dir;
	
	// read list
	dir = fopen(USER_DIR, "r");
	fscanf(dir, " %d", &num);
    for(i = 0; i < num; i++){
		fscanf(dir, "%s", users[i].name);
	}
	fclose(dir);
	
	//sort algorithm
	for (i = 0; i < num-1; i++){
		low = i;
		for (j = i+1; j < num; j++){
			if(strcmp(users[low].name, users[j].name) > 0){ // when low is higher
				low = j;
			}
		}
		
		if (i != low){
			strcpy(temp, users[i].name);
			strcpy(users[i].name, users[low].name);
			strcpy(users[low].name, temp);
		}
	}

	// update list
    dir = fopen(USER_DIR, "w");
    fprintf(dir, "%d\n", num);
    for(i = 0; i < num; i++){
		fprintf(dir, "%s\n", users[i].name);
	}
    fclose(dir);
}

void checkProfiles(profileList users){
	int i, numFiles;
	FILE *dir;

	sortProfiles(users);
	dir = fopen(USER_DIR, "r");
	fscanf(dir, "%d", &numFiles);

	for(i = 0; i < numFiles; i++){
		fscanf(dir, "%s", users[i].name);
		printf("%d. ", i + 1);

		if(strcmp(users[i].name, "") == 0){
			printf("< empty >\n");
		} else printf("%s\n", users[i].name);
	}

	fclose(dir);
}

int selectProfile(profile *currentUser, profileList *users){
	char name[21];
	char filename[21];
	char path[] = USER_PATH;
	//FILE *user;

	printf("[PROFILE SELECTION]\nCURRENT USER: %s\n", currentUser->name);
	checkProfiles(*users);

	printf("\nSELECT PROFILE TO USE: ");
	scanf("%s", name);
	strcpy(filename, name);
	strcat(filename, ".txt");
	strcat(path, filename);
	
	if(fileExists(path) == 0) {
	    printf("\nProfile does not exist. Try again.\n\n");
		return 0;
	} else if(!(checkCapital(name))) {
		printf("\nName is not all uppercase letters.\n");
		return 0;
	} else{
	    strcpy(currentUser->name, name);
	    getStatistics(currentUser);
	    return 1;
	}
}

void newProfile(profile *currentUser, profileList users){
	char name[21];
	char filename[21];
    char path[] = USER_PATH;
    char gamePath[] = GAME_PATH;
    int num, i;
    FILE *user;
    FILE *dir;

	printf("[PROFILE CREATION]\nEXISTING USER PROFILES:\n");
	checkProfiles(users);
	
	dir = fopen(USER_DIR, "r");
    fscanf(dir, "%d", &num);
    fclose(dir);
    
    if(num > 9){
		printf("\nMax number of profiles reached. (10 profiles)\n");
		return;
	}

    printf("\nName must be:\n[1] 3 to 20 characters\n[2] Uppercase letters only\n\nProvide profile name: ");
	scanf("%s", name);
	strcpy(filename, name);
	strcat(filename, ".txt");
	strcat(path, filename);

    if(strlen(name) > 20){
    	printf("Name is over 20 characters.\n");
    	return;
	}else if(strlen(name) < 3){
		printf("Name is less than 3 characters.\n");
    	return;
	}else if(fileExists(path) != 0) {
        printf("Profile already exists.\n");
	}else if(!(checkCapital(name))){
		printf("Name is not all uppercase letters.\n");
	}else{
        printf("\nUser profile [%s] created.\n\n", name);

		// read list of profiles
		dir = fopen(USER_DIR, "r");
    	fscanf(dir, " %d", &num);
    	for(i = 0; i < num; i++){
			fscanf(dir, "%s", users[i].name);
		}
		fclose(dir);

		// write list
    	dir = fopen(USER_DIR, "w");
    	fprintf(dir, "%d\n", num + 1);
    	for(i = 0; i < num; i++){
			fprintf(dir, "%s\n", users[i].name);
		}
		fprintf(dir, "%s\n", name);
    	fclose(dir);
    	

    	user = fopen(path, "w");
		// user details
		fprintf(user, "%s\n", name);
		// game details
		fprintf(user, "%d\n", 0);
		fprintf(user, "%d\n", 0);
		fprintf(user, "%d\n", 0);
		fprintf(user, "%d\n", 0);
		// recent games
		for(i = 0; i < 3; i++){
			fprintf(user, "%s%s_snapshot%d.txt\n", GAME_PATH, name, i);
		}
    	fclose(user);
    	// make recent games files
    	for(i = 0; i < 3; i++){
    		strcpy(gamePath, GAME_PATH);
			strcat(gamePath, name);
			if (i == 0){
				strcat(gamePath, "_snapshot0.txt");
			}
			if (i == 1){
				strcat(gamePath, "_snapshot1.txt");
			}
			if (i == 2){
				strcat(gamePath, "_snapshot2.txt");
			}
			FILE *recent = fopen(gamePath, "w");
			fclose(recent); 
		}
    	strcpy(currentUser->name, name);
	}
}

void deleteProfile(profile *currentUser, profileList users){
	char name[21];
	char filename[21];
    char path[] = USER_PATH;
    char gamePath[] = GAME_PATH;
    int num, i;
    FILE *dir;

	printf("[PROFILE DELETION]\nEXISTING USER PROFILES:\n");
	checkProfiles(users);

	printf("\nSELECT PROFILE TO DELETE: ");
	scanf(" %s", name);
	strcpy(filename, name);
	strcat(filename, ".txt");
	strcat(path, filename);

	if(fileExists(path) == 0) {
        printf("\nUser does not exist. Try again.\n\n");
        return;
    } else if(!(checkCapital(name))){
		printf("Name is not all uppercase letters.\n");
	} else {
		// read list
	    dir = fopen(USER_DIR, "r");
	    fscanf(dir, " %d", &num);
	    for(i = 0; i < num; i++){
			fscanf(dir, "%s", users[i].name);
		}
		fclose(dir);

		// update list
	    dir = fopen(USER_DIR, "w");
	    fprintf(dir, "%d\n", num - 1);
	    for(i = 0; i < num; i++){
	    	if(strcmp(users[i].name, name) == 0){
	    		fprintf(dir, "%s", "");
			} else fprintf(dir, "%s\n", users[i].name);
		}
	    fclose(dir);
	    
	    // switch current user to none if currentUser == name
		if (strcmp(currentUser->name, name) == 0){
			strcpy(currentUser->name, "");
			//selectProfile(currentUser, users);
		}
	
	    // delete user snapshots | WORKING
		for(i = 0; i < 3; i++){
			strcpy(gamePath, GAME_PATH);
			strcat(gamePath, name);
			if (i == 0){
				strcat(gamePath, "_snapshot0.txt");
			}
			if (i == 1){
				strcat(gamePath, "_snapshot1.txt");
			}
			if (i == 2){
				strcat(gamePath, "_snapshot2.txt");
			}
			remove(gamePath);
		}
	    
		// delete user file | WORKING
		strcpy(filename, name);
		strcat(filename, ".txt");
		strcpy(path, USER_PATH);
		strcat(path, filename);
		remove(path);
		
		printf("\nProfile [%s] deleted successfully.\n\n", name);
	}
}

void changeProfile(profile *currentUser, profileList *users){
	int choice, quit;

	while(!quit){
		menuProfile(*currentUser, &choice);
		switch(choice) {
            case 1:
                selectProfile(currentUser, users); // WORKING
                break;
            case 2:
                newProfile(currentUser, *users); // WORKING
                break;
            case 3:
            	deleteProfile(currentUser, *users); // WORKING
                break;
            case 4:
            	printf("Returned to main menu.\n");
				quit = 1;
				break;
			default:
				printf("Invalid selection. Please choose again.\n");
		}
	}
	
}

/* play */

void playCustom(game *customLevel, profile *currentUser, customLevelList *cLevels){
	char filename[20];
    char path[100] = LVL_PATH;
    FILE *chosenLevel;
    customLevel->mines = 0;

	printf("EXISTING CUSTOM LEVELS:\n");
	checkLevels(cLevels);

	printf("\nSELECT CUSTOM LEVEL: ");
	scanf("%s", filename);
    strcat(filename, ".txt");
    strcat(path, filename);
    printf("%s\n", path);

    if(fileExists(path) == 0) {
        printf("\nLevel does not exist. Try again.\n");
        return;
    } else {
    	chosenLevel = fopen(path, "r");

		// read level data
		fscanf(chosenLevel, "%d %d", &customLevel->rows, &customLevel->cols);
		for(int i = 0; i < customLevel->rows; i++) {
			fscanf(chosenLevel, "%s", customLevel->board[i]);
    	}
    	
    	// count mines
	    for (int i = 0; i < customLevel->rows; i++) {
	        for (int j = 0; j < customLevel->cols; j++) {
	            if (customLevel->board[i][j] == 'X') {
        			customLevel->mines++;
    			}
	        }
	    }
    	fclose(chosenLevel);
		
		strcpy(customLevel->mode, CUSTOM);
    	gameProper(*customLevel, currentUser);
	}	
}

void playClassic(game *level, profile *currentUser){
	int classicSelect;
	int validChoice = 0;
	
	while(!validChoice){
	printf("\nChoose difficulty\n[1] EASY\t[2] DIFFICULT\n\t[0] BACK\n\n");
	printf("SELECTION: ");
	scanf(" %d", &classicSelect);
	
	switch (classicSelect)
	{
		case 1: 
			level->cols = 8;
			level->rows = 8;
			level->mines = 10;
			strcpy(level->mode, CLASSIC_EASY);
			makeBoard(level);
			gameProper(*level, currentUser);
			validChoice = 1;
			break;
		case 2:
			level->cols = 15;
			level->rows = 10;
			level->mines = 35;
			strcpy(level->mode, CLASSIC_DIFFICULT);
			makeBoard(level);
			gameProper(*level, currentUser);
			validChoice = 1;
			break;
		case 0:
			printf("You have opted to go back.\n\n");
			validChoice = 1;
			break;
		default:
			printf("Invalid input. Try again.\n");
	}
	}
}

void play(profile currentUser, game level, game customLevel, customLevelList *cLevels)
{
	int gameSelect;
	int validChoice = 0;

	do {
	printf("\nGame type selection\n[1] CLASSIC\t[2] CUSTOM\n\t[0] BACK\n\nSELECTION: ");
	scanf("%d", &gameSelect);

	switch (gameSelect)
	{
		case 1:
			playClassic(&level, &currentUser);
			validChoice = 1;
			break;
		case 2:
			playCustom(&customLevel, &currentUser, cLevels);
			validChoice = 1;
			break;
		case 0:
			printf("You have opted to go back.\n\n");
			validChoice = 1;
			break;
		default:
			printf("Invalid input. Try again.\n");
	}
	} while(!validChoice);
}

void startMenu(profile *currentUser, profileList *users){
	int num, valid = 0;
	FILE *dir;
	
	printf("Welcome to Minesweeper!\n");
	delay(300);
	
	dir = fopen(USER_DIR, "r");
	fscanf(dir, " %d", &num);
	fclose(dir);

	if(num == 0){
		newProfile(currentUser, *users);
	} else {
		while(!valid){
			valid = selectProfile(currentUser, users);
		}
			getStatistics(currentUser);
	}
}

/* GAME PROPER */
int main(){
	// initalize variables
	profile currentUser;
	profileList users;

	game level;
	game customLevel;
	customLevelList customLvls;

	// start
	int menuSelect;
	int start = 0;

	startMenu(&currentUser, &users);
	delay(300);
	system("cls");
	
    do {
	printf("\nMain Menu\n[1] PLAY\t\t[2] LEVEL EDITOR\n[3] CHANGE PROFILE\t[4] VIEW STATISTICS \n[0] QUIT\n\nSELECTION: ");
	scanf("%d", &menuSelect);

	switch (menuSelect){
		case 1:
			play(currentUser, level, customLevel, &customLvls);
			break;
		case 2:
			levelEditor(&customLevel, &customLvls);
			break;
		case 3:
			changeProfile(&currentUser, &users);
			break;
		case 4:
			viewStatistics(&currentUser);
			break;
		case 0:
			start = 1;
			break;
		default:
			printf("Invalid input. Try again.");
	}
	}while(!start);
}

/*
	This is to certify that this project is my own work, based on my personal
	efforts in studying and applying the concepts learned. I have constructed
	the functions and their respective algorithms and corresponding code by
	myself. The program was run, tested, and debugged by my own efforts.
	I further certify that I have not copied in part or whole or otherwise
	plagiarized the work of other students and/or persons.

	GRINO, MARY EUNICE E., DLSU ID# 12325872
	TAMONDONG, MARIEL M., DLSU ID# 12308323
*/
