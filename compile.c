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

// PREPROCESSOR DIRECTIVES
struct level{
	int rows;
	int cols;
	int mines;
	int gameBoard[100][100];
	char board[100][100];
};

typedef struct level game;

struct recent_games{
		char mode;
		char outcome;
		char snapshot[100][100];
};

struct player{
	char name[20];
	int games_won_classic;
	int games_lost_classic;
	int games_won_custom;
	int games_lost_custom;
	struct recent_games recentgame;
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
			else if (level.gameBoard[i][j] == 10){ //not revealed
				iSetColor(I_COLOR_WHITE);
				printf(" . ");
			}
			else if (level.gameBoard[i][j] == 100){ //flag
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

void forwardCascade(game *level, int i, int j){
	int k, l, mine = 0;
	
	for(k = i; k < level->rows; k++){ //forward cascading
		for(l = j; l < level->cols && !mine; l++){
			if(mineCount(*level, k, l) != -1){
				level->gameBoard[k][l] = mineCount(*level, k, l);
			}
			else mine = 1;
		}
	}
}

void backwardCascade(game *level, int i, int j){
	int k, l, mine = 0;
	
	for(k = i; k >= 0; k--){ //backward cascading
		for(l = j; l >= 0 && !mine; l--){
			if(mineCount(*level, k, l) != -1){
				level->gameBoard[k][l] = mineCount(*level, k, l);
			}
			else mine = 1;
		}
	}
}

void placeFlag(game *level){
	int i, j;
	
	printf("\nEnter row to flag: ");
	scanf(" %d", &i);
	printf("Enter column to flag: ");
	scanf(" %d", &j);
	
	if(level->gameBoard[i][j] == 10){
		level->gameBoard[i][j] = 100;
	}
	else if (level->gameBoard[i][j] == 100)
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
	
	if(level->gameBoard[i][j] == 100){
		level->gameBoard[i][j] = 10;
	}
	else if (i >= level->rows || j >= level->cols)
		printf("Input is out of bounds\n");
	else printf("Tile is not flagged. Try again.\n");
}

int inspectBoard(game *level) {
    int i, j;
    
    printf("\nEnter row to inspect: ");
    scanf(" %d", &i);
    printf("Enter column to inspect: ");
    scanf(" %d", &j);
    
    if (i < level->rows && j < level->cols) {
        if (mineCount(*level, i, j) == -1) {
            printf("\nYou have hit a mine. Game over.\n");
            return 0;    
        } else {
            level->gameBoard[i][j] = mineCount(*level, i, j); //reveal chosen tile
            forwardCascade(level, i, j);
            backwardCascade(level, i, j);
            return 1;
        }
    } else {
        printf("Invalid input. Try again.\n");
        return 1;
    }
}

void gameProper(game level){
	int i, j, alive = 1;
	int choice;
	int totalSquares = (level.rows*level.cols)-level.mines;
	int revealedCount;

	//PLAYER BOARD
	for(i = 0; i < level.rows; i++){
		for (j = 0; j < level.cols; j++){
            level.gameBoard[i][j] = 10; // 10 == HIDDEN
        }
    }
    
    printBoardChar(level); // FOR TESTING
	
	while(alive){
		printBoard(level);
		printf("\n[1] INSPECT\n[2] FLAG\n[3] REMOVE FLAG\n\nSelection: ");
		scanf(" %d", &choice);
		
		switch (choice){
			case 1:
				alive = inspectBoard(&level);
				break;
			case 2:
				placeFlag(&level);
				break;
			case 3:
				removeFlag(&level);
				break;
			default:
				printf("Invalid input. Please try again.\n");
		}
		
		revealedCount = 0;
		for(i = 0; i < level.rows; i++){
			for(j = 0; j < level.cols; j++){
				if (level.gameBoard[i][j] != 10 && level.board[i][j] != 'X'){
					revealedCount++;
				}
			}
		}
		
		if(revealedCount == totalSquares){
			printBoard(level);
			printf("\nAll non-mine tiles revealed.\nYou win!\n");
			printBoardChar(level);
			alive = 0;
		}
	}
}

/* level edit */

void menuLevelEditor(int *choice){
	printf("[1] PLACE mine\n[2] DELETE mine\n[3] SAVE\n[4] RETURN to main menu\n\nSelection: ");
	scanf("%d", choice);
}

void printBoardCharEdit(game *customLevel) {
    for (int i = 0; i < customLevel->rows; i++) {
        for (int j = 0; j < customLevel->cols; j++) {
            printf(" %c ", customLevel->gameBoard[i][j]);
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

    while(quit != 1){
    	printBoardCharEdit(customLevel);
    	printf("MINES: %d\n", minesCount);
		menuLevelEditor(&choice);
    	
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
    } else {
        printf("Level %s will be created.\n", filename);

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
        
        if(editLevel(customLevel) == 1){
    		level = fopen(path, "w");
    		saveFile(level, customLevel);
    		fclose(level);
        	printf("Level created successfully.\n\n");
		} else{
			printf("Level was not saved.\n\n");
		}
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
			
    	gameProper(*customLevel);
	}	
}

void playClassic(game *level){
	int classicSelect;
	int validChoice = 0;
	
	while(!validChoice){
	printf("\nChoose difficulty\n[1] EASY or [2] DIFFICULT\n\n");
	printf("Selection: ");
	scanf(" %d", &classicSelect);
	
	switch (classicSelect)
	{
		case 1: 
			level->cols = 8;
			level->rows = 8;
			level->mines = 10;
			makeBoard(level);
			gameProper(*level);
			validChoice = 1;
			break;
		case 2:
			level->cols = 10;
			level->rows = 15;
			level->mines = 35;
			makeBoard(level);
			gameProper(*level);
			validChoice = 1;
			break;
		default:
			printf("Invalid input. Try again.");
	}
	}
}

void play(profile user, game level, game customLevel)
{
	int gameSelect;
	int validChoice = 0;

	do {
	printf("\nGame type selection\n[1] CLASSIC or [2] CUSTOM\n\nSELECTION: ");
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
		default:
			printf("Invalid input. Try again.");
	}
	} while(!validChoice);
}


// GAME PROPER
int main(){
	// initalize variables
	profile user;
	game customLevel;
	game level;

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
			levelEditor(&customLevel);
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

