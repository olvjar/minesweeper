void cascade(game *level, int i, int j){
    if (i < 0 || i >= level->rows || j < 0 || j >= level->cols || 
        level->board[i][j] == 'X' || level->gameBoard[i][j] == FLAG || !(level->gameBoard[i][j] == HIDDEN)) {
        return; // Base case: out of bounds or already revealed/flagged
    }
    
    level->gameBoard[i][j] = mineCount(*level, i, j); // reveal mine count
    
    if (level->gameBoard[i][j] == 0) { // if no neighboring mines
        cascade(level, i - 1, j); // up
        cascade(level, i + 1, j); // down
        cascade(level, i, j - 1); // left
        cascade(level, i, j + 1); // right
    }
}