/*
	Description: This program simulates the game Minesweeper, a classic single player computer game.
				 The goal of the game is to uncover all of the non-mine tiles, without being blown up by a mine.
				 This program has 2 game modes: classic and custom. Classic randomly generates a board each time, depending on the difficulty.
				 Meanwhile, custom allows for the user to create their own levels.
				 This program also includes player profiles that save the statistics of a player and the snapshots of their 3 most recent games.
	Author: GRINO, Mary Eunice E. and TAMONDONG, Mariel M.
	Last Modified: March 31, 2024
	Acknowledgments: Thomas Tiam-Lee
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#include "interface.c"
#include "ascii.c"

#define CLEARSCREEN system("cls")

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
    int gameBoard[10][15];
    char board[10][15];
};

struct fileInfo{
	char filename[21];
	char path[100];
};

typedef struct level game;
typedef struct fileInfo file;
typedef file customLevelList[20];
typedef game recentGame;

struct recent_games{
	char path[100];
	char outcome[50];
	char mode[50];
	int time;
	int rows;
	int cols;
	char snapshot[10][15];
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

struct scoreboard{
		char user[21];
		int time;
};

typedef struct scoreboard leaderboard[3];

// FUNCTIONS

/*	
	This function delays the execution of preceding the code
	@param time - an integer that pertains to the delay length
	@return void
	Pre-condition: time is a nonnegative integer
*/
void delay(int time){
	clock_t delay;
	delay = clock() + time;
	while(delay > clock());
}

/* controls */

/*	
	This function allows the user to choose their input in a menu using arrow keys
	@param *cont - an integer pointer that determines the continuation of a loop
	@param selection - an integer that pertains to the current selection of the arrow
	@param max - an integer that pertains to the max number of choices in a selection

	@return the selection of the user
	
	Pre-condition: *cont == 1, selection is a nonnegative integer, and max is a nonnegative integer
*/

int controlsMenu(int *cont, int selection, int max){
	int input;
	
	input = getch();
	// up and down arrow keys
	if (input == 224){
		input = getch();
		if (input == 72 && selection > 0){
			selection--;
		} else if (input == 80 && selection < max - 1){
			selection++;
		} else if (input == 75 || input == 77) {
			printf("\nOnly use up and down arrow keys.\n");
			delay(500);
		}
	}
	else if (input == '\r'){
		*cont = 0;
		}
	else {
		printf("\nInvalid input. Use arrow keys.\n");
		delay(500);
	}
	
	return selection;
}

/*	
	This function allows the user to choose their input in the game using arrow keys
	@param level - a structure that pertains to the information of the level
	@param *rowChosen - an integer pointer that holds the value of the row index of the array
	@param *colChosen - an integer pointer that holds the value of the column index of the array

	@return void
	
	Pre-condition: level has all members initialized
*/

int controlsGame(game level, int *rowChosen, int *colChosen, time_t timeStart, int *timeElapsed){
	int input;
    int cont = 1;
    int i, j;
    int row = *rowChosen;
    int col = *colChosen;
	int choice, hours, minutes, seconds;
	time_t timeEnd;

	while (cont) {
		CLEARSCREEN;
		
			time(&timeEnd);
	        *timeElapsed = difftime(timeEnd, timeStart);
		hours = *timeElapsed / 3600;
	        minutes = (*timeElapsed % 3600) / 60;
	        seconds = *timeElapsed % 60;
		
	    
	    printf("\t [ARROW KEYS]\n[1] INSPECT       [2] FLAG\n[3] REMOVE FLAG   [ESC] QUIT \n\n");
	    printf("\nTIME: %02d:%02d:%02d\n", hours, minutes, seconds);
	    
	    
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
					}
				else if (j == level.cols){
					iSetColor(I_COLOR_PURPLE);
					printf(" | ");
				}
				else if (i == row && j == col){
	                if (level.gameBoard[i][j] == HIDDEN){ //not revealed
					    iSetColor(I_COLOR_WHITE);
					    printf(">.<");
				    }
				    else if (level.gameBoard[i][j] == FLAG){ //flag
					    iSetColor(I_COLOR_CYAN);
					    printf(">F<");
				    }
				    else{
					    iSetColor(I_COLOR_GREEN);
					    printf(">%d<", level.gameBoard[i][j]); //valid space & revealed
				    }
	            } else {
	                if (level.gameBoard[i][j] == HIDDEN){ //not revealed
					    iSetColor(I_COLOR_WHITE);
					    printf(" . ");
			    	}
				    else if (level.gameBoard[i][j] == FLAG){ //flag
				    	iSetColor(I_COLOR_CYAN);
				    	printf(" F ");
				    }
			    	else{
				    	iSetColor(I_COLOR_GREEN);
				    	printf(" %d ", level.gameBoard[i][j]); //valid space & revealed
				    }
					
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
	
		
	        input = getch(); 
	        if (input == 224) { // arrow key entered
	            input = getch(); 
	            switch(input) {
	                case 72:
	                    if (row > 0){
							row--;
						}
	                    break;
	                case 80:
	                    if (row < level.rows-1){
							row++;
						}
	                    break;
	                case 75:
	                    if (col > 0){
	                    	col--;
						} 
	                    break;
	                case 77:
	                    if (col < level.cols-1){
	                    	col++;
						} 
	                    break;
	                default:
	                    printf("Unknown key pressed. Please press arrow keys.\n");
	                    delay(500);
	                    break;
	            }
	        } else if (input == '1' || input == '2' || input == '3' || input == 27) {
	            cont = 0;
				if (input == '1') choice = 0;
				if (input == '2') choice = 1;
				if (input == '3') choice = 2;
				if (input == 27) choice = 3;
	        }
	        else {
				printf("Invalid input. Use arrow keys.\n");
				delay(500);
		    }
    }

	*rowChosen = row;
	*colChosen = col;
	return choice;
}

/*	
	This function allows the user to choose their input in the level editor using arrow keys
	@param level - a structure that pertains to the information of the level
	@param *rowChosen - an integer pointer that holds the value of the row index of the array
	@param *colChosen - an integer pointer that holds the value of the column index of the array

	@return void
	
	Pre-condition: level has all members initialized
*/

void controlsLevelEdit(game level, int *rowChosen, int *colChosen){
	int input;
    int cont = 1;
    int i, j;
    int row = 0;
    int col = 0;


	while (cont) {
		CLEARSCREEN;
	    
	    printf("Use [ARROW KEYS] and [ENTER] to select\n\n");
	    
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
					}
				else if (j == level.cols){
					iSetColor(I_COLOR_PURPLE);
					printf(" | ");
				}
				else if (i == row && j == col){
	                if (level.board[i][j] == 'X'){ 
					    iSetColor(I_COLOR_RED);
					    printf(">X<");
				    } else{
					    iSetColor(I_COLOR_WHITE);
					    printf(">.<");
				    }
	            } else {
	                if (level.board[i][j] == 'X'){ 
					    iSetColor(I_COLOR_RED);
					    printf(" X ");
				    } else{
					    iSetColor(I_COLOR_WHITE);
					    printf(" . ");
					}
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
	
		
	        input = getch(); 
	        if (input == 224) { // arrow key entered
	            input = getch(); 
	            switch(input) {
	                case 72:
	                    if (row > 0){
							row--;
						}
	                    break;
	                case 80:
	                    if (row < level.rows-1){
							row++;
						}
	                    break;
	                case 75:
	                    if (col > 0){
	                    	col--;
						} 
	                    break;
	                case 77:
	                    if (col < level.cols-1){
	                    	col++;
						} 
	                    break;
	                default:
	                    printf("\nUnknown key pressed. Please press arrow keys.\n");
	                    break;
	            }
	        } else if (input == '\r') {
	            cont = 0;
	        }
	        else {
				printf("\nInvalid input. Use arrow keys.\n");
				delay(500);
		    }
    }

	CLEARSCREEN;
	*rowChosen = row;
	*colChosen = col;
}

/* board */

/*	
	This function prints the board that is visible to the player while playing
	@param level - a structure that pertains to the information of the level

	@return void
	
	Pre-condition: level has all members initialized
*/

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
				iSetColor(I_COLOR_CYAN);
				printf(" F ");
			}
			else{
				iSetColor(I_COLOR_GREEN);
				printf(" %d ", level.gameBoard[i][j]); //valid space & revealed
			}
				
        }
        printf("\n");
    }
    
    iSetColor(I_COLOR_PURPLE);
    for (i = 0; i < level.cols; i++) {
    	if (i == 0){
    		printf("   ");	
		} else printf("---");
	}
	printf("-------\n");
	iSetColor(I_COLOR_WHITE);
}

/*	
	This function prints the board that is not visible to the player (i.e. the board comprprised of '.' and 'X')
	@param level - a structure that pertains to the information of the level
	@return void
	
	Pre-condition: level has all memebers initialized
*/

void printBoardChar(game level){
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
				}
			else if (j == level.cols){
				iSetColor(I_COLOR_PURPLE);
				printf(" | ");
			}
			else if (level.board[i][j] == 'X'){ 
				iSetColor(I_COLOR_RED);
				printf(" X ");
			}
			else {
				iSetColor(I_COLOR_WHITE);
				printf(" . ");
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

/*	
	This function randomly generates a board, given the variables level->mines, level->cols, and level->rows
	@param *level - a structure pointer that pertains to the information of the level

	@return void
	
	Pre-condition: level has all members initialized, except for board[][] and gameBoard[][]
*/

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

/*	
	This function checks if a cell contains a mine. If not, it counts the amount of adjacent mines
	@param level - a structure that pertains to the information of the level
	@param i - the row index of the cell to inspect
	@param j - the column index of the cell to inspect

	@return -1 if the cell is a mine. If not, the amount of adjacent mines
	
	Pre-condition: level has a level.board initialized, and i and j are nonnegative integers
*/

int mineCount(game level, int i, int j){
	int count = 0;
	
	if(level.board[i][j] != 'X'){
		if(level.board[i-1][j] == 'X' && i > 0)
		count++;
		if(level.board[i+1][j] == 'X' && i < level.rows - 1)
		count++;
		if(level.board[i][j-1] == 'X' && j > 0)
		count++;
		if(level.board[i][j+1] == 'X' && j < level.cols - 1)
		count++;
		if(level.board[i-1][j-1] == 'X' && i > 0 && j > 0)
		count++;
		if(level.board[i-1][j+1] == 'X' && i > 0 && j < level.cols - 1)
		count++;
		if(level.board[i+1][j-1] == 'X' && i < level.rows - 1 && j > 0)
		count++;
		if(level.board[i+1][j+1] == 'X' && i < level.rows -1 && j < level.cols - 1)
		count++;
			
		return count;
		}
	else return -1;
}

/*	
	This is a recursive function that reveals the gameBoard in a cascading manner from the original tile
	@param *level - a structure pointer that pertains to the information of the level
	@param i - the row index of the cell to inspect
	@param j - the column index of the cell to inspect

	@return void
	
	Pre-condition: level has a level.board initialized, and i and j are nonnegative integers
*/

void cascade(game *level, int i, int j){
    if (i < 0 || i >= level->rows || j < 0 || j >= level->cols || 
        level->board[i][j] == 'X' || level->gameBoard[i][j] == FLAG || !(level->gameBoard[i][j] == HIDDEN)) {
        return; // base case: out of bounds or already revealed/flagged
    }
    
    level->gameBoard[i][j] = mineCount(*level, i, j); // reveal mine count
    
    if (level->gameBoard[i][j] == 0) { // if no neighboring mines
        cascade(level, i - 1, j); // up
        cascade(level, i + 1, j); // down
        cascade(level, i, j - 1); // left
        cascade(level, i, j + 1); // right
        cascade(level, i - 1, j - 1); // up left
        cascade(level, i - 1, j + 1); // up right
        cascade(level, i + 1, j - 1); // bottom left
        cascade(level, i + 1, j + 1); // bottom right
    }
}

/*	
	This function allows the player to place a flag on the gameBoard
	@param *level - a structure pointer that pertains to the information of the level

	@return void
	
	Pre-condition: level has all members initialized, except for gameBoard
*/

void placeFlag(game *level, int i, int j){

	if(level->gameBoard[i][j] == HIDDEN){
		level->gameBoard[i][j] = FLAG;
		return; 
	}
	else if (level->gameBoard[i][j] == FLAG)
		printf("Tile is already flagged. Try again.\n");
	else if (i >= level->rows || j >= level->cols)
		printf("Input is out of bounds\n");
	else printf("Tile is already revealed. Try again.\n");
	
	printf("Press any key to continue...");
	getch();
}

/*	
	This function allows the player to remove a flag on the gameBoard
	@param *level - a structure pointer that pertains to the information of the level

	@return void
	
	Pre-condition: level has all members initialized, except for gameBoard
*/

void removeFlag(game *level, int i, int j){
	
	if(level->gameBoard[i][j] == FLAG){
		level->gameBoard[i][j] = HIDDEN;
		return;
	}
	else if (i >= level->rows || j >= level->cols)
		printf("Input is out of bounds\n");
	else printf("Tile is not flagged. Try again.\n");
	
	printf("Press any key to continue...");
	getch();
}


/*	
	This function allows the player to inspect the board.
	It ends the game when the player inspects a mine, and triggers cascade if not.
	@param *level - a structure pointer that pertains to the information of the level
	@param outcome[] - a string that holds the outcome of the game

	@return the boolean value determining the game continues. 1 for true, 0 for false
	
	Pre-condition: level has all members initialized
*/

int inspectBoard(game *level, char outcome[], int time, int i, int j) {
    
    if (i < level->rows && j < level->cols && level->gameBoard[i][j] != FLAG) {
        if (mineCount(*level, i, j) == -1) {
            asciiGameLost(time);
            strcpy(outcome, "lose");
            level->gameBoard[i][j] = 999;
            printBoardChar(*level);
            return 0;    
        } else {
            cascade(level, i, j);
            return 1;
        }
    } else if (level->gameBoard[i][j] == FLAG) {
    	printf("You can not inspect a flagged tile.\n");
    	return 1;
	} else{
        printf("Invalid input. Try again.\n");
        return 1;
    }
}

/*	
	This function checks if the player wins the game, given that they have revealed all non-mine tiles
	@param level - a structure that pertains to the information of the level
	@param outcome[] - a string that holds the outcome of the game

	@return the boolean value determining the game continues. 1 for true, 0 for false
	
	Pre-condition: level has all members initialized and alive == 1
*/

int gameChecker(game level, char outcome[], int time){
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
		asciiGameWon(time);
		strcpy(outcome, "win");
		printBoardChar(level);
		return 0;
	}
	else return 1;
}

/*	
	This function checks if a file exists 
	@param *fileName - a string that holds the path of the file			 

	@return the boolean value if the file exists. 1 for true and 0 for false.
	
	Pre-condition: none
*/

int fileExists(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1; // exists
    } else {
    	fclose(file);
        return 0; // null
    }
}

/*	
	This function reads and writes a given snapshot of a recent game from one file to another.
	@param destFile[] - a string that holds the path of the destination of the file to be overwritten
	@param sourceFile[] - a string that holds the path of the file to be copied from

	@return void
	
	Pre-condition: destFile[] and sourceFile[] are valid paths
*/

void transferSnapshot(char destFile[], char sourceFile[]){
	FILE *fsource;
	FILE *fdest;
	char mode[21];
	char outcome[51];
	int rows, cols, time, i ,j;
	char val[10][15];
	
	if(fileExists(sourceFile) == 0) {
		return;
	}
	
	fsource = fopen(sourceFile, "r");
	fdest = fopen(destFile, "w");

	fscanf(fsource, " %s", outcome);
	
	if (strcmp(outcome, "0") == 0) {
		fprintf(fdest, "0");
		fclose(fsource);
		fclose(fdest);
		return;
	} 
	
	fscanf(fsource, " %d", &time);
	fscanf(fsource, " %s", mode);
	fscanf(fsource, " %d", &rows);
	fscanf(fsource, " %d", &cols);

	fprintf(fdest, "%s ", outcome);
	fprintf(fdest, "%d\n", time);
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

/*	
	This function saves a snapshot of the final board in a text file containing the player's most recent game
	@param level - a structure that pertains to the information of the level
	@param outcome[] - a string that holds the outcome of the game
	@param currentUser - a structure that contains the information of the current user
	@param time - an integer that holds the amount of time the game was played from start to end

	@return void
	
	Pre-condition: level has all members initialized, outcome is valid, currentUser is initialized, and time is a nonnegative integer
*/

void saveSnapshot(game level, char outcome[], profile currentUser, int time){
	int i, j;
    FILE *fgame;

	currentUser.recentgame[0].time = time;
	transferSnapshot(currentUser.recentgame[2].path, currentUser.recentgame[1].path); // overwrites 2 with 1
	transferSnapshot(currentUser.recentgame[1].path, currentUser.recentgame[0].path); // overwrites 1 with 0
    
	// 0 will now be overwritten with latest game
    fgame = fopen(currentUser.recentgame[0].path, "w");
    
    if (strcmp(outcome, "win") == 0){
		fprintf(fgame, "WON %d\n", currentUser.recentgame[0].time);
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
		fclose(fgame);
	}
	
	else if (strcmp(outcome, "lose") == 0){
		fprintf(fgame, "LOST %d\n", currentUser.recentgame[0].time);
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
		fclose(fgame);
	}
	
	else if (strcmp(outcome, "quit") == 0){
		fprintf(fgame, "QUIT %d\n", currentUser.recentgame[0].time);
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
		fclose(fgame);
	}
    
    fclose(fgame);
}

/*	
	This function updates the player's statistics after a game ends.
	@param level - a structure that pertains to the information of the level
	@param outcome[] - a string that holds the outcome of the game
	@param *currentUser - a structure pointer that contains the information of the current user

	@return void
	
	Pre-condition: level has all members initialized, outcome is valid, and currentUser is initialized
*/

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

/*	
	This function generates the gameBoard, keeps track of the time elapsed, 
	and continuously asks the player to inspect, flag, or remove a flag until the game ends or the player quits.
	After the game ending, it will save the snapshot of the board and updates the statistics of the user.
	@param level - a structure that pertains to the information of the level
	@param *currentUser - a structure pointer that contains the information of the current user

	@return void
	
	Pre-condition: level has all members initialized, *currentUser has all members initialized
*/

void gameProper(game level, profile *currentUser){
	int i, j, alive = 1;
	int choice = 0;
	char outcome[11];

	//PLAYER BOARD
	for(i = 0; i < level.rows; i++){
		for (j = 0; j < level.cols; j++){
            level.gameBoard[i][j] = HIDDEN; 
        }
    }
    
    CLEARSCREEN;
    time_t timeStart;
	int timeElapsed;
	
	i = j = 0;
	
	time(&timeStart);
	while(alive){
			//CLEARSCREEN;

		choice = controlsGame(level, &i, &j, timeStart, &timeElapsed);
		
		switch (choice){
			case 0:
				alive = inspectBoard(&level, outcome, timeElapsed, i ,j);
				break;
			case 1:
				placeFlag(&level, i, j);
				break;
			case 2:
				removeFlag(&level, i, j);
				break;
			case 3:
				CLEARSCREEN;
				strcpy(outcome, "quit");
				asciiGameQuit(timeElapsed);
				printBoardChar(level);
				alive = 0;
				break;
			default:
				printf("Invalid input. Please try again.\n");
		}
		
		if (alive){
		alive = gameChecker(level, outcome, timeElapsed);
		}
	}

	saveSnapshot(level, outcome, *currentUser, timeElapsed);
	updateStatistics(level, outcome, currentUser);
	printf("Press any key to continue...");
	getch();
}

/* level edit */

/*	
	This function prints the menu for editing a level and allows for arrow key input from the user
	@param customLevel - a structure pointer that pertains to the information of a custom level
	@param minesCount - an integer that holds the amount of mines of a custom level

	@return the selection of the user
	
	Pre-condition: *customLevel has all members initialized (except for gameBoard), minesCount is a nonnegative integer
*/

int menuEditLevel(game *customLevel, int minesCount){
	int i, selection = 0, cont = 1;
	
	while(cont){
		CLEARSCREEN;
		printf("\n");
    	printBoardChar(*customLevel);
    	printf("\nGRID: %dx%d\t", customLevel->rows, customLevel->cols);
    	printf("MINES: %d\n\n", minesCount);
    	printf("Use [ARROW KEYS] and [ENTER] to select\n\n");
		
		for(i = 0; i < 4; i++){
			if (i == selection){
				if (i == 0) printf(" > PLACE mine\n");
				if (i == 1) printf(" > DELETE mine\n");
				if (i == 2) printf(" > SAVE\n");
				if (i == 3) printf(" > GO BACK\n");
			} else {
				if (i == 0) printf("   PLACE mine\n");
				if (i == 1) printf("   DELETE mine\n");
				if (i == 2) printf("   SAVE\n");
				if (i == 3) printf("   GO BACK\n");
			}
		}
		selection = controlsMenu(&cont, selection, 4);
	}
	return selection;
}

/*	
	This function prints the menu for the level editor menu and allows for arrow key input from the user
	@return the selection of the user
	
	Pre-condition: none
*/

int menuLevelEditor(){
	int selection = 0, cont = 1;
	
	while(cont){
		CLEARSCREEN;
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx \n");
		printf("|                                    | \n");
		printf("|            LEVEL EDITOR            | \n");
		printf("|  --------------------------------  | \n");
		printf("|                                    | \n");
		printf("|     WHAT WOULD YOU LIKE TO DO?     | \n");
		printf("|                                    | \n");
		printf("|                                    | \n");
		renderMenuLevel(selection);
		printf("|                                    | \n");
		printf("|                                    | \n");
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx \n");
		printf("Use [ARROW KEYS] and [ENTER] to select\n\n");
	
		selection = controlsMenu(&cont, selection, 4);
	}
	return selection;
}

/*	
	This function opens the level directory, scans and puts them cLevels, and prints the levels
	@param *cLevels - a 1D structure array of that holds the information file name and path of each level

	@return the boolean value if the file exists. 1 for true and 0 for false.
	
	Pre-condition: the integer value of the first line in the directory is greater than or equal to 0
*/

int checkLevels(customLevelList *cLevels){
	int i, numFiles;
	FILE *dir;

	dir = fopen(LVL_DIR, "r");
	fscanf(dir, "%d", &numFiles);
	
	printf("|      EXISTING CUSTOM LEVELS        |\n");
	for(i = 0; i < numFiles; i++){
		fscanf(dir, " %s", cLevels[i]->filename);
		printf("|      %d. %-20s       |  \n", i + 1, cLevels[i]->filename);
	}

	fclose(dir);
	return numFiles;
}

void renderMenuLevelAsk(int mode, char *filename, customLevelList *cLevels) {
    int num;
	
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  \n");
    printf("|                                    |  \n");
    if(mode == 0) printf("|             PLAY CUSTOM            | \n");
    if(mode == 1) printf("|           LEVEL CREATION           | \n");
	if(mode == 2) printf("|           LEVEL DELETION           | \n");
	if(mode == 3) printf("|            LEVEL EDITOR            | \n");
    printf("|  --------------------------------  |  \n");
	printf("|                                    |  \n");
	num = checkLevels(cLevels);
	printf("|                                    |  \n");
    printf("|         PROVIDE FILE NAME          |  \n");
    if(mode == 0 || mode == 1) printf("|           (MAX 20 CHAR):           |  \n");
	if(mode == 2 || mode == 3) printf("|       (DO NOT INCLUDE .TXT):       | \n");
    printf("|                                    |  \n");
    printf("|                                    |  \n");
    printf("|                                    |  \n");
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  \n");
    iMoveCursor(11, 10+num);
    iShowCursor();
    scanf("%s", filename);
    iMoveCursor(0, 13+num);
    iHideCursor();
}
/*	
	This function allows the player to place a mine on the board
	@param *customLevel - a structure pointer that pertains to the information of a custom level
	@param *minesCount - an integer pointer that holds the amount of mines of a custom level
	
	@return void
	
	Pre-condition: level has all members initialized, except for gameBoard, and *minesCount is a nonnegative integer
*/

void placeMine(game *customLevel, int *minesCount) {
    int row, col;

	controlsLevelEdit(*customLevel, &row, &col);
    if (row >= 0 && row < customLevel->rows && col >= 0 && col < customLevel->cols && customLevel->board[row][col] == '.') {
        customLevel->board[row][col] = 'X'; // Place mine
        (*minesCount)++;
        printBoardChar(*customLevel);
        printf("\nMine placed.\n");
    } else {
        printBoardChar(*customLevel);
		printf("\nInvalid position. Mine not placed.\n");
    }
   
    printf("Press any key to continue...\n");
    getch();
}

/*	
	This function allows the player to delete a mine on the board
	@param *customLevel - a structure pointer that pertains to the information of a custom level
	@param *minesCount - an integer pointer that holds the amount of mines of a custom level
	
	@return void
	
	Pre-condition: level has all members initialized, except for gameBoard, and *minesCount is a nonnegative integer
*/

void deleteMine(game *customLevel, int *minesCount) {
    int row, col;

	controlsLevelEdit(*customLevel, &row, &col);
    if (row >= 0 && row < customLevel->rows && col >= 0 && col < customLevel->cols && customLevel->board[row][col] == 'X') {
        customLevel->board[row][col] = '.'; // Delete mine
        (*minesCount)--;
        printBoardChar(*customLevel);
		printf("\nMine removed.\n");
    } else {
        printBoardChar(*customLevel);
		printf("\nInvalid position. There is no mine.\n");
    }
    
    printf("Press any key to continue...\n");
    getch();
}

/*	
	This function checks if a custom level is valid for saving.
	A level is invalid if (1) every cell is a mine, and (2) there are no mines.
	@param *customLevel - a structure pointer that pertains to the information of a custom level
	@param *minesCount - an integer pointer that holds the amount of mines of a custom level
	
	@return a boolean value determining if a level is valid. 1 for true and 0 for false.
	
	Pre-condition: level has all members initialized, except for gameBoard, and *minesCount is a nonnegative integer
*/

int checkValidity(game *customLevel, int *minesCount){
	int cells = customLevel->rows * customLevel->cols;

	if(cells == *minesCount){
		printf("\nInvalid level: Every cell contains a mine.\n");
		return 0;
	} else if(*minesCount == 0){
		printf("\nInvalid level: There are no mines placed.\n");
		return 0;
	} else{
		return 1;
	}
}

/*	
	This function deletes a level from the level directory and also deletes the level file
	@param *cLevels - a 1D structure array of that holds the information file name and path of each level	 

	@return void
	
	Pre-condition: the integer value of the first line in the directory is greater than or equal to 0
*/

void deleteLevel(customLevelList *cLevels){
	char name[21];
	char filename[25];
	char path[100] = LVL_PATH;
	int num, i;
	FILE *dir;

	renderMenuLevelAsk(2, name, cLevels);
	strcpy(filename, name);
	strcat(filename, ".txt");
    strcat(path, filename);

	if(fileExists(path) == 0) {
        printf("\nLevel does not exist. Try again.\n");
        return;
    } else {
		// delete level file
		if (remove(path) != 0) {
        	perror("\nError deleting user file\n");
			return;
    	} else{
			printf("\n%s deleted successfully.\n", filename);
		}
	    remove(path);
		
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
			if(strcmp(cLevels[i]->filename, name) != 0){
				fprintf(dir, "%s\n", cLevels[i]->filename);
			}
		}
		fclose(dir);
	}
}

/*	
	This function saves the level. 
	It opens and reads the level directory and rewrites it with the addition of the file to be saved.
	@param mode - an integer that corresponds to whether the file is new or old
	@param *file - a file pointer that points to a file containing the custom level
	@param *customLevel - a structure pointer that pertains to the information of a custom level
	@param *cLevels - a 1D structure array of that holds the information file name and path of each level			 
	@param *fileName - a string that holds the path of the file

	@return void
	
	Pre-condition: there is no duplicate file name in the directory
*/

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

/*	
	This function directs to functions that edit a level, depending on the user's selection.
	Specifically, (1) placing a mine, (2) deleting a mine, and (3) saving the level. 
	It opens and reads the level directory and rewrites it with the addition of the file to be saved.
	@param *customLevel - a structure pointer that pertains to the information of a custom level
	@param *minesCount - an integer pointer that holds the amount of mines of a custom level

	@return an integer to determine if the level is valid. 1 for true
	
	Pre-condition: *customLevel has rows, cols, mode, and board initialized, and minesCount is a nonnegative integer
*/

int editLevel(game *customLevel, int minesCount){
    int save = 0;
    int quit = 0;
    int check;
    int choice = 0;

	CLEARSCREEN;
    while(!quit){
		choice = menuEditLevel(customLevel, minesCount);

        switch(choice){
            case 0:
				CLEARSCREEN;
				placeMine(customLevel, &minesCount);
                break;
            case 1:
            	CLEARSCREEN;
				deleteMine(customLevel, &minesCount);
                break;
            case 2:
				check = checkValidity(customLevel, &minesCount);
				
				if(check == 1) {
            		save = 1;
            		quit = 1;
            		return save;
                } else {
					printf("\nPress any key to continue...\n");
					getch();
				}
                break;
            case 3:
				printf("\nYou have opted to go back.\n");
				quit = 1;
				delay(500);
				break;
			default:
            	CLEARSCREEN;
				printf("Invalid selection. Please choose again.\n");
        }
	}

	return save;
}

/*	
	This function allows the player to edit a level that was already created.
	@param *customLevel - a structure pointer that pertains to the information of a custom level
	@param *cLevels - a 1D structure array of that holds the information file name and path of each level

	@return void
	
	Pre-condition: *customLevel has all members initialized (except for gameBoard), 
					and the integer value of the first line in the directory is greater than or equal to 0
*/

void loadLevel(game *customLevel, customLevelList *cLevels){
	char name[21];
	char filename[25];
    char path[100] = LVL_PATH;
    FILE *level;
    int minesCount = 0;
    int edit;

	renderMenuLevelAsk(3, name, cLevels);
	strcpy(filename, name);
    strcat(filename, ".txt");
    strcat(path, filename);

    if(fileExists(path) == 0) {
        printf("\nLevel does not exist. Try again.\n");
        return;
    } else {
    	level = fopen(path, "r");

		// read level data
		fscanf(level, "%d %d", &customLevel->rows, &customLevel->cols);
		for(int i = 0; i < customLevel->rows; i++) {
			fscanf(level, " %s", customLevel->board[i]);
    	}
    	
    	// count mines
	    for (int i = 0; i < customLevel->rows; i++) {
	        for (int j = 0; j < customLevel->cols; j++) {
	            if (customLevel->board[i][j] == 'X') {
        			minesCount++;
    			}
	        }
	    }
		fclose(level);

		edit = editLevel(customLevel, minesCount);

    	if(edit == 1){
    		level = fopen(path, "w");
    		saveFile(1, level, customLevel, cLevels, name);
    		fclose(level);
    		
    		CLEARSCREEN;
    		printBoardChar(*customLevel);
        	printf("\nLevel successfully edited.\n\n");
		} else{
			CLEARSCREEN;
			printBoardChar(*customLevel);
			printf("\nLevel was not saved.\n\n");
		}
	}
}

/*	
	This function allows the player to create a new custom level
	@param *customLevel - a structure pointer that pertains to the information of a custom level
	@param *cLevels - a 1D structure array of that holds the information file name and path of each level

	@return void
	
	Pre-condition: none
*/

void createLevel(game *customLevel, customLevelList *cLevels){
	char name[100];
	char filename[25];
    char path[100] = LVL_PATH;
    FILE *level;
    FILE *dir;
    int minesCount = 0;
    int edit, num;
	
	dir = fopen(LVL_DIR, "r");
    fscanf(dir, " %d", &num);
    fclose(dir);
    
	// check if max number of levels is reached
    if(num >= 20){
		printf("\nMax number of levels reached. (20 levels)\n");
		return;
	}

	renderMenuLevelAsk(1, name, cLevels);
	strcpy(filename, name);
    strcat(filename, ".txt");
    strcat(path, filename);

    if (fileExists(path) == 1) {
        printf("\nLevel cannot be created: File already exists.\n");
    } else if(strlen(name) > 20){
    	printf("\nLevel cannot be created: Filename is over 20 characters.\n");
	}else{
        CLEARSCREEN;
		printf("\n  Level %s will be created.\n\n", filename);
		delay(1000);

		int validNum = 0;
		while(!validNum){
				CLEARSCREEN;
			    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  \n");
			    printf("|                                    |  \n");
			    printf("|           LEVEL CREATION           |  \n");
			    printf("|  --------------------------------  |  \n");
			    printf("|                                    |  \n");
			    printf("|                                    |  \n");
			    printf("|   ENTER NUMBER OF ROWS (5 to 10)   |  \n");
			    printf("|       AND COLUMNS (5 to 15):       |  \n");
			    printf("|                                    |  \n");
			    printf("|                                    |  \n");
			    printf("|                                    |  \n");
			    printf("|                                    |  \n");
			    printf("|                                    |  \n");
			    printf("|                                    |  \n");
			    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  \n");
			    iMoveCursor(15,10);
        		iShowCursor();
				scanf("%d %d", &customLevel->rows, &customLevel->cols);
				iHideCursor();

				if((customLevel->rows < 5 || customLevel->rows > 10) && (customLevel->cols < 5 || customLevel->cols > 15)){
					iMoveCursor(15,15);
        			printf("\nInvalid number of rows and columns.\n\n");
        			delay(1000);
				} else if (customLevel->rows < 5 || customLevel->rows > 10) {
					iMoveCursor(15,15);
            		printf("\nInvalid number of rows.\n\n");
            		delay(1000);
        		} else if(customLevel->cols < 5 || customLevel->cols > 15){
        			iMoveCursor(15,15);
        			printf("\nInvalid number of columns.\n\n");
        			delay(1000);
				} else {
					validNum = 1;
				}
		}

		for (int i = 0; i < customLevel->rows; i++) {
        	for (int j = 0; j < customLevel->cols; j++) {
            	customLevel->board[i][j] = '.';
        	}
    	}

        edit = editLevel(customLevel, minesCount);

        if(edit == 1){
    		level = fopen(path, "w");
    		saveFile(0, level, customLevel, cLevels, name);
    		fclose(level);
    		
    		CLEARSCREEN;
    		printBoardChar(*customLevel);
        	printf("\nLevel created successfully.\n\n");
		} else{
			CLEARSCREEN;
			printBoardChar(*customLevel);
			printf("\nLevel was not saved.\n\n");
		}
    }
}

/*	
	This function directs to functions for custom levels, depending on the user's selection.
	Specifically, (1) creating a new level, (2) editing a previously created level, and (3) deleting a level. 
	@param *customLevel - a structure pointer that pertains to the information of a custom level
	@param *cLevels - a 1D structure array of that holds the information file name and path of each level

	@return void
	
	Pre-condition: none
*/

void levelEditor(game *customLevel, customLevelList *cLevels) {
    int quit = 0, choice = 0;

	CLEARSCREEN;
	while(!quit){
		choice = menuLevelEditor();

        switch (choice) {
            case 0:
				CLEARSCREEN;
				createLevel(customLevel, cLevels);
                break;
            case 1:
                CLEARSCREEN;
				loadLevel(customLevel, cLevels);
                break;
            case 2:
            	CLEARSCREEN;
				deleteLevel(cLevels);
                break;
            case 3:
				printf("\nYou have opted to go back.\n");
				quit = 1;
				break;
			default:
				printf("Invalid selection. Please choose again.\n\n");
        }
    	if (!quit){
			printf("Press any key to continue...\n");
			getch();
		}
	}
}

/* statistics */

/*	
	This function gets the game data and recent games of the current user.
	@param *currentUser - a structure pointer that contains the information of the current user

	@return void
	
	Pre-condition: *currentUser has all members initialized
*/
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
	fclose(user);
}

/*	
	This function allows the current user to view their statistics.
	@param *currentUser - a structure pointer that contains the information of the current user

	@return void
	
	Pre-condition: *currentUser has all members initialized
*/
void viewStatistics(profile *currentUser){
    int i, j, k, b, c, l, cont = 1, selection = 0;
    int time, hours, minutes, seconds;
    FILE *recentgames;
	getStatistics(currentUser);
    
	while(cont){
		CLEARSCREEN;
		printf("\n");
    	iSetColor(I_COLOR_CYAN);
    	printf("PLAYER %-20s\n", currentUser->name);
   		printf("*******************************\n\n");
    	iSetColor(I_COLOR_WHITE);

		printf("  xxxxxxxxxxxxxxxxxxxxxxxxxxxx   \n");
		printf("  |   CLASSIC   |   CUSTOM   |   \n");
		printf("  xxxxxxxxxxxxxxxxxxxxxxxxxxxx   \n");
		printf("  |             |            |   \n");
		printf("  |   WON: %d    |  WON: %d    |   \n", currentUser->games_won_classic, currentUser->games_won_custom);
		printf("  |  LOST: %d    | LOST: %d    |   \n", currentUser->games_lost_classic, currentUser->games_lost_custom);
		printf("  |             |            |   \n");
		printf("  xxxxxxxxxxxxxxxxxxxxxxxxxxxx   \n\n");
		

		recentgames = fopen(currentUser->recentgame[0].path, "r");

        fscanf(recentgames, "%s", currentUser->recentgame[0].outcome);

        fclose(recentgames);

		if (strcmp(currentUser->recentgame[0].outcome, "0") == 0) {
			printf("You currently have no recent games.\n");
			printf("Press any key to return to menu...\n");
			getch();
			return;
		}
		else {
			for(i = 0; i < 2; i++){
				if (i == selection){
					if (i == 0) printf(" > VIEW recent games\n");
					if (i == 1) printf(" > RETURN to main menu\n");
				} else {
					if (i == 0) printf("   VIEW recent games\n");
					if (i == 1) printf("   RETURN to main menu\n");
				}
			}
			selection = controlsMenu(&cont, selection, 2);
		}
	}
    
	if (selection == 1){
		return;
	} else{

    for(i = 0; i < 3 && i > -1;) {
		cont = 1;
		selection = 0;
		
		recentgames = fopen(currentUser->recentgame[i].path, "r");

        fscanf(recentgames, "%s", currentUser->recentgame[i].outcome);
		fscanf(recentgames, "%d", &currentUser->recentgame[i].time);
        fscanf(recentgames, "%s", currentUser->recentgame[i].mode);
        fscanf(recentgames, "%d %d", &currentUser->recentgame[i].rows, &currentUser->recentgame[i].cols);

        for (b = 0; b < currentUser->recentgame[i].rows; b++) {
           for (c = 0; c < currentUser->recentgame[i].cols; c++) {
                fscanf(recentgames, " %c", &currentUser->recentgame[i].snapshot[b][c]);
            }
        }
        
        time = currentUser->recentgame[i].time;
		hours = time / 3600;
        minutes = (time % 3600) / 60;
        seconds = time % 60;

        fclose(recentgames);

		while (cont){

		if(strcmp(currentUser->recentgame[i].outcome, "0") == 0){
			CLEARSCREEN;
			iSetColor(I_COLOR_CYAN);
		    printf("RECENT GAMES!                    \n");
			printf("*******************************\n");	
			iSetColor(I_COLOR_WHITE);
			printf("\n#%d | ", i+1);
			printf("\nGAME %s \n", currentUser->recentgame[i].outcome);
		} else{

			CLEARSCREEN;
			iSetColor(I_COLOR_CYAN);
		    printf("RECENT GAMES!                    \n");
			printf("*******************************\n");	
			iSetColor(I_COLOR_WHITE);
			printf("\n#%d | ", i+1);
			printf("\nGAME %s ", currentUser->recentgame[i].outcome);
			printf("[%02d:%02d:%02d]\n", hours, minutes, seconds);
			printf("%s ", currentUser->recentgame[i].mode);
			printf("%dx%d\n", currentUser->recentgame[i].rows, currentUser->recentgame[i].cols);
			for (j = 0; j < currentUser->recentgame[i].rows; j++) {
				for (k = 0; k < currentUser->recentgame[i].cols; k++) {
					if(currentUser->recentgame[i].snapshot[j][k] == 'X'){
						iSetColor(I_COLOR_RED);
						printf("X ");
					} else if (currentUser->recentgame[i].snapshot[j][k] == 'F'){
						iSetColor(I_COLOR_CYAN);
						printf("F ");
					} else if (currentUser->recentgame[i].snapshot[j][k] == 'x'){
						iSetColor(I_COLOR_YELLOW);
						printf("x ");
					} else if (currentUser->recentgame[i].snapshot[j][k] == '.'){
						iSetColor(I_COLOR_WHITE);
						printf(". ");
					} else {
						iSetColor(I_COLOR_GREEN);
						printf("%c ", currentUser->recentgame[i].snapshot[j][k]);
					}
				}
			printf("\n");
			}
			printf("\n");
			iSetColor(I_COLOR_WHITE);
			}

			if (i == 1) { // second game
				for(l = 0; l < 3; l++){

				if (l == selection){
					if (l == 0) printf(" > NEXT game\n");
					if (l == 1) printf(" > PREVIOUS game\n");
					if (l == 2) printf(" > RETURN to main menu\n");
				} else {
					if (l == 0) printf("   NEXT game\n");
					if (l == 1) printf("   PREVIOUS game\n");
					if (l == 2) printf("   RETURN to main menu\n");
				}
				}
				selection = controlsMenu(&cont, selection, 3);
			}

			if (i == 0) { // first game
				for(l = 0; l < 2; l++){

				if (l == selection){
					if (l == 0) printf(" > NEXT game\n");
					if (l == 1) printf(" > RETURN to main menu\n");
				} else {
					if (l == 0) printf("   NEXT game\n");
					if (l == 1) printf("   RETURN to main menu\n");
				}
				}
				selection = controlsMenu(&cont, selection, 3);
			}

			if (i == 2) { // third game
				for(l = 0; l < 2; l++){

				if (l == selection){
					if (l == 0) printf(" > PREVIOUS game\n");
					if (l == 1) printf(" > RETURN to main menu\n");
				} else {
					if (l == 0) printf("   PREVIOUS game\n");
					if (l == 1) printf("   RETURN to main menu\n");
				}
				}
				selection = controlsMenu(&cont, selection, 3);
			}
			
		}
		if (selection == 0 && i == 1) i++;
		else if (selection == 1 && i == 1) i--;
		else if (selection == 2 && i == 1) i = 4;
		else if (selection == 0 && i == 0) i++;
		else if (selection == 1 && i == 0) i = 4;
		else if (selection == 0 && i == 2) i--;
		else if (selection == 1 && i == 2) i = 4;
    }
	}
}

/* profile */

/*	
	This function allows the current user to view their statistics.
	@param currentUser - a structure that contains the information of the current user

	@return the selection of the user
	
	Pre-condition: *currentUser has all members initialized
*/
int menuProfile(profile currentUser, int *choice){
	int selection = 0, cont = 1;
	
	while(cont){
		CLEARSCREEN;
	    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx \n");
	    printf("|                                    | \n");
	    printf("|            CHANGE PROFILE          | \n");
	    printf("|  --------------------------------  | \n");
	    printf("|                                    | \n");
	    printf("|     WHAT WOULD YOU LIKE TO DO?     | \n");
	    printf("|                                    | \n");
	    printf("|                                    | \n");
		renderMenuProfile(selection);
	    printf("|                                    | \n");
	    printf("|                                    | \n");
	    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx \n");
	    printf("Use [ARROW KEYS] and [ENTER] to select\n\n");
		selection = controlsMenu(&cont, selection, 4);
	}
	return selection;
}

/*	
	This function allows the current user to view their statistics.
	@param name[] - a string that holds the name input of the user
	@return 1 if name is capitalized, 0 otherwise
	
	Pre-condition: name is not empty
*/
int checkCapital(char name[]){
	int i;
	
	for(i = 0; i<strlen(name); i++){
		if(name[i] >= 'A' && name [i] <= 'Z'){
		} else return 0;
	}
	return 1;
}

/*	
	This function sorts the profile list alphabetically.
	@param  users - a 1D array that contains the player profiles

	@return void
	
	Pre-condition: users is not empty
*/
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

/*	
	This function checks the currently existing users/profiles;
	@param  users - a 1D array that contains the player profiles

	@return number of existing users/profiles
	
	Pre-condition: users is not empty
*/
int checkProfiles(profileList users){
	int i, numFiles;
	FILE *dir;

	sortProfiles(users);
	dir = fopen(USER_DIR, "r");
	fscanf(dir, "%d", &numFiles);

	printf("|         EXISTING PROFILES:         |\n");
	for(i = 0; i < numFiles; i++){
		fscanf(dir, "%s", users[i].name);
		printf("|      %d. %-20s       |  \n", i + 1, users[i].name);
	}

	fclose(dir);
	return numFiles;
}

/*	
	This function displays a menu for profile creation, deletion, or selection. 
	It prompts the user for a file name, and updates the cursor position for input.
	
	@param mode - an integer that indicates the purpose of the menu being displayed
	@param *filename - a string that holds the path of the file
	@param currentUser - a structure that contains the information of the current user
	@param users - a 1D array that contains the player profiles

	@return void
	
	Pre-condition: no parameters are empty
*/

void renderMenuProfileAsk(int mode, char *filename, profile currentUser, profileList users) {
    int num;
	
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  \n");
    printf("|                                    |  \n");
    if(mode == 1) printf("|         PROFILE CREATION           | \n");
	if(mode == 2) printf("|          PROFILE DELETION          | \n");
	if(mode == 3) printf("|          PROFILE SELECTION         | \n");
    printf("|  --------------------------------  |  \n");
    printf("|  CURRENT USER: %-20s|\n", currentUser.name);
	printf("|                                    |  \n");
	num = checkProfiles(users);
	printf("|                                    |  \n");
    printf("|         PROVIDE FILE NAME          |  \n");
    if(mode == 1) printf("|      (MAX 20 UPPERCASE CHAR):      |  \n");
	if(mode == 2 || mode == 3) printf("|       (DO NOT INCLUDE .TXT):       | \n");
    printf("|                                    |  \n");
    printf("|                                    |  \n");
    printf("|                                    |  \n");
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx  \n");
    iMoveCursor(11, 11+num);
    iShowCursor();
    scanf("%s", filename);
    iMoveCursor(0, 14+num);
    iHideCursor();
}

/*
    This function allows the user to select an existing profile and updates the current user.

    @param *currentUser - a pointer to a structure containing the information of the current user
    @param *users - a pointer to an array of structures containing player profiles
    
	@return 1 if profile selection is successful, else returns 0

    Pre-condition: currentUser and users are not empty
*/
int selectProfile(profile *currentUser, profileList *users){
	char name[21];
	char filename[25];
	char path[] = USER_PATH;

	renderMenuProfileAsk(3, name, *currentUser, *users);
	strcpy(filename, name);
	strcat(filename, ".txt");
	strcat(path, filename);
	
	if(!(checkCapital(name))) {
		printf("\nName is not all uppercase letters.\n");
		return 0;
	}else if(fileExists(path) == 0) {
	    printf("\nProfile does not exist. Try again.\n");
		return 0;
	} else{
	    strcpy(currentUser->name, name);
	    getStatistics(currentUser);
	    return 1;
	}
}

/*
    This function allows the user to create a new profile and updates the list of profiles.

    @param *currentUser - a pointer to a structure containing the information of the current user
    @param users - a structure containing an array of player profiles

    @return void

    Pre-condition: currentUser and users are not empty
*/
void newProfile(profile *currentUser, profileList users){
	char name[100];
	char filename[25];
    char path[] = USER_PATH;
    char gamePath[] = GAME_PATH;
    int num, i;
    FILE *user;
    FILE *dir;
	
	// read number of users
	dir = fopen(USER_DIR, "r");
    fscanf(dir, " %d", &num);
    fclose(dir);
    
	// check if max number of users is reached
    if(num > 9){
		printf("\nMax number of profiles reached. (10 profiles)\n");
		return;
	}

	// render menu and get user input
    renderMenuProfileAsk(1, name, *currentUser, users);
	strcpy(filename, name);
	strcat(filename, ".txt");
	strcat(path, filename);

	// validation
    if(!(checkCapital(name))){
		printf("\nName is not all uppercase letters.\n");
	}else if(strlen(name) > 20){
    	printf("\nName is over 20 characters.\n");
	}else if(strlen(name) < 3){
		printf("\nName is less than 3 characters.\n");
	}else if(fileExists(path) != 0) {
        printf("\nProfile already exists.\n");
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
			fprintf(recent, "0");
			fclose(recent); 
		}
    	strcpy(currentUser->name, name);
		getStatistics(currentUser);
	}
}

/*
    This function allows the user to delete an existing profile and associated game snapshots.

    @param *currentUser - a pointer to a structure containing the information of the current user
    @param users - a structure containing an array of player profiles
    @return void

    Pre-condition: currentUser and users are not empty
*/
void deleteProfile(profile *currentUser, profileList users){
    char name[21];
    char filename[25];
    char path[] = USER_PATH;
    char gamePath[] = GAME_PATH;
    int num, i;
    FILE *dir;

	renderMenuProfileAsk(2, name, *currentUser, users);
    strcpy(filename, name);
    strcat(filename, ".txt");
    strcat(path, filename);

	if(!(checkCapital(name))){
        printf("\nName is not all uppercase letters.\n");
	} else if(fileExists(path) == 0) {
        printf("\nProfile does not exist. Try again.\n");
        return;
    } else {
		// delete user file | WORKING
		strcpy(filename, name);
		strcat(filename, ".txt");
		strcpy(path, USER_PATH);
		strcat(path, filename);
		remove(path);
		
		if (fileExists(path) == 0){
			printf("\nProfile [%s] deleted successfully.\n\n", name);
		} else {
		printf("\nProfile not deleted successfully.\n\n");
        return;
    	}

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
		}

    // delete user snapshots
		for(i = 0; i < 3; i++){
			strcpy(gamePath, GAME_PATH);
			strcat(gamePath, name);
			if (i == 0) {
				strcat(gamePath, "_snapshot0.txt");
			}
			if (i == 1) {
				strcat(gamePath, "_snapshot1.txt");
			}
			if (i == 2) {
				strcat(gamePath, "_snapshot2.txt");
			}
			remove(gamePath);
		}
	}
}

/*
    This function displays the change profile menu to change profile, create a new profile, and delete any existing profiles.

    @param *currentUser - a pointer to a structure containing the information of the current user
    @param *users - a pointer to an array of structures containing player profiles
    @return void

    Pre-condition: currentUser and level has all members initialized
*/
void changeProfile(profile *currentUser, profileList *users){
	int choice = 0, quit = 0;

	CLEARSCREEN;
	while(!quit){
		choice = menuProfile(*currentUser, &choice);
		switch(choice) {
            case 0:
                CLEARSCREEN;
				selectProfile(currentUser, users); // WORKING
                break;
            case 1:
                CLEARSCREEN;
				newProfile(currentUser, *users); // WORKING
                break;
            case 2:
            	CLEARSCREEN;
				deleteProfile(currentUser, *users); // WORKING
                break;
            case 3:
				printf("\nYou have opted to go back.\n");
				quit = 1;
				break;
			default:
				printf("Invalid selection. Please choose again.\n\n");
		}
		
		if(!quit){
			printf("Press any key to continue...\n");
			getch();
		}
	}
}

/* play */

/*
    This function allows the player to play a custom level. 

    @param *customLevel - a pointer to a structure containing custom level details
    @param *currentUser - a pointer to a structure containing the information of the current user
    @param *cLevels - a pointer to a structure containing a list of custom levels
    @return void

	Pre-condition: currentUser, customLevel, and level has all members initialized
*/
void playCustom(game *customLevel, profile *currentUser, customLevelList *cLevels){
	char filename[21];
    char path[100] = LVL_PATH;
    FILE *chosenLevel;
    customLevel->mines = 0;

	renderMenuLevelAsk(0, filename, cLevels);
    strcat(filename, ".txt");
    strcat(path, filename);

    if(fileExists(path) == 0) {
        printf("\nLevel does not exist. Try again.\n");
        printf("Press any key to continue...\n");
		getch();
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

/*
    This function allows the player to play a classic level.

    @param *level - a pointer to a structure containing classic level details
    @param *currentUser - a pointer to a structure containing the information of the current user
    @return void

    Pre-condition: currentUser and level has all members declared
*/
void playClassic(game *level, profile *currentUser){
	int classicSelect = 0;
	int exit = 0;
	int cont = 1;
	
	while (!exit){
		while(cont){
			CLEARSCREEN;
		    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    \n");
		    printf("|                                    |    \n");
		    printf("|           START THE GAME           |    \n");
		    printf("|  --------------------------------  |    \n");
		    printf("|                                    |    \n");
		    printf("|     [CLASSIC] PICK DIFFICULTY      |    \n");
		    printf("|                                    |    \n");
		    printf("|                                    |    \n");
			renderMenuPlayClassic(classicSelect);
		    printf("|                                    |    \n");
		    printf("|                                    |    \n");
		    printf("|                                    |    \n");
		    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    \n");
		    printf("Use [ARROW KEYS] and [ENTER] to select\n\n");
			classicSelect = controlsMenu(&cont, classicSelect, 3);
		}
		
		switch (classicSelect)
		{
			case 0: 
				level->cols = 8;
				level->rows = 8;
				level->mines = 10;
				strcpy(level->mode, CLASSIC_EASY);
				makeBoard(level);
				gameProper(*level, currentUser);
				exit = 1;
				break;
			case 1:
				level->cols = 15;
				level->rows = 10;
				level->mines = 35;
				strcpy(level->mode, CLASSIC_DIFFICULT);
				makeBoard(level);
				gameProper(*level, currentUser);
				exit = 1;
				break;
			case 2:
				printf("\nYou have opted to go back.\n");
				exit = 1;
				break;
			default:
				printf("\nInvalid input. Try again.\n");
			}
		}
	}

/*
    This function displays a menu for the player to choose between the classic or custom gamemode.

    @param *currentUser - a pointer to a structure containing the information of the current user
    @param *level - a pointer to a structure containing classic level details
    @param *customLevel - a pointer to a structure containing custom level details
    @param *cLevels - a pointer to a structure containing a list of custom levels
    @return void

    Pre-condition: currentUser, level, customLevel, and cLevels has all members declared
*/
void play(profile currentUser, game level, game customLevel, customLevelList *cLevels)
{
	int gameSelect = 0;
	int cont = 1;
	int exit = 0;

	while (!exit){
		while (cont) {
			CLEARSCREEN;
		    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    \n");
		    printf("|                                    |    \n");
		    printf("|           START THE GAME           |    \n");
		    printf("|  --------------------------------  |    \n");
		    printf("|                                    |    \n");
		    printf("|       CHOOSE YOUR GAME MODE!       |    \n");
		    printf("|                                    |    \n");
		    printf("|                                    |    \n");
			renderMenuPlay(gameSelect);
		    printf("|                                    |    \n");
		    printf("|                                    |    \n");
		    printf("|                                    |    \n");
		    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx    \n");
			printf("Use [ARROW KEYS] and [ENTER] to select\n\n");
			gameSelect = controlsMenu(&cont, gameSelect, 3);
		}
			
			switch (gameSelect)
			{
				case 0:
					CLEARSCREEN;
					playClassic(&level, &currentUser);
					exit = 1;
					break;
				case 1:
					CLEARSCREEN;
					playCustom(&customLevel, &currentUser, cLevels);
					exit = 1;
					break;
				case 2:
					printf("\nYou have opted to go back.\n\n");	
					exit = 1;
					break;
				default:
					printf("\nInvalid input. Try again.\n");
			}
	}
	
}

/* leaderboard */

/*
    This function sorts leaderboard rankings by best time

    @param ranking - a structure that contains the game information of leaderboard entry
    @param count - an integer indicating the valid entries of players
    
	@return void

    Pre-condition: ranking has all members initialized and count is a non-negative integer
*/

void sortLeaderboard(leaderboard ranking, int count) {
    int i, j, low;
    int temp_time;
    char temp_user[21];

    for (i = 0; i < count - 1; i++) {
        low = i;
        for (j = i + 1; j < count; j++) {
            if (ranking[low].time > ranking[j].time) {
                low = j;
            }
        }

        if (i != low) {
            // time swap
            temp_time = ranking[i].time;
            ranking[i].time = ranking[low].time;
            ranking[low].time = temp_time;

            // user swap
            strcpy(temp_user, ranking[i].user);
            strcpy(ranking[i].user, ranking[low].user);
            strcpy(ranking[low].user, temp_user);
        }
    }
}

/*
    This function sorts displays the leaderboard for both easy and difficult levels, showing the top 3 players.

    @param easyRanking - a structure that contains the top 3 leaderboard for classic easy
    @param difficultRanking -  a structure that contains the top 3 leaderboard for classic difficult
    
	@return void

    Pre-condition: easyRanking and difficultRanking has all members decalred 
*/

void printLeaderboard(leaderboard easyRanking, leaderboard difficultRanking) {
    int easyHours[3], easyMinutes[3], easySeconds[3];
    int diffHours[3], diffMinutes[3], diffSeconds[3];
    int easyTime[3];
    int difficultTime[3];
    int i;

    for (i = 0; i < 3; i++) {
        easyTime[i] = easyRanking[i].time;
        difficultTime[i] = difficultRanking[i].time;
    }

    for (i = 0; i < 3; i++) {
        easyHours[i] = easyTime[i] / 3600;
        easyMinutes[i] = (easyTime[i] % 3600) / 60;
        easySeconds[i] = easyTime[i] % 60;
        diffHours[i] = difficultTime[i] / 3600;
        diffMinutes[i] = (difficultTime[i] % 3600) / 60;
        diffSeconds[i] = difficultTime[i] % 60;
    }
    
    printf("|   EASY**********************LEADERBOARD   |\n");
    printf("|   [#1 %-20s -  %02d:%02d:%02d]   |\n", easyRanking[0].user, easyHours[0], easyMinutes[0], easySeconds[0]);
    printf("|   [#2 %-20s -  %02d:%02d:%02d]   |\n", easyRanking[1].user, easyHours[1], easyMinutes[1], easySeconds[1]);
    printf("|   [#3 %-20s -  %02d:%02d:%02d]   |\n", easyRanking[2].user, easyHours[2], easyMinutes[2], easySeconds[2]);
    printf("|   DIFFICULT*****************LEADERBOARD   |\n");
    printf("|   [#1 %-20s -  %02d:%02d:%02d]   |\n", difficultRanking[0].user, diffHours[0], diffMinutes[0], diffSeconds[0]);
    printf("|   [#2 %-20s -  %02d:%02d:%02d]   |\n", difficultRanking[1].user, diffHours[1], diffMinutes[1], diffSeconds[1]);
    printf("|   [#3 %-20s -  %02d:%02d:%02d]   |\n", difficultRanking[2].user, diffHours[2], diffMinutes[2], diffSeconds[2]);

}

/*
    This function creates the leardboard by reading through all existing players recent game snapshots then updating temporary leaderboards based on game won time. It then sorts and outputs top 3 entries to the final leaderboard to be printed for both easy and difficult.

    @param easyRanking - a structure that contains the top 3 leaderboard for classic easy
    @param difficultRanking -  a structure that contains the top 3 leaderboard for classic difficult
	@param users - a 1D array that contains the player profiles

	@return void

    Pre-condition: easyRanking, difficultRanking, and users has all members decalred 
*/

void makeLeaderboard(leaderboard easyRanking, leaderboard difficultRanking, profileList users) {
    FILE *user, *dir, *recentgames;
    int i, j, b, c, numFiles;
    int easyCount = 0, difficultCount = 0; // valid entries
    char path[25];
    struct scoreboard tempEasy[MAX_PROFILES * 3];
    struct scoreboard tempDifficult[MAX_PROFILES * 3];

    // initialize clean leaderboard
    for (i = 0; i < MAX_PROFILES * 3; i++) {
        if (i < 3) {
            easyRanking[i].time = 0;
            difficultRanking[i].time = 0;
            easyRanking[i].user[0] = '\0';
            difficultRanking[i].user[0] = '\0';
        }
        tempEasy[i].time = 0;
        tempDifficult[i].time = 0;
        tempEasy[i].user[0] = '\0';
        tempDifficult[i].user[0] = '\0';
    }

    dir = fopen(USER_DIR, "r");
    if (dir == NULL) {
        printf("Error opening profile directory.\n");
        return;
    }
    
	fscanf(dir, "%d", &numFiles);

    for (i = 0; i < numFiles; i++) {
        fscanf(dir, "%s", users[i].name);

        if (strcmp(users[i].name, "") == 0) {
            return;
        } else {
			// make path
            strcpy(path, USER_PATH);
            strcat(path, users[i].name);
            strcat(path, ".txt");
			// open path
            user = fopen(path, "r");
            if (user == NULL) {
                printf("Error opening user file %s.\n", path);
                return;
            }
            fscanf(user, "%s", users[i].name);
            fscanf(user, "%d\n%d\n%d\n%d", &users[i].games_won_classic, &users[i].games_lost_classic, &users[i].games_won_custom, &users[i].games_lost_custom);
            for (j = 0; j < 3; j++) {
                fscanf(user, "%s\n", users[i].recentgame[j].path);
            }
            fclose(user);
        }
    }
    fclose(dir);

    // check all scanned profiles existing recent games
    for (i = 0; i < numFiles; i++) { // go through all profiles
        for (j = 0; j < 3; j++) { // go through recent games
            recentgames = fopen(users[i].recentgame[j].path, "r");
            if (recentgames == NULL) {
                printf("Error opening recent game file %s.\n", users[i].recentgame[j].path);
                fclose(recentgames);
                return;
            }
			
            fscanf(recentgames, "%s", users[i].recentgame[j].outcome);
            
            if(strcmp(users[i].recentgame[j].outcome, "0") == 0){
            	fclose(recentgames);
			} else {
				//read snapshot data
	            fscanf(recentgames, "%d", &users[i].recentgame[j].time);
	            fscanf(recentgames, "%s", users[i].recentgame[j].mode);
	            fscanf(recentgames, "%d %d", &users[i].recentgame[j].rows, &users[i].recentgame[j].cols);
	
	            for (b = 0; b < users[i].recentgame[j].rows; b++) {
	                for (c = 0; c < users[i].recentgame[j].cols; c++) {
	                    fscanf(recentgames, " %c", &users[i].recentgame[j].snapshot[b][c]);
	                }
	            }
	            fclose(recentgames);
        	}

            // write temp leaderboards
            if (strcmp(users[i].recentgame[j].outcome, "WON") == 0) {
                if (strcmp(users[i].recentgame[j].mode, CLASSIC_EASY) == 0) {
                    strcpy(tempEasy[easyCount].user, users[i].name);
                    tempEasy[easyCount].time = users[i].recentgame[j].time;
                    easyCount++;
                } else if (strcmp(users[i].recentgame[j].mode, CLASSIC_DIFFICULT) == 0) {
                    strcpy(tempDifficult[difficultCount].user, users[i].name);
                    tempDifficult[difficultCount].time = users[i].recentgame[j].time;
                    difficultCount++;
                }
            }
        }
    }

    // set the temporary leaderboards
    sortLeaderboard(tempEasy, easyCount); 
    sortLeaderboard(tempDifficult, difficultCount);

    // fill the final leaderboards with sorted and valid entries
    for (i = 0; i < 3; i++) {
        if (i < easyCount) {
            easyRanking[i] = tempEasy[i];
        } else { // if not enough entries, leave other ranks empty
            easyRanking[i].time = 0;
            easyRanking[i].user[0] = '\0';
        }

        if (i < difficultCount) {
            difficultRanking[i] = tempDifficult[i];
        } else {
            difficultRanking[i].time = 0;
            difficultRanking[i].user[0] = '\0';
        }
    }

    // print leaderboard
    printLeaderboard(easyRanking, difficultRanking);
}

/* menu */
/*
    This function displays the start menu the game, prompting the user between profile creation or selection.

    @param *currentUser - a pointer to a structure containing the information of the current user
    @param *users - a pointer to an array of structures containing player profiles
    @return void

    Pre-condition: currentUser and users are declared
*/
void startMenu(profile *currentUser, profileList *users){
	int num, i, selection = 0, cont = 1, valid = 0;
	FILE *dir;
	
	asciiStartMenu();
	getch();
	
	dir = fopen(USER_DIR, "r");
	fscanf(dir, " %d", &num);
	fclose(dir);

	if(num <= 0){
		cont = 0;
		selection = 0;
	} 
	
	while (cont){
		CLEARSCREEN;
		printf("CHOOSE PLAYER:\n\n");
		printf("Use [ARROW KEYS] and [ENTER] to select\n\n");
			
		for(i = 0; i < 2; i++){
			if (selection == i && i == 0){
				iSetColor(I_COLOR_YELLOW);
				printf(" > NEW PROFILE\n");
				iSetColor(I_COLOR_WHITE);
			} else if (selection == i && i == 1){
				iSetColor(I_COLOR_YELLOW);
				printf(" > SELECT PROFILE\n");
				iSetColor(I_COLOR_WHITE);
			} else if (i == 0){
				printf("   NEW PROFILE\n");
			} else if (i == 1){
				printf("   SELECT PROFILE\n");
			}
		}
		selection = controlsMenu(&cont, selection, 2);
	}
	
	while (!valid){
		switch(selection){
			case 0:
				CLEARSCREEN;
				newProfile(currentUser, *users);
				if (strcmp(currentUser->name, "") == 0){
					valid = 0;
				} else valid = 1;
				break;
			case 1:
				CLEARSCREEN;
				valid = selectProfile(currentUser, users);
				break;
			default:
				printf("Invalid input. Try again.\n");
		}
		printf("Press any key to continue...\n");
		getch();
	}
	getStatistics(currentUser);
}

/* GAME PROPER */
int main(){
	// initalize variables
	profile currentUser = {""};
	profileList users = {{""}};

	game level;
	game customLevel;
	customLevelList customLvls = {{""}};
	
	leaderboard easyRanking;
	leaderboard difficultRanking;

	// start
	iHideCursor();
	int menuSelect = 0, cont;
	int exit = 0;
	startMenu(&currentUser, &users);
	delay(300);
	
	while (!exit){
		cont = 1;
		while (cont){
			CLEARSCREEN;
		 	printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
		    printf("|                                           |\n");
		    printf("|                MAIN MENU                  |\n");
		    printf("|  ---------------------------------------  |\n");
		    printf("|     CURRENT USER: %-20s    |\n",currentUser.name);
		    printf("|                                           |\n");
			makeLeaderboard(easyRanking, difficultRanking, users);
			renderMainMenu(menuSelect);
			printf("|                                           |\n");
    		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	
			menuSelect = controlsMenu(&cont, menuSelect, 5);
		}
			
		switch (menuSelect){
			case 0:
				if (strcmp(currentUser.name, "") != 0){
					play(currentUser, level, customLevel, &customLvls);
				} else printf("NO CURRENT USER. Please select player profile.\n");
				break;
			case 1:
				levelEditor(&customLevel, &customLvls);
				break;
			case 2:
				changeProfile(&currentUser, &users);
				break;
			case 3:
				if (strcmp(currentUser.name, "") != 0){
					viewStatistics(&currentUser);
				} else printf("NO CURRENT USER. Please select player profile.\n");
				break;
			case 4:
				exit = 1;
				printf("\nGame exited.\n\n");
				break;
			default:
				printf("Invalid input. Try again.\n");
			}
		delay(1000);
	}
}
