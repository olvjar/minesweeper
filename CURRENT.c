#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "interface.c"

struct level{
	int rows;
	int cols;
	int mines;
	int gameBoard[100][100];
	char board[100][100];
};

typedef struct level game;


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
				}
			else if (level.gameBoard[i][j] == 10){ //not revealed
				iSetColor(I_COLOR_WHITE);
				printf(" . ");
			}
			else if (level.gameBoard[i][j] == 100){ //flag
				iSetColor(I_COLOR_RED);
				printf(" F ");
			}
			else{
				iSetColor(I_COLOR_GREEN);
				printf(" %d ", level.gameBoard[i][j]); //valid space & revealed
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

	for(i = 0; i < level.rows; i++){
		for (j = 0; j < level.cols; j++){
            level.gameBoard[i][j] = 10;
        }
    }
    
    
    makeBoard(&level);
    printBoardChar(level);
	
	while(alive){
		iClear(0, 0, level.cols*3, level.rows*3);
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
			gameProper(*level);
			validChoice = 1;
			break;
		case 2:
			level->cols = 10;
			level->rows = 15;
			level->mines = 35;
			gameProper(*level);
			validChoice = 1;
			break;
		default:
			printf("Invalid input. Try again.");
	}
	}
}

void playCustom(){
	printf("Choose a level to play:");
}

void play(game level){
	int gameSelect;
	int validChoice = 0;
	
	while(!validChoice){
	printf("\nGame type selection\n[1] CLASSIC or [2] CUSTOM\n\n");
	printf("Selection: ");
	scanf(" %d", &gameSelect);
	
	switch (gameSelect)
	{
		case 1: 
			playClassic(&level);
			validChoice = 1;
			break;
		case 2:
			playCustom(&level);
			validChoice = 1;
			break;
		default:
			printf("Invalid input. Try again.");
	}
	}
}

int main()
{	
	game level;

	// start
	int menuSelect;
	int start = 0;

    do {
	printf("Main Menu\n[1] PLAY\t\t[2] LEVEL EDITOR\n[3] CHANGE PROFILE\t[4] VIEW STATISTICS \n[0] QUIT\n\n");
	printf("Selection: ");
	scanf(" %d", &menuSelect);
	
	switch (menuSelect)
	{
		case 1: 
			play(level);
			start = 1;
			break;
		case 2:
			//levelEditor(&customLevel);
			start = 1;
			break;
		case 3:
			//changeProfile(&user);
			start = 1;
			break;
		case 4:
			//viewStats(&user);
			start = 1;
			break;
		default:
			printf("Invalid input. Try again.");
	}
	} while(!start);
	
}
