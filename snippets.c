    fprintf(stdout, "%d %d\n", customLevel->rows, customLevel->cols);
    	for (int i = 0; i < customLevel->rows; i++) {
        	for (int j = 0; j < customLevel->cols; j++) {
            fprintf(stdout, " %c ", customLevel->board[i][j]);
        	}
        printf("\n");

