#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void printBoard(int board[][100], int rows, int cols){
    int i, j;
    
	printf("   ");
	for (i = 0; i < cols; i++) {
    	printf(" %d ", i);
    }
	
	printf("\n");
	
	for (i = 0; i < rows; i++) {
        for (j = -1; j < cols; j++) {
			if (j == -1)
				printf(" %d ", i);
			else if (board[i][j] == 10)
				printf(" . ");
			else
				printf(" %d ", board[i][j]);
        }
        printf("\n");
    }
}

//for testing of made board/text file
void printBoardChar(char board[][100], int rows, int cols){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
			printf(" %c ", board[i][j]);
        }
        printf("\n");
    }
}

void makeBoard(char board[][100], int rows, int cols, int mines){
    srand(time(0));
	int minesCount;
	
   	for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = '.';
        }
    }
    
    for(minesCount = 0; minesCount < mines; minesCount++) {
        int row = rand() % rows;
        int col = rand() % cols;
        if (board[row][col] == 'X') {
            minesCount--;
        } else {
            board[row][col] = 'X';
        }
    }
    printBoardChar( board,  rows,  cols);
    printf("\n");
}

int mineCount(char board[][100], int rowIndex, int colIndex){
	int count = 0;
	int i = rowIndex; //tinatamad ako palitan yung is and js to the index kineme
	int j = colIndex;
			
	if(board[i][j] != 'X'){
		if(board[i-1][j] == 'X')
		count++;
		if(board[i+1][j] == 'X')
		count++;
		if(board[i][j-1] == 'X')
		count++;
		if(board[i][j+1] == 'X')
		count++;
		if(board[i-1][j-1] == 'X')
		count++;
		if(board[i-1][j+1] == 'X')
		count++;
		if(board[i+1][j-1] == 'X')
		count++;
		if(board[i+1][j+1] == 'X')
		count++;
			
		return count;
		}
	else return -1;
}

void forwardCascade(char board[][100], int gameBoard[][100], int i, int j, int rows, int cols){
	int k, l, mine = 0;
	
	for(k = i; k < rows; k++){ //forward cascading
		for(l = j; !mine && l < cols; l++){
			if(mineCount(board, k, l) != -1){
				gameBoard[k][l] = mineCount(board, k, l);
			}
			else mine = 1;
		}	
		mine = 0;
	}
}

void backwardCascade(char board[][100], int gameBoard[][100], int i, int j, int rows, int cols){
	int k, l, mine = 0;
	
	for(k = i; k >= 0; k--){ //backward cascading
		for(l = j; !mine && l >= 0; l--){
			if(mineCount(board, k, l) != -1){
				gameBoard[k][l] = mineCount(board, k, l);
			}
			else mine = 1;
		}
		mine = 0;
	}
}

void gameProper(char board[][100], int gameBoard[][100], int rows, int cols, int mines){
	int i, j, alive = 1;

	for(i = 0; i < rows; i++){
		for (j = 0; j < cols; j++){
            gameBoard[i][j] = 10;
        }
    }
    
    makeBoard(board, rows, cols, mines);
	
	while(alive){
		printBoard(gameBoard, rows, cols);
		printf("\nEnter row to inspect: ");
		scanf(" %d", &i);
		printf("Enter column to inspect: ");
		scanf(" %d", &j);
		
		if (i <= rows && j <= cols){
			if(mineCount(board, i, j) == -1){
				printf("\nYou have hit a mine. Game over.");
				alive = 0;
			}
			else {
			gameBoard[i][j] = mineCount(board, i, j); //reveal chosen tile
			forwardCascade(board, gameBoard, i, j, rows, cols);
			backwardCascade(board, gameBoard, i, j, rows, cols);
			}
		}
		else printf("Invalid input. Try again.\n");
	}
}



int main()
{	
	int rows, cols, mines;
	char board[100][100];
	int gameBoard[100][100];
	
	printf("rows: ");
	scanf("%d", &rows);
	printf("colums: ");
	scanf("%d", &cols);
	printf("mines: ");
	scanf("%d", &mines);

	printf("\n");
	gameProper(board, gameBoard, rows, cols, mines);
	
}