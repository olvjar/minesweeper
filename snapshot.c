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


int saveSnapshot(struct player profile, game level, char outcome[], int n){
	int i, j;
	char filename[20];
    char path[] = "recent games/";
    FILE *fgame;
    
    strcpy(filename, "recentgame");
    if (n == 1) strcat(filename, "1");
    if (n == 2) strcat(filename, "2");
    if (n == 3) strcat(filename, "3");
	strcat(filename, ".txt");
    strcat(path, filename);
    
    fgame = fopen(path, "w");
    
    if (strcmp(outcome, "win") == 0){
		for (i = 0; i < level.rows; i++){
    		for (j = 0; j < level.cols; j++){
    			if (level.gameBoard[i][j] != 10){
				//	fprintf(fgame, (char) level.gameBoard[i][j]);
    			}
    			else fprintf(fgame, "X");
		}
		fprintf(fgame, "\n");
	}
}
	
	else if (strcmp(outcome, "lose") == 0){
		for (i = 0; i < level.rows; i++){
    		for (j = 0; j < level.cols; j++){
    			if (level.gameBoard[i][j] != 10 && level.gameBoard[i][j] != 100){ // if tile is revealed
				//	fprintf(fgame, (char) level.gameBoard[i][j]);
				}
				if (level.gameBoard[i][j] == 10){ // tile == not revealed
					fprintf(fgame, ".");
    			}
    			else if (level.gameBoard[i][j] == 100){ // tile == flag
    				fprintf(fgame, "F");
				}
				else if (level.board[i][j] == 'X'){ // tile == bomb
					fprintf(fgame, "x");
				}
				else fprintf(fgame, "X");
		}
		fprintf(fgame, "\n");
	}
}
	
	else if (strcmp(outcome, "pause") == 0){
	}
    
    fclose(fgame);
    
    
}

