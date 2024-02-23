#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
};

void printBoard(char board[][100], int rows, int cols){
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
};

int main(){
	int rows, cols, mines;
	printf("rows: ");
	scanf("%d", &rows);
	printf("colums: ");
	scanf("%d", &cols);
	printf("mines: ");
	scanf("%d", &mines);
	
	char board[100][100];
	makeBoard(board, rows, cols, mines);
	printBoard(board, rows, cols);
	
	return 0;
}