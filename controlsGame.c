#include <stdio.h>
#include <conio.h>
#include<stdlib.h>
#include <time.h>

#include "interface.c"

#define HIDDEN 10
#define FLAG 100

struct level {
    int rows;
    int cols;
    int mines;
	char mode[30];
    int gameBoard[100][100];
    char board[100][100];
};

struct fileInfo{
	char filename[21];
	char path[100];
};

typedef struct level game;

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

void controlsGame(game level, int *rowChosen, int *colChosen){
	int input;
    int cont = 1;
    int i, j;
    int row = 0;
    int col = 0;
    
    iHideCursor();
    printf("Press arrow keys (use arrow keys and press Enter to quit):\n");

	while (cont) {
		system("cls");
	    
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
						} else printf("Border reached.\n");
	                    break;
	                case 80:
	                    if (row < level.rows-1){
							row++;
						} else printf("Border reached.\n");
	                    break;
	                case 75:
	                    if (col > 0){
	                    	col--;
						} else printf("Border reached.\n");
	                    break;
	                case 77:
	                    if (col < level.cols-1){
	                    	col++;
						} else printf("Border reached.\n");
	                    break;
	                default:
	                    printf("Unknown key pressed. Please press arrow keys.\n");
	                    break;
	            }
	        }
	        else if (input == '\r') // Enter key
	            cont = 0;
	        else
	            printf("Invalid input. Use arrow keys.\n");
    }

	*rowChosen = row;
	*colChosen = col;
    iShowCursor();
}

int main() {
    int input;
    int cont = 1;
    int row = 0;
    int col = 0;
    game level;
    level.rows = 10;
    level.cols = 15;
    int i, j;
    
    makeBoard(&level);
    
    printf("Press arrow keys (use arrow keys and press Enter to quit):\n");

	while (cont) {
	system("cls");
    
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
					} else printf("Border reached.\n");
                    break;
                case 80:
                    if (row < level.rows-1){
						row++;
					} else printf("Border reached.\n");
                    break;
                case 75:
                    if (col > 0){
                    	col--;
					} else printf("Border reached.\n");
                    break;
                case 77:
                    if (col < level.cols-1){
                    	col++;
					} else printf("Border reached.\n");
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

