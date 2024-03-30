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

void transferSnapshot(char destFile[], char sourceFile[]){
	FILE *fsource;
	FILE *fdest;
	char mode[21];
	char outcome[51];
	int rows, cols, i ,j;
	char val[100][100];
	
	fsource = fopen(sourceFile, "r");
	fdest = fopen(destFile, "w");
	
	fscanf(fsource, "%s", outcome);
	fscanf(fsource, "%s", mode);
	fscanf(fsource, "%d", &rows);
	fscanf(fsource, "%d", &cols);

	fprintf(fdest, "%s\n", outcome);
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
	
	printf("copy success");
	
	fclose(fsource);
	fclose(fdest);
}

int saveSnapshot(game level, char outcome[], profile *currentUser){
	int i, j;
	char SS0[] = GAME_PATH; // LATEST
	char SS1[] = GAME_PATH;
	char SS2[] = GAME_PATH;
    FILE *fgame;
    
    strcat(SS0, currentUser->name);
	strcat(SS0, "0");
    strcat(SS0, ".txt");
	strcat(SS1, currentUser->name);
	strcat(SS1, "1");
    strcat(SS1, ".txt");
	strcat(SS2, currentUser->name);
	strcat(SS2, "2");
    strcat(SS2, ".txt");

	transferSnapshot(SS2, SS1); // overwrites 2 with 1
	transferSnapshot(SS1, SS0); // overwrites 1 with 0
    
	// 0 will now be overwritten with latest game
    fgame = fopen(SS0, "w");
    
    if (strcmp(outcome, "win") == 0){
		fprintf(fgame, "WIN\n");
		fprintf(fgame, "%s %d %d", level.mode, level.rows, level.cols);
		
		for (i = 0; i < level.rows; i++){
    		for (j = 0; j < level.cols; j++){
    			
    			if (level.gameBoard[i][j] != FLAG && level.board[i][j] != 'X'){ // tile != flag && tile != bomb
					fprintf(fgame, " %d ", level.gameBoard[i][j]);
				}
				else if (level.board[i][j] == 'X' || level.gameBoard[i][j] == FLAG){ // tile == bomb || tile == flag (game only wins if all tiles are shown, so a flag == bomb)
					fprintf(fgame, " X ");
				}
		}
		fprintf(fgame, "\n");
	}
}
	
	else if (strcmp(outcome, "lose") == 0){
		fprintf(fgame, "\nLOSE\n");
		fprintf(fgame, "%s %d %d\n", level.mode, level.rows, level.cols);
		
		for (i = 0; i < level.rows; i++){
    		for (j = 0; j < level.cols; j++){
    			if (level.gameBoard[i][j] != HIDDEN && level.gameBoard[i][j] != FLAG && level.gameBoard[i][j] != 999){ // tile != hidden && tile != flag && tile != bombExploded
					fprintf(fgame, " %d ", level.gameBoard[i][j]);
				}
				else if(level.gameBoard[i][j] == 999){ // bomb == exploded
					fprintf(fgame, " X ");
				}
				else if (level.board[i][j] == 'X'){ // tile == bomb
					fprintf(fgame, " x ");
				}
				else if (level.gameBoard[i][j] == FLAG){ // tile == flag
    				fprintf(fgame, " F ");
				}
				else if (level.gameBoard[i][j] == HIDDEN){ // tile == not revealed
					fprintf(fgame, " . ");
    			}
			}
		fprintf(fgame, "\n");
		}
}
	
	else if (strcmp(outcome, "quit") == 0){
		fprintf(fgame, "QUIT\n");
		fprintf(fgame, "%s %d %d\n", level.mode, level.rows, level.cols);

		
		for(i = 0; i < level.rows; i++){
			for(j = 0; j < level.cols; j++){
				
				if(level.gameBoard[i][j] == HIDDEN){
					fprintf(fgame, " . ");
				}
				else if (level.gameBoard[i][j] == FLAG){
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

int main()
{

	char SS0[101] = "snapshot0.txt";
	char SS1[101] = "snapshot1.txt";
	char SS2[101] = "snapshot2.txt";
	char fnew[101] = "new.txt";
	
	transferSnapshot(SS2, SS1);
	transferSnapshot(SS1, SS0);
	transferSnapshot(SS0, fnew);
//	transferSnapshot(temp, SS0);
//	transferSnapshot(SS0, SS1);
//	transferSnapshot(SS1, temp);
	
	return 0;
}
