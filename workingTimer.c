void delay(int time){
	clock_t delay;
	delay = clock() + time;
	while(delay > clock());
}

void gameClock(time_t timeStart){
	time_t timeEnd = time(NULL);
	int timeElapsed, hours, minutes, seconds;
	
    timeElapsed = difftime(timeEnd, timeStart); 
    
	hours = timeElapsed / 3600;
    minutes = (timeElapsed % 3600) / 60;
    seconds = timeElapsed % 60;
        
	printf("TIME: %02d:%02d:%02d\n", hours, minutes, seconds);
	fflush(stdout);
	delay(300);
}

void gameProper(game level){
	int i, j, alive = 1;
	int choice;
	char outcome[11];
	
	// time
	time_t timeStart = time(NULL);
	time_t gameTime = timeStart;
	
	
	//PLAYER BOARD
	for(i = 0; i < level.rows; i++){
		for (j = 0; j < level.cols; j++){
            level.gameBoard[i][j] = 10; // 10 == HIDDEN
        }
    }
    printBoardChar(level); // FOR TESTING
	
	while(alive){
		delay(500);
		system("cls");
		
		time_t timeCurrent = time(NULL);
		int timeUpdated = timeCurrent - gameTime;
		
		if (timeUpdated >= 1) {
            gameClock(timeStart);
            gameTime = timeCurrent;
        }
		
		printBoard(level);
		printf("\n[1] INSPECT\n[2] FLAG\n[3] REMOVE FLAG\n[0] QUIT\n\nSELECTION: ");
		scanf(" %d", &choice);
        
		switch (choice){
			case 1:
				alive = inspectBoard(&level, outcome);
				break;
			case 2:
				placeFlag(&level);
				break;
			case 3:
				removeFlag(&level);
				break;
			case 0:
				strcpy(outcome, "quit");
				printf("Game Quitted\n");
				alive = 0;
				break;
			default:
				printf("Invalid input. Please try again.\n");
		}
		
		if (alive){
		alive = gameChecker(level, outcome);
		}
	}
	gameClock(timeStart);
	saveSnapshot(level, outcome);
}
