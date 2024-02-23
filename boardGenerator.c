#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void makeBoard(int board[][100], int rows, int cols, int mines){
    srand(time(0));
	int minesCount;
	
   	for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = 0;
        }
    }
    
    for(minesCount = 0; minesCount < mines; minesCount++) {
        int row = rand() % rows;
        int col = rand() % cols;
        if (board[row][col] == 10) {
            minesCount--;
        } else {
            board[row][col] = 10;
        }
    }
};

void boardInitialize(int board[][100], int rows, int cols, int mines)
{
	int i, j, count;
	
	makeBoard(board, rows, cols, mines);
	
	for(i=0;i<rows;i++)
	{
		for(j=0;j<cols;j++)
		{
			count = 0;
			
			if(board[i][j] != 10)
			{
			if(board[i-1][j] == 10)
			count++;
			if(board[i+1][j] == 10)
			count++;
			if(board[i][j-1] == 10)
			count++;
			if(board[i][j+1] == 10)
			count++;
			if(board[i-1][j-1] == 10)
			count++;
			if(board[i-1][j+1] == 10)
			count++;
			if(board[i+1][j-1] == 10)
			count++;
			if(board[i+1][j+1] == 10)
			count++;
			
			board[i][j] = count;
		}
		}
	}
}

void printBoard(int board[][100], int rows, int cols){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            
			if(board[i][j] == 10)
			printf(" X ");
			else printf(" %d ", board[i][j]);
        }
        printf("\n");
    }
};

int main()
{	
	int rows, cols, mines;
	int board[100][100];
	
	printf("rows: ");
	scanf("%d", &rows);
	printf("colums: ");
	scanf("%d", &cols);
	printf("mines: ");
	
	scanf("%d", &mines);
	
	
	boardInitialize( board,  rows, cols, mines);
	printf("\n");
	printBoard(board, rows, cols);
}