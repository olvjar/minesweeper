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

#define LVL_PATH "levels/"
#define LVL_DIR "levels/level_list.txt"
#define HIDDEN 10
#define FLAG 100
#define CLASSIC_EASY "CLASSIC-EASY"
#define CLASSIC_DIFFICULT "CLASSIC-DIFFICULT"
#define CUSTOM "CUSTOM"

// PREPROCESSOR DIRECTIVES
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

struct recent_games{
		char mode[11];
		char outcome[11];
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

// FUNCTIONS

/* board */

void printBoard(game level){
    int i, j;
    
    printf("\n");
	printf("   ");
	for (i = 0; i < level.cols; i++) {
		iSetColor(I_COLOR_PURPLE);
    	printf(" %d ", i);
    }
	
	printf("\n");
	
	for (i = 0; i < level.rows; i++) {
        for (j = -1; j < level.cols; j++) {
			if (j == -1){
				iSetColor(I_COLOR_PURPLE);
				printf(" %d ", i);
				iSetColor(I_COLOR_WHITE);
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
            level->gameBoard[i][j] = mineCount(*level, i, j); //reveal chosen tile
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

int saveSnapshot(game level, char outcome[]){
	int i, j;
	char filename[20];
    char path[] = "recent games/";
    FILE *fgame;
    
    strcpy(filename, "recentgame");
	strcat(filename, ".txt");
    strcat(path, filename);
    
    fgame = fopen(path, "w");
    
    if (strcmp(outcome, "win") == 0){
		fprintf(fgame, "%s", level.mode);
		fprintf(fgame, "GAME WON\n");
		
		for (i = 0; i < level.rows; i++){
    		for (j = 0; j < level.cols; j++){
    			
    			if (level.gameBoard[i][j] != FLAG && level.board[i][j] != 'X'){ // tile != flag && tile != bomb
					fprintf(fgame, " %d ", level.gameBoard[i][j]);
				}
				else if (level.board[i][j] == 'X' || level.gameBoard[i][j] == FLAG){ // tile == bomb || tile == flag (game only wins if all tiles are shown, so a flag == bomb)
					fprintf(fgame, " X ");
				}
		}
		fprintf(fgame, "\n");
	}
}
	
	else if (strcmp(outcome, "lose") == 0){
		fprintf(fgame, "%s", level.mode);
		fprintf(fgame, "GAME LOST\n");
		
		for (i = 0; i < level.rows; i++){
    		for (j = 0; j < level.cols; j++){
    			if (level.gameBoard[i][j] != HIDDEN && level.gameBoard[i][j] != FLAG && level.gameBoard[i][j] != 999){ // tile != hidden && tile != flag && tile != bombExploded
					fprintf(fgame, " %d ", level.gameBoard[i][j]);
				}
				else if(level.gameBoard[i][j] == 999){ // bomb == exploded
					fprintf(fgame, " X ");
				}
				else if (level.board[i][j] == 'X'){ // tile == bomb
					fprintf(fgame, " x ");
				}
				else if (level.gameBoard[i][j] == FLAG){ // tile == flag
    				fprintf(fgame, " F ");
				}
				else if (level.gameBoard[i][j] == HIDDEN){ // tile == not revealed
					fprintf(fgame, " . ");
    			}
			}
		fprintf(fgame, "\n");
		}
}
	
	else if (strcmp(outcome, "quit") == 0){
		fprintf(fgame, "%s", level.mode);
		fprintf(fgame, "GAME QUIT\n");
		
		for(i = 0; i < level.rows; i++){
			for(j = 0; j < level.cols; j++){
				
				if(level.gameBoard[i][j] == HIDDEN){
					fprintf(fgame, " . ");
				}
				else if (level.gameBoard[i][j] == FLAG){
					fprintf(fgame, " F ");
				}
				else fprintf(fgame, " %d ", level.gameBoard[i][j]);
			}
		fprintf(fgame, "\n");
		}
	}
    
    fclose(fgame);
    
    return 1;
}

void gameProper(game level){
	int i, j, alive = 1;
	int choice;
	char outcome[11];

	//PLAYER BOARD
	for(i = 0; i < level.rows; i++){
		for (j = 0; j < level.cols; j++){
            level.gameBoard[i][j] = HIDDEN; 
        }
    }
    
    printBoardChar(level); // FOR TESTING
	
	while(alive){
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
				printf("Game Quitted\n");
				alive = 0;
				break;
			default:
				printf("Invalid input. Please try again.\n");
		}
		
		if (alive){
		alive = gameChecker(level, outcome);
		}
	}
	saveSnapshot(level, outcome);
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

void printBoardCharEdit(game *customLevel) {
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

void placeMine(game *customLevel, int *minesCount) {
    int row, col;
	*minesCount = customLevel->mines;
	
    printf("Enter row and column for mine %d: ", *minesCount + 1);
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
	*minesCount = customLevel->mines;
	
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

void checkFiles(customLevelList *cLevels){
	int i, numFiles;
	FILE *dir;
	
	dir = fopen(LVL_DIR, "r");
	fscanf(dir, "%d", &numFiles);
	
	for(i = 0; i < numFiles; i++){
		fscanf(dir, "%s", cLevels[i]->filename);
		printf("%d. ", i + 1);
		
		if(strcmp(cLevels[i]->filename, "") == 0){
			printf("< empty >\n");
		} else printf("%s\n", cLevels[i]->filename);
	}
	
	fclose(dir);
}


void deleteFile(customLevelList *cLevels){
	char filename[20];
	char path[100] = LVL_PATH;
	int num, i;
	FILE *dir;
	
	printf("EXISTING CUSTOM LEVELS:\n");
	checkFiles(cLevels);
	
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
    	printBoardCharEdit(customLevel);
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
        
    	//iClear(0,0,100,30);
	}
	
	return save;
}

void loadLevel(game *customLevel, customLevelList *cLevels){
	char filename[20];
    char path[100] = LVL_PATH;
    FILE *level;
	
	printf("EXISTING CUSTOM LEVELS:\n");
	checkFiles(cLevels);
	
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
        
    	//iClear(0,0,100,30);
	}
}

/* play */

void playCustom(game *customLevel){
	//int row, col;
	char filename[20];
    char path[] = "levels/";
	FILE *chosenLevel;

	printf("Provide level name to play: ");
	scanf("%s", filename);
    strcat(filename, ".txt");
    strcat(path, filename);
    printf("%s\n", path);
    
    
    if(fileExists(path) == 0) {
        printf("\nLevel does not exist. Try again.\n");
        return;
    } else {
    	chosenLevel = fopen(path, "r");
    	
    	fscanf(chosenLevel, "%d %d", &customLevel->rows, &customLevel->cols);
    	// customLevel->rows = row;
    	// customLevel->cols = col;
		for (int i = 0; i < customLevel->rows; i++) {
			fscanf(chosenLevel, "%s", customLevel->board[i]);
    	}
    	fclose(chosenLevel);
		
		strcpy(customLevel->mode, CUSTOM);
    	gameProper(*customLevel);
	}	
}

void playClassic(game *level){
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
			gameProper(*level);
			validChoice = 1;
			break;
		case 2:
			level->cols = 10;
			level->rows = 15;
			level->mines = 35;
			strcpy(level->mode, CLASSIC_DIFFICULT);
			makeBoard(level);
			gameProper(*level);
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

void play(profile user, game level, game customLevel)
{
	int gameSelect;
	int validChoice = 0;

	do {
	printf("\nGame type selection\n[1] CLASSIC\t[2] CUSTOM\n\t[0] BACK\n\nSELECTION: ");
	scanf("%d", &gameSelect);

	switch (gameSelect)
	{
		case 1:
			playClassic(&level);
			validChoice = 1;
			break;
		case 2:
			playCustom(&customLevel);
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


// GAME PROPER
int main(){
	// initalize variables
	profile user;
	game customLevel;
	game level;
	customLevelList customLvls;

	// start
	int menuSelect;
	int start = 0;

    do {
	printf("Main Menu\n[1] PLAY\t\t[2] LEVEL EDITOR\n[3] CHANGE PROFILE\t[4] VIEW STATISTICS \n[0] QUIT\n\nSELECTION: ");
	scanf("%d", &menuSelect);

	switch (menuSelect){
		case 1:
			play(user, level, customLevel);
			break;
		case 2:
			levelEditor(&customLevel, &customLvls);
			break;
		case 3:
			//changeProfile(&user);
			break;
		case 4:
			//viewStats(&user);
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

