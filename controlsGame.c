#include <stdio.h>
#include <conio.h>
#include<stdlib.h>

int main() {
    int input;
    int cont = 1;
    int row = 0;
    int col = 0;
    int maxRows = 5;
    int maxCols = 5;
    int board[5][5] = {{0}};
    
    printf("Press arrow keys (use arrow keys and press Enter to quit):\n");

	while (cont) {
	system("cls");
	
	for (int i=0;i<maxRows;i++){
		for (int j = 0; j<maxCols; j++){
			if(i == row && j == col){
			printf(">%d<", board[i][j]);
		} else printf(" %d ", board[i][j]);
		}
		printf("\n");
	}
	
        input = getch(); 
        if (input == 224) { // arrow key entered
            input = getch(); 
            switch(input) {
                case 72:
                    if (row > 0){
						row--;
					} else printf("Border reached.\n");
					//printf("Up arrow key pressed.\n");
                    break;
                case 80:
                    if (row < maxRows){
						row++;
					} else printf("Border reached.\n");
					//printf("Down arrow key pressed.\n");
                    break;
                case 75:
                    if (col > 0){
                    	col--;
					} else printf("Border reached.\n");
					//printf("Left arrow key pressed.\n");
                    break;
                case 77:
                    if (col < maxCols){
                    	col++;
					} else printf("Border reached.\n");
					//printf("Right arrow key pressed.\n");
                    break;
                default:
                    printf("Unknown arrow key pressed.\n");
                    break;
            }
        }
        else if (input == '\r') // Enter key
            cont = 0;
        else
            printf("Invalid input. Use arrow keys.\n");
    }

	printf("Choice: (%d, %d)", row, col);
    return 0;
}

