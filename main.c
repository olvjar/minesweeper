/*
	Description: <describe what this program does briefly >
	Author: GRINO, Mary Eunice E. and TAMONDONG, Mariel M.
	Section: S17B
	Last Modified: March 25, 2024
	Acknowledgments: Thomas Tiam-Lee
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#include "interface.c"

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
typedef file customLevelList[100];
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
	@ param time - an integer that pertains to the delay length
	@ return void
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
	@param *cont - a pointer that determines the continuation of a loop
	@ param selection - an integer that pertains to the current selection of the arrow
	@ param max - an integer that pertains to the max number of choices in a selection

	@ return the selection of the user
	
	Pre-condition: *cont == 1, selection is a nonnegative integer, max is a nonnegative integer
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
	@ param level - a structure that pertains to the information of the level
	@ param *rowChosen - a pointer that holds the value of the row index of the array
	@ param *colChosen - a pointer that holds the value of the column index of the array

	@ return void
	
	Pre-condition: level has all values declared
*/

void controlsGame(game level, int *rowChosen, int *colChosen){
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
	        } else if (input == '\r') {
	            cont = 0;
	        }
	        else {
				printf("Invalid input. Use arrow keys.\n");
				delay(500);
		    }
    }

	CLEARSCREEN;
	*rowChosen = row;
	*colChosen = col;
}

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
	
	controlsGame(*level, &i, &j);

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
	
	controlsGame(*level, &i, &j);
	
	if(level->gameBoard[i][j] == FLAG){
		level->gameBoard[i][j] = HIDDEN;
	}
	else if (i >= level->rows || j >= level->cols)
		printf("Input is out of bounds\n");
	else printf("Tile is not flagged. Try again.\n");
}

int inspectBoard(game *level, char outcome[]) {
    int i, j;
    
    controlsGame(*level, &i, &j);
    
    if (i < level->rows && j < level->cols && level->gameBoard[i][j] != FLAG) {
        if (mineCount(*level, i, j) == -1) {
            printf("\nYou have hit a mine. Game over.\n");
            strcpy(outcome, "lose");
            level->gameBoard[i][j] = 999;
            printBoardChar(*level);
            printf("Press any key to continue...");
			getch();
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
		printf("Press any key to continue...");
		getch();
		return 0;
	}
	else return 1;
}

void transferSnapshot(char destFile[], char sourceFile[]){
	FILE *fsource;
	FILE *fdest;
	char mode[21];
	char outcome[51];
	int rows, cols, time, i ,j;
	char val[10][15];
	
	fsource = fopen(sourceFile, "r");
	fdest = fopen(destFile, "w");
	
	fscanf(fsource, " %s", outcome);
	if (outcome[0] == '0') return; 
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

int saveSnapshot(game level, char outcome[], profile currentUser, int time){
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
		return 1;
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
		return 1;
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
		return 1;
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
	int i, j, cont = 1, alive = 1;
	int choice = 0;
	char outcome[11];

	//PLAYER BOARD
	for(i = 0; i < level.rows; i++){
		for (j = 0; j < level.cols; j++){
            level.gameBoard[i][j] = HIDDEN; 
        }
    }
    
    CLEARSCREEN;
	time_t timeStart, timeEnd;
	int timeElapsed, hours, minutes, seconds;
	
	time(&timeStart);
	while(alive){
		cont = 1;
		while(cont){
			CLEARSCREEN;
			printBoardChar(level); //REMOVE THIS

			time(&timeEnd);
	        timeElapsed = difftime(timeEnd, timeStart);
	        
	        hours = timeElapsed / 3600;
	        minutes = (timeElapsed % 3600) / 60;
	        seconds = timeElapsed % 60;
			
			printf("\nTIME: %02d:%02d:%02d", hours, minutes, seconds);
			printBoard(level);
			
			for (i = 0; i < 4; i++){
				if (i == choice){
					if (i==0) printf(" > INSPECT tile\n");
					if (i==1) printf(" > FLAG tile\n");
					if (i==2) printf(" > REMOVE flag\n");
					if (i==3) printf(" > QUIT current game\n");
				} else {
					if (i==0) printf("   INSPECT tile\n");
					if (i==1) printf("   FLAG tile\n");
					if (i==2) printf("   REMOVE flag\n");
					if (i==3) printf("   QUIT current game\n");
				}
			}
			
			choice = controlsMenu(&cont, choice, 4);
			}
		
		switch (choice){
			case 0:
				alive = inspectBoard(&level, outcome);
				break;
			case 1:
				placeFlag(&level);
				break;
			case 2:
				removeFlag(&level);
				break;
			case 3:
				strcpy(outcome, "quit");
				printf("Game QUIT\n");
				alive = 0;
				break;
			default:
				printf("Invalid input. Please try again.\n");
		}
		
		printf("before game checker");
		if (alive){
		alive = gameChecker(level, outcome);
		}
	}
	printf("game ended.\n");
	saveSnapshot(level, outcome, *currentUser, timeElapsed);
	printf("Save snapshot passed\n");
	updateStatistics(level, outcome, currentUser);
	printf("update stats passed\n");
}

/* level edit */

void printCustomBoard(game *customLevel) {
    for (int i = 0; i < customLevel->rows; i++) {
        for (int j = 0; j < customLevel->cols; j++) {
            printf(" %c ", customLevel->board[i][j]);
        }
        printf("\n");
    }
}

int menuEditLevel(game *customLevel, int minesCount){
	int i, selection = 0, cont = 1;
	
	while(cont){
		CLEARSCREEN;
		printf("\n");
    	printCustomBoard(customLevel);
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

int menuLevelEditor(){
	int i, selection = 0, cont = 1;
	
	while(cont){
		CLEARSCREEN;
		printf("[LEVEL EDITOR]\n\nWhat would you like to do?\n\n");
		printf("Use [ARROW KEYS] and [ENTER] to select\n\n");
	
		for(i = 0; i < 4; i++){
			if (i == selection){
				if (i == 0) printf(" > CREATE a new level\n");
				if (i == 1) printf(" > EDIT an existing level\n");
				if (i == 2) printf(" > DELETE an existing level\n");
				if (i == 3) printf(" > GO BACK\n");
			} else {
				if (i == 0) printf("   CREATE a new level\n");
				if (i == 1) printf("   EDIT an existing level\n");
				if (i == 2) printf("   DELETE an existing level\n");
				if (i == 3) printf("   GO BACK\n");
			}
		}
		selection = controlsMenu(&cont, selection, 4);
	}
	return selection;
}


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

	controlsLevelEdit(*customLevel, &row, &col);
    printf("MINE %d\n", (*minesCount) + 1);
    if (row >= 0 && row < customLevel->rows && col >= 0 && col < customLevel->cols && customLevel->board[row][col] == '.') {
        customLevel->board[row][col] = 'X'; // Place mine
        (*minesCount)++;
        printCustomBoard(customLevel);
        printf("Mine placed.\n");
    } else {
        printCustomBoard(customLevel);
		printf("Invalid position. Mine not placed.\n\n");
    }
}

void deleteMine(game *customLevel, int *minesCount) {
    int row, col;

	controlsLevelEdit(*customLevel, &row, &col);
    printf("MINE %d\n", (*minesCount) - 1);
    if (row >= 0 && row < customLevel->rows && col >= 0 && col < customLevel->cols && customLevel->board[row][col] == 'X') {
        customLevel->board[row][col] = '.'; // Delete mine
        (*minesCount)--;
        printCustomBoard(customLevel);
		printf("Mine removed.\n");
    } else {
        printCustomBoard(customLevel);
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

void deleteLevel(customLevelList *cLevels){
	char filename[21];
	char path[100] = LVL_PATH;
	int num, i;
	FILE *dir;

	printf("[LEVEL DELETION]\n\nEXISTING CUSTOM LEVELS:\n");
	checkLevels(cLevels);

	printf("\nProvide level to delete (do not include .txt): ");
	scanf("%s", filename);
	strcat(filename, ".txt");
    strcat(path, filename);

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
		if (remove(path) != 0) {
        	perror("\nError deleting user file\n");
    	} else{
			printf("\n%s deleted successfully.\n", filename);
		}
	    remove(path);
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

	CLEARSCREEN;
    while(quit != 1){
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
            	CLEARSCREEN;
				if(checkValidity(customLevel, &minesCount) == 1) {
            		save = 1;
            		quit = 1;
                }
                break;
            case 3:
				printf("\nReturning to main menu.\n");
				quit = 1;
				break;
			default:
            	CLEARSCREEN;
				printf("Invalid selection. Please choose again.\n");
        }
        
        printf("Press any key to continue...\n");
		getch();
	}

	return save;
}

void loadLevel(game *customLevel, customLevelList *cLevels){
	char filename[20];
    char path[100] = LVL_PATH;
    FILE *level;
    int minesCount = 0;

	printf("[LEVEL EDITOR]\nEXISTING CUSTOM LEVELS:\n");
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
			fclose(level);
		}

	}
}

void createLevel(game *customLevel, customLevelList *cLevels){
	char filename[20];
    char path[100] = LVL_PATH;
    FILE *level;
    int minesCount = 0;

	printf("[LEVEL CREATION]\n\nProvide file name: ");
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
    int quit = 0, choice;

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
				printf("\nReturning to main menu.\n");
				quit = 1;
				break;
			default:
				printf("Invalid selection. Please choose again.\n\n");
        }
    	printf("Press any key to continue...\n");
		getch();
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
	fclose(user);
}

void viewStatistics(profile *currentUser){
	int choice, quit = 0;
    int i, j, k, b, c;
    int time, hours, minutes, seconds;
    FILE *recentgames;
	
	CLEARSCREEN;
	getStatistics(currentUser);
	
	printf("Press [ESC] to go back\n\n");
	printf("Name: %s\n", currentUser->name);
    printf("Classic games - Won: %d Lost: %d\n", currentUser->games_won_classic, currentUser->games_lost_classic);
    printf("Custom games - Won: %d Lost: %d\n\n", currentUser->games_won_custom, currentUser->games_lost_custom);
	printf("Recent Games:\n");
    
    for(i = 0; i < 3; i++) {
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


		if(strcmp(currentUser->recentgame[i].outcome, "0") == 0){
			printf(" ");
		} else{
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
			iSetColor(I_COLOR_WHITE);
		}
    }
	while(!quit){
		if(kbhit()){
            choice = getch();
                if(choice == 27){ // escape
                    quit = 1;
				}
		}
	}
}

/* profile */

int menuProfile(profile currentUser, int *choice){
	int i, selection = 0, cont = 1;
	
	while(cont){
		CLEARSCREEN;
		printf("[CHANGE PROFILE]\nCURRENT USER: %s\n\nWhat would you like to do?\n\n", currentUser.name);
		printf("Use [ARROW KEYS] and [ENTER] to select\n\n");
	
		for(i = 0; i < 4; i++){
			if (i == selection){
				if (i == 0) printf(" > SELECT profile\n");
				if (i == 1) printf(" > CREATE NEW profile\n");
				if (i == 2) printf(" > DELETE profile\n");
				if (i == 3) printf(" > GO BACK\n");
			} else {
				if (i == 0) printf("   SELECT profile\n");
				if (i == 1) printf("   CREATE NEW profile\n");
				if (i == 2) printf("   DELETE profile\n");
				if (i == 3) printf("   GO BACK\n");
			}
		}
		selection = controlsMenu(&cont, selection, 4);
	}
	return selection;
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
    fscanf(dir, " %d", &num);
    fclose(dir);
    
    if(num > 9){
		printf("\nMax number of profiles reached. (10 profiles)\n");
		return;
	}

    printf("\nName must be:\n[1] 3 to 20 characters\n[2] Uppercase letters only\n\nProvide profile name: ");
	scanf(" %s", name);
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
			fprintf(recent, "0");
			fclose(recent); 
		}
    	strcpy(currentUser->name, name);
		getStatistics(currentUser);
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
		// delete user file | WORKING
		strcpy(filename, name);
		strcat(filename, ".txt");
		strcpy(path, USER_PATH);
		strcat(path, filename);
		remove(path);
		
		if (fileExists(path) == 0){
			printf("\nProfile [%s] deleted successfully.\n\n", name);
		} else {
		printf("Profile not deleted successfully.\n\n");
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

void changeProfile(profile *currentUser, profileList *users){
	int choice, quit = 0;

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
				printf("\nReturning to main menu.\n");
				quit = 1;
				break;
			default:
				printf("Invalid selection. Please choose again.\n\n");
		}
		printf("Press any key to continue...\n");
		getch();
	}
}

/* leaderboard */

void sortLeaderboard(leaderboard ranking) {
    int i, j, low;
    int temp_time;
    char temp_user[21];

    for (i = 0; i < 3 - 1; i++) {
        low = i;
        for (j = i + 1; j < 3; j++) {
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

    printf("            LEADERBOARDS             \n");
    printf(" EASY*************************CLASSIC\n");
    printf(" [#1 %s -  %02d:%02d:%02d]\n", easyRanking[0].user, easyHours[0], easyMinutes[0], easySeconds[0]);
    printf(" [#2 %s -  %02d:%02d:%02d]\n", easyRanking[1].user, easyHours[1], easyMinutes[1], easySeconds[1]);
    printf(" [#3 %s -  %02d:%02d:%02d]\n", easyRanking[2].user, easyHours[2], easyMinutes[2], easySeconds[2]);
    printf("                                     \n");
    printf(" DIFFICULT********************CLASSIC\n");
    printf(" [#1 %s -  %02d:%02d:%02d]\n", difficultRanking[0].user, diffHours[0], diffMinutes[0], diffSeconds[0]);
    printf(" [#2 %s -  %02d:%02d:%02d]\n", difficultRanking[1].user, diffHours[1], diffMinutes[1], diffSeconds[1]);
    printf(" [#3 %s -  %02d:%02d:%02d]\n", difficultRanking[2].user, diffHours[2], diffMinutes[2], diffSeconds[2]);
    printf("                                     \n");
}

void makeLeaderboard(leaderboard easyRanking, leaderboard difficultRanking, profileList users) {
    FILE *user;
    FILE *dir;
    FILE *recentgames;
    int i, b, c, numFiles;
    char path[100];

    // check and scan all existing profiles
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
            strcpy(path, USER_PATH);
            strcat(path, users[i].name);
            strcat(path, ".txt");
            user = fopen(path, "r");
            if (user == NULL) {
                printf("Error opening user file %s.\n", path);
                return;
            }
            fscanf(user, "%s", users[i].name);
            fscanf(user, "%d\n%d\n%d\n%d", &users[i].games_won_classic, &users[i].games_lost_classic, &users[i].games_won_custom, &users[i].games_lost_custom);
            for (int j = 0; j < 3; j++) {
                fscanf(user, "%s\n", users[i].recentgame[j].path);
            }
            fclose(user);
        }
    }
    fclose(dir);

    // check all scanned profiles existing recent games
    for (i = 0; i < numFiles; i++) { // go through all profiles
        for (int j = 0; j < 3; j++) { // go through recent games
            recentgames = fopen(users[i].recentgame[j].path, "r");
            if (recentgames == NULL) {
                printf("Error opening recent game file %s.\n", users[i].recentgame[j].path);
                return;
            }

            fscanf(recentgames, "%s", users[i].recentgame[j].outcome);
            fscanf(recentgames, "%d", &users[i].recentgame[j].time);
            fscanf(recentgames, "%s", users[i].recentgame[j].mode);
            fscanf(recentgames, "%d %d", &users[i].recentgame[j].rows, &users[i].recentgame[j].cols);

            for (b = 0; b < users[i].recentgame[j].rows; b++) {
                for (c = 0; c < users[i].recentgame[j].cols; c++) {
                    fscanf(recentgames, " %c", &users[i].recentgame[j].snapshot[b][c]);
                }
            }
            fclose(recentgames);

            // write leaderboard
            if (strcmp(users[i].recentgame[j].outcome, "WON") == 0) {
                if (strcmp(users[i].recentgame[j].mode, CLASSIC_EASY) == 0) {
                    strcpy(easyRanking[i].user, users[i].name);
                    easyRanking[i].time = users[i].recentgame[j].time;
                } else if (strcmp(users[i].recentgame[j].mode, CLASSIC_DIFFICULT) == 0) {
                    strcpy(difficultRanking[i].user, users[i].name);
                    difficultRanking[i].time = users[i].recentgame[j].time;
                }
            }
        }
    }

    // sort leaderboards
    sortLeaderboard(easyRanking);
    sortLeaderboard(difficultRanking);
    // print leaderboard
    printLeaderboard(easyRanking, difficultRanking);
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

void playClassic(game *level, profile *currentUser){
	int classicSelect;
	int exit = 0;
	int i, cont = 1;
	
	while (!exit){
		while(cont){
			CLEARSCREEN;
			printf("Use [ARROW KEYS] and [ENTER] to select\n\n");
			printf("Choose DIFFICULTY:\n");
			
			for(i = 0; i < 3;i++){
				if (classicSelect == i){
					if (i == 0) printf(" > EASY\n");
					if (i == 1) printf(" > DIFFICULT\n");
					if (i == 2) printf(" > GO BACK\n");
				} else {
					if (i == 0) printf("   EASY\n");
					if (i == 1) printf("   DIFFICULT\n");
					if (i == 2) printf("   GO BACK\n");
				}
			}
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
		printf("Press any key to continue...\n");
		getch();
		}
	}


void play(profile currentUser, game level, game customLevel, customLevelList *cLevels)
{
	int gameSelect = 0;
	int i;
	int cont = 1;
	int max = 3;
	int exit = 0;

	while (!exit){
		while (cont) {
			CLEARSCREEN;
			printf("Use [ARROW KEYS] and [ENTER] to select\n\n");
			printf("Choose GAME TYPE:\n");
			
			for(i = 0; i < max; i++){
				if (i == gameSelect && i == 0){
					printf(" > CLASSIC\n");
				} else if (i == gameSelect && i == 1){
					printf(" > CUSTOM\n");
				} else if (i == gameSelect && i == 2){
					printf(" > GO BACK\n"); 
				}
				else if (i == 0){
					printf("   CLASSIC\n");
				} else if (i == 1){
					printf("   CUSTOM\n");
				} else if (i == 2){
					printf("   GO BACK\n");
				}
			}
			
			gameSelect = controlsMenu(&cont, gameSelect, max);
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
					printf("Press any key to continue...\n");
					getch();
					exit = 1;
					break;
				default:
					printf("\nInvalid input. Try again.\n");
			}
	}
	
}


void startMenu(profile *currentUser, profileList *users){
	int num, i, selection = 0, cont = 1, valid = 0;
	FILE *dir;
	
	printf("Welcome to Minesweeper!\n");
	delay(1000);
	
	dir = fopen(USER_DIR, "r");
	fscanf(dir, " %d", &num);
	fclose(dir);

	if(num <= 0){
		newProfile(currentUser, *users);
	} else {
		while (cont){
			CLEARSCREEN;
			printf("CHOOSE PLAYER:\n\n");
			printf("Use [ARROW KEYS] and [ENTER] to select\n\n");
			
			for(i = 0; i < 2; i++){
				if (selection == i && i == 0){
					printf(" > NEW PROFILE\n");
				} else if (selection == i && i == 1){
					printf(" > SELECT PROFILE\n");
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
	
	}
	getStatistics(currentUser);
}

/* GAME PROPER */
int main(){
	// initalize variables
	profile currentUser;
	profileList users;

	game level;
	game customLevel;
	customLevelList customLvls;
	
	leaderboard easyRanking;
	leaderboard difficultRanking;

	// start
	int menuSelect, i, cont;
	int exit = 0;
	startMenu(&currentUser, &users);
	delay(300);
	
	while (!exit){
		cont = 1;
		while (cont){
			CLEARSCREEN;
	    	printf("CURRENT USER: %s\n", currentUser.name);
	    	printf("[MAIN MENU]\n\n");
	    	
			makeLeaderboard(easyRanking, difficultRanking, users);
	
			for (i = 0; i < 5;i++){
				if (i == menuSelect){
					if (i == 0) printf(" > PLAY\n");
					if (i == 1) printf(" > LEVEL EDITOR\n");
					if (i == 2) printf(" > CHANGE PROFILE\n");
					if (i == 3) printf(" > VIEW STATISTICS\n");
					if (i == 4) printf(" > QUIT\n");
				} else {
					if (i == 0) printf("   PLAY\n");
					if (i == 1) printf("   LEVEL EDITOR\n");
					if (i == 2) printf("   CHANGE PROFILE\n");
					if (i == 3) printf("   VIEW STATISTICS\n");
					if (i == 4) printf("   QUIT\n");
				}
			}
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
		
		delay(1500);
	}
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
