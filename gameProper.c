#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct level{
	int rows;
	int cols;
	int mines;
	int gameBoard[100][100]; //what the user sees/plays with
	char board[100][100]; //is the level, does not vary
};

typedef struct level game;

void printBoard(game level){
    int i, j;
    
    printf("\n");
	printf("   ");
	for (i = 0; i < level.cols; i++) {
    	printf(" %d ", i);
    }
	
	printf("\n");
	
	for (i = 0; i < level.rows; i++) {
        for (j = -1; j < level.cols; j++) {
			if (j == -1)
				printf(" %d ", i);
			else if (level.gameBoard[i][j] == 10) //not revealed
				printf(" . ");
			else if (level.gameBoard[i][j] == 100) //flag
				printf(" F ");
			else
				printf(" %d ", level.gameBoard[i][j]);
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
		mine = 0;
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
		mine = 0;
	}
}

void placeFlag(int gameBoard[][100]){
	int i, j;
	
	printf("\nEnter row to flag: ");
	scanf(" %d", &i);
	printf("Enter column to flag: ");
	scanf(" %d", &j);
	
	if(gameBoard[i][j] == 10){
		gameBoard[i][j] = 100;
	}
	else if (gameBoard[i][j] == 100)
		printf("Tile is already flagged. Try again.\n");
	else printf("Tile is already revealed. Try again.\n");
}

void removeFlag(int gameBoard[][100]){
	int i, j;
	
	printf("\nEnter row to remove flag: ");
	scanf(" %d", &i);
	printf("Enter column to remove flag: ");
	scanf(" %d", &j);
	
	if(gameBoard[i][j] == 100){
	gameBoard[i][j] = 10;
	}
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
		printBoard(level);
		printf("\n[1] INSPECT\n[2] FLAG\n[3] REMOVE FLAG\n\nSelection: ");
		scanf(" %d", &choice);
		
		switch (choice){
			case 1:
				alive = inspectBoard(&level);
				break;
			case 2:
				placeFlag(level.gameBoard);
				break;
			case 3:
				removeFlag(level.gameBoard);
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


int main()
{	
	game level;
	
	printf("rows: ");
	scanf("%d", &level.rows);
	printf("columns: ");
	scanf("%d", &level.cols);
	printf("mines: ");
	scanf("%d", &level.mines);

	printf("\n");
	gameProper(level);
	
}