#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "interface.c"

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

struct level {
    int rows;
    int cols;
    int mines;
	char mode[30];
    int gameBoard[100][100];
    char board[100][100];
};

typedef struct level game;


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
	
	printf("copy success");
	
	fclose(fsource);
	fclose(fdest);
}

int saveSnapshot(game level, char outcome[], profile *currentUser){
	int i, j;
	char SS0[] = GAME_PATH; // LATEST
	char SS1[] = GAME_PATH;
	char SS2[] = GAME_PATH;
    FILE *fgame;
    
    strcat(SS0, currentUser->name);
	strcat(SS0, "0");
    strcat(SS0, ".txt");
	strcat(SS1, currentUser->name);
	strcat(SS1, "1");
    strcat(SS1, ".txt");
	strcat(SS2, currentUser->name);
	strcat(SS2, "2");
    strcat(SS2, ".txt");

	transferSnapshot(SS2, SS1); // overwrites 2 with 1
	transferSnapshot(SS1, SS0); // overwrites 1 with 0
    
	// 0 will now be overwritten with latest game
    fgame = fopen(SS0, "w");
    
    if (strcmp(outcome, "win") == 0){
		fprintf(fgame, "WIN\n");
		fprintf(fgame, "%s %d %d", level.mode, level.rows, level.cols);
		
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
		fprintf(fgame, "\nLOSE\n");
		fprintf(fgame, "%s %d %d\n", level.mode, level.rows, level.cols);
		
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
		fprintf(fgame, "QUIT\n");
		fprintf(fgame, "%s %d %d\n", level.mode, level.rows, level.cols);

		
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
				printf("Game QUIT\n");
				alive = 0;
				break;
			default:
				printf("Invalid input. Please try again.\n");
		}
		
		if (alive){
		alive = gameChecker(level, outcome);
		}
	}
	saveSnapshot(level, outcome, currentUser);
}


// GAME PROPER
int main(){
	// initalize variables
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
			play(level);
			break;
		case 2:
			//levelEditor(&customLevel, &customLvls);
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

