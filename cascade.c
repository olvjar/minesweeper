//CASCADE VARIATIONS


//VARIATION 1 GRAPEVINE W SIDEPRESS
void forwardCascade(game *level, int i, int j) {
	int k, l, m, o, mine = 0;
    
    for(k=i;k<level->rows;k++){
		for(l=j;l<level->cols && !mine;l++)	{
			if (mineCount(*level, k, l) == 0) {
				for (m = -1; m <= 1; m++) { // checks all adjacent tiles
     				for (o = -1; o <= 1; o++) {
     					if (mineCount(*level, m, o) != -1){
             				level->gameBoard[m][o] = mineCount(*level, m, o);
             			}
		            }	
       			}
    		}
    		else mine = 0;
      	}
      	mine = 1;
   	}
}

void backwardCascade(game *level, int i, int j) {
	int k, l, m, o, mine = 0;
    
    for(k = i;k > 0;k--){
		for(l = j;l > 0 && !mine;l--)	{
			if (mineCount(*level, k, l) == 0) {
				for (m = -1; m <= 1; m++) { // checks all adjacent tiles
     				for (o = -1; o <= 1; o++) {
     					if (mineCount(*level, m, o) != 0 -1){
             				level->gameBoard[m][o] = mineCount(*level, m, o);
             			}
		            }	
       			}
    		}
    		else mine = 0;
      	}
      	mine = 1;
   	}
}

//VARIATION 2 SIDE TO SIDE WITH LATERAL RAISE

void forwardCascade(game *level, int i, int j){
	int k, l, mine = 0;
	
	for(k = i; k < level->rows; k++){ //forward cascading
		for(l = j; l < level->cols && !mine; l++){
			if(mineCount(*level, k, l) != -1){
				level->gameBoard[k][l] = mineCount(*level, k, l);
			}
			else mine = 1;
		}
	mine = 0
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
