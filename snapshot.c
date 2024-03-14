#include<stdio.h>
#include<string.h>

struct level{
	int rows;
	int cols;
	int mines;
	int gameBoard[100][100];
	char board[100][100];
};

typedef struct level game;

struct recent_games{
		char mode;
		char outcome[101];
		char snapshot[100][100];
};

struct player{
	char name[20];
	int games_won_classic;
	int games_lost_classic;
	int games_won_custom;
	int games_lost_custom;
	struct recent_games recentgame[3];
};


int saveSnapshot(game level, char outcome[]){
	int i, j;
	char filename[20];
    char path[] = "recent games/";
    FILE *fgame;
    
    strcpy(filename, "recentgame");
	strcat(filename, ".txt");
    strcat(path, filename);
    
    fgame = fopen(path, "w");
    
    if (strcmp(outcome, "win") == 0){
		fprintf(fgame, "GAME WON\n");
		
		for (i = 0; i < level.rows; i++){
    		for (j = 0; j < level.cols; j++){
    			
    			if (level.gameBoard[i][j] != 100 && level.board[i][j] != 'X'){ // tile != flag && tile != bomb
					fprintf(fgame, " %d ", level.gameBoard[i][j]);
				}
				else if (level.board[i][j] == 'X' || level.gameBoard[i][j] == 100){ // tile == bomb || tile == flag (game only wins if all tiles are shown, so a flag == bomb)
					fprintf(fgame, " X ");
				}
		}
		fprintf(fgame, "\n");
	}
}
	
	else if (strcmp(outcome, "lose") == 0){
		fprintf(fgame, "GAME LOST\n");
		
		for (i = 0; i < level.rows; i++){
    		for (j = 0; j < level.cols; j++){
    			if (level.gameBoard[i][j] != 10 && level.gameBoard[i][j] != 100 && level.gameBoard[i][j] != 999){ // tile != hidden && tile != flag && tile != bombExploded
					fprintf(fgame, " %d ", level.gameBoard[i][j]);
				}
				else if(level.gameBoard[i][j] == 999){ // bomb == exploded
					fprintf(fgame, " X ");
				}
				else if (level.board[i][j] == 'X'){ // tile == bomb
					fprintf(fgame, " x ");
				}
				else if (level.gameBoard[i][j] == 100){ // tile == flag
    				fprintf(fgame, " F ");
				}
				else if (level.gameBoard[i][j] == 10){ // tile == not revealed
					fprintf(fgame, " . ");
    			}
			}
		fprintf(fgame, "\n");
		}
}
	
	else if (strcmp(outcome, "quit") == 0){
		fprintf(fgame, "GAME QUIT\n");
		
		for(i = 0; i < level.rows; i++){
			for(j = 0; j < level.cols; j++){
				
				if(level.gameBoard[i][j] == 10){
					fprintf(fgame, " . ");
				}
				else if (level.gameBoard[i][j] == 100){
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

