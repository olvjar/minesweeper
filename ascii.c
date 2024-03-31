#include <stdio.h>

/*
This function displays ASCII art when player loses the game, showing the time elapsed in hours, minutes, and seconds.
@param time - total time elapsed in seconds when the game was won
@return void
Pre-condition: time is a nonnegative integer 
*/

void asciiGameLost(int time) {
	int hours, minutes, seconds;
	
	hours = time / 3600;
	minutes = (time % 3600) / 60;
	seconds = time % 60;
	
	iSetColor(I_COLOR_RED);
	printf("\n\n    ______________________________          "); iSetColor(I_COLOR_YELLOW); printf(".      \n"); iSetColor(I_COLOR_RED);
    printf("   /                            / \\    "); iSetColor(I_COLOR_YELLOW); printf(". \\  | / .  \n"); iSetColor(I_COLOR_RED);
 	printf("  |    "); iSetColor(I_COLOR_WHITE); printf("YOU HAVE HIT A MINE"); iSetColor(I_COLOR_RED); printf("     |         "); iSetColor(I_COLOR_YELLOW); printf("\\ \\ / /   \n");
    iSetColor(I_COLOR_RED); printf("  |    -------------------     |  "); iSetColor(I_COLOR_WHITE); printf("========="); iSetColor(I_COLOR_YELLOW); printf("  - - .\n"); 
    iSetColor(I_COLOR_RED); printf("  |    "); iSetColor(I_COLOR_WHITE); printf("GAME OVER! %02d:%02d:%02d", hours, minutes, seconds); iSetColor(I_COLOR_RED); printf("     |         "); iSetColor(I_COLOR_YELLOW); printf("/ / \\ \\   \n");
    iSetColor(I_COLOR_RED);
    printf("   \\____________________________\\_/    "); iSetColor(I_COLOR_YELLOW); printf(".  / | \\ .  \n"); iSetColor(I_COLOR_RED);
    printf("                                            "); iSetColor(I_COLOR_YELLOW); printf(".      \n");
    iSetColor(I_COLOR_WHITE);
}

/*
This function displays ASCII art when player wins the game, showing the time elapsed in hours, minutes, and seconds.
@param time - total time elapsed in seconds when the game was won.
@return void
Pre-condition: time is a nonnegative integer 
*/
void asciiGameWon(int time) {
	int hours, minutes, seconds;
	
	hours = time / 3600;
	minutes = (time % 3600) / 60;
	seconds = time % 60;
	
	iSetColor(I_COLOR_YELLOW);
    printf("\n\n                      /|\\                        \n");
    printf("                      \\|/                        \n");
    printf("                    . ' ` .                      \n");
    printf("              *  . '        ` .  *               \n");
    printf("             / \\'      *       `/ \\              \n");
    printf("  *    /   \\     / \\      /   \\       *     *\n");
    printf("   `  \\ \'. /     \\\\///   \\\\\\ //     \\.\\ /  '     \n");
    printf("    `_____________________________________'      \n");
    printf("     |                                   |       \n");
	printf("     *    "); iSetColor(I_COLOR_WHITE); printf("ALL NON-MINE TILES REVEALED"); iSetColor(I_COLOR_YELLOW); printf("    *         \n");
    printf("     |    --*-----*----*----*-----*--    |         \n");
    printf("     *         "); iSetColor(I_COLOR_WHITE); printf("YOU WIN! %02d:%02d:%02d", hours, minutes, seconds); iSetColor(I_COLOR_YELLOW); printf("         *         \n");
    printf("     |___________________________________|       \n");
    printf("     _____________________________________       \n");
    printf("                                                      \n");
    iSetColor(I_COLOR_WHITE);
}

/*
This function displays ASCII art when player quit the game, showing the time elapsed in hours, minutes, and seconds.
@param time - total time elapsed in seconds when the game was quit.
@return void
Pre-condition: time is a nonnegative integer 
*/
void asciiGameQuit(int time){
	int hours, minutes, seconds;
	
	hours = time / 3600;
	minutes = (time % 3600) / 60;
	seconds = time % 60;
	
	iSetColor(I_COLOR_PURPLE);
    printf("                                                      \n");
    printf("         *********                                    \n");
    printf("       ***       ***                                  \n");
    printf("     ***           ***                                \n");
    printf("    **               **                               \n");
    printf("   **  x x      x x   **                              \n");
    printf("  **    x        x     **   "); iSetColor(I_COLOR_WHITE); printf("ALL QUITTERS ARE LOSERS   \n"); iSetColor(I_COLOR_PURPLE);
    printf("  **   x x      x x    **   -----------------------   \n");
    printf("  **                   **     "); iSetColor(I_COLOR_WHITE); printf("GAME LOST! %02d:%02d:%02d     \n", hours, minutes, seconds); iSetColor(I_COLOR_PURPLE);
    printf("  **                   **                             \n");
    printf("   **     *******     **                              \n");
    printf("    **   *       *   **                               \n");
    printf("     ***           ***                                \n");
    printf("       ***       ***                                  \n");
    printf("         *********                                    \n");
    printf("                                                      \n");
    iSetColor(I_COLOR_WHITE);
}

/*
This function displays ASCII art start menu when the user first opens the program.
@return void
Pre-condition: none
*/
void asciiStartMenu() {
	int quit;
	iSetColor(I_COLOR_YELLOW);
    printf("                                                                         \n");
    printf("        *********         "); iSetColor(I_COLOR_WHITE); printf("WELCOME TO\n"); iSetColor(I_COLOR_YELLOW);
    printf("      ***       ***                                                       \n");
    printf("    ***           ***     "); iSetColor(I_COLOR_CYAN); printf("###  #  ##  ###  ## # # ### ### ### ### ###    \n"); iSetColor(I_COLOR_YELLOW);
    printf("   **               **    "); iSetColor(I_COLOR_CYAN); printf("###  #  # # ##   #  ### ##  ##  # # ##  #      \n"); iSetColor(I_COLOR_YELLOW);
    printf("  **    **     **    **   "); iSetColor(I_COLOR_CYAN); printf("# #  #  # # ### ##  ### ### ### ### ### #      \n"); iSetColor(I_COLOR_YELLOW);
    printf(" **     **     **     **                                  "); iSetColor(I_COLOR_CYAN); printf("#             \n"); iSetColor(I_COLOR_YELLOW);
    printf(" **                   **                                                 \n");
    printf(" **                   **  "); iSetColor(I_COLOR_WHITE); printf("A LOGIC PUZZLE GAME WHEREIN YOU TRY TO AVOID  \n"); iSetColor(I_COLOR_YELLOW);
    printf(" **    *         *    **  "); iSetColor(I_COLOR_WHITE); printf("HIDDEN MINES AND REVEAL ALL THE GRID TILES     \n"); iSetColor(I_COLOR_YELLOW);
    printf("  **    *       *    **                                                  \n");
    printf("   **    *******    **    "); iSetColor(I_COLOR_WHITE); printf("(RE)CREATED BY: GMLY                           \n"); iSetColor(I_COLOR_YELLOW);
    printf("    ***           ***                                                    \n");
    printf("      ***       ***       "); iSetColor(I_COLOR_WHITE); printf("--------------------------------------------   \n"); iSetColor(I_COLOR_YELLOW);
    printf("        *********         "); iSetColor(I_COLOR_WHITE); printf("PRESS ANY KEY TO CONTINUE...                   \n"); 
    printf("                                                                         \n");
    iSetColor(I_COLOR_WHITE);
    
    while(!quit){
		if(kbhit()){
            quit = 1;
		}
	}
}

/*
This function renders the menu outputs when the user is selecting through options in the main menu.
@param selection - an integer that pertains to the current selection of the arrow
@return void
Pre-condition: selection is a non-negative integer 
*/
void renderMainMenu(int selection) {
	int values[] = {3, 5, 6};
	int choice = rand() % 3;
	int color = values[choice];
	char *menuMessage[] = {
    	"|                                           |",
    	"",
    	"|                                           |",
	};
	
    char *messages[] = {
    	"+          START THE GAME!          +",
    	"+   MAKE PLAYABLE CUSTOM LEVELS!!   +",
    	"+    CHANGE CURRENT USER PLAYING    +",
    	"+   VIEW CURRENT USER GAME STATS!   +",
    	"+             END GAME              +",
	};
	
    char *menuControls[] = {
        "|       PLAY                                |",
        "|       LEVEL EDITOR                        |",
        "|       CHANGE PROFILE                      |",
        "|       VIEW STATISTICS                     |",
        "|       QUIT                                |",
    };
    for (int i = 0; i < 3; i++) {
        if (i == 1) {
        	printf("|   ");
        	//iSetColor(rand() % (6 - 3 + 1) + 3); // randomly choose from range 3 to 6 of iSetColors
            iSetColor(color);
			printf("%s", messages[selection]);
            iSetColor(I_COLOR_WHITE);
            printf("   |\n");
        } else {
        	iSetColor(I_COLOR_WHITE);
            printf("%s\n", menuMessage[i]);
        }
    }
    
    for (int i = 0; i < 5; i++) {
        if (i == selection) {
        	if (i == 0){
				printf("|     ");
				iSetColor(I_COLOR_YELLOW); 
				printf("> PLAY");
				iSetColor(I_COLOR_WHITE);
				printf("                                |\n");
			}
            if (i == 1){
				printf("|     ");
				iSetColor(I_COLOR_YELLOW); 
				printf("> LEVEL EDITOR");
				iSetColor(I_COLOR_WHITE);
				printf("                        |\n");
			}
			if (i == 2){
				printf("|     ");
				iSetColor(I_COLOR_YELLOW); 
				printf("> CHANGE PROFILE");
				iSetColor(I_COLOR_WHITE);
				printf("                      |\n");
			}
			if (i == 3){
				printf("|     ");
				iSetColor(I_COLOR_YELLOW);
				printf("> VIEW STATISTICS");
				iSetColor(I_COLOR_WHITE);
				printf("                     |\n");
			}
			if (i == 4){
				printf("|     ");
				iSetColor(I_COLOR_RED);
				printf("> QUIT");
				iSetColor(I_COLOR_WHITE);
				printf("                                |\n");
			}
        } else {
        	iSetColor(I_COLOR_WHITE);
            printf("%s\n", menuControls[i]);
        }
    }
}

/*
This function renders the menu outputs when the user is selecting through gamemode options in the play menu
@param selection - an integer that pertains to the current selection of the arrow
@return void
Pre-condition: selection is a non-negative integer 
*/
void renderMenuPlay(int selection) {
	char *menu[] = {
        "|              CLASSIC               |",
        "|              CUSTOM                |",
        "|              GO BACK               |",
    };

    for (int i = 0; i < 3; i++) {
		if (i == selection) {
		    if (i == 0){
		        printf("|            ");
		        iSetColor(I_COLOR_YELLOW); 
		        printf("> CLASSIC");
		        iSetColor(I_COLOR_WHITE);
		        printf("               |\n");
		    }
		    if (i == 1){
		        printf("|            ");
		        iSetColor(I_COLOR_YELLOW); 
		        printf("> CUSTOM");
		        iSetColor(I_COLOR_WHITE);
		        printf("                |\n");
		    }
		    if (i == 2){
		        printf("|            ");
		        iSetColor(I_COLOR_YELLOW); 
		        printf("> GO BACK");
		        iSetColor(I_COLOR_WHITE);
		        printf("               |\n");
		    }
		} else {
		    printf("%s\n", menu[i]);
		}
    }
}

/*
This function renders the menu outputs when the user is selecting through difficulty options in the playClassic menu
@param selection - an integer that pertains to the current selection of the arrow
@return void
Pre-condition: selection is a non-negative integer 
*/
void renderMenuPlayClassic(int selection) {
	char *menu[] = {
        "|              EASY                  |",
        "|              DIFFICULT             |",
        "|              GO BACK               |",
    };

    for (int i = 0; i < 3; i++) {
		if (i == selection) {
		    if (i == 0){
		        printf("|            ");
		        iSetColor(I_COLOR_YELLOW); 
		        printf("> EASY");
		        iSetColor(I_COLOR_WHITE);
		        printf("                  |\n");
		    }
		    if (i == 1){
		        printf("|            ");
		        iSetColor(I_COLOR_YELLOW); 
		        printf("> DIFFICULT");
		        iSetColor(I_COLOR_WHITE);
		        printf("             |\n");
		    }
		    if (i == 2){
		        printf("|            ");
		        iSetColor(I_COLOR_YELLOW); 
		        printf("> GO BACK");
		        iSetColor(I_COLOR_WHITE);
		        printf("               |\n");
		    }
		} else {
		    printf("%s\n", menu[i]);
		}
    }
}

/*
This function renders the menu outputs when the user is selecting through options in the levelEditor menu
@param selection - an integer that pertains to the current selection of the arrow
@return void
Pre-condition: selection is a non-negative integer 
*/
void renderMenuLevel(int selection) {
	char *menu[] = {
	    "|        CREATE a new level          |",
	    "|        EDIT an existing level      |",
	    "|        DELETE an existing level    |",
	    "|        RETURN to main menu         |"
	};

    for (int i = 0; i < 4; i++) {
		if (i == selection) {
		    if (i == 0){
		        printf("|      ");
		        iSetColor(I_COLOR_YELLOW); 
		        printf("> CREATE a new level");
		        iSetColor(I_COLOR_WHITE);
		        printf("          |\n");
		    }
		    if (i == 1){
		        printf("|      ");
		        iSetColor(I_COLOR_YELLOW); 
		        printf("> EDIT an existing level");
		        iSetColor(I_COLOR_WHITE);
		        printf("      |\n");
		    }
		    if (i == 2){
		        printf("|      ");
		        iSetColor(I_COLOR_YELLOW); 
		        printf("> DELETE an existing level");
		        iSetColor(I_COLOR_WHITE);
		        printf("    |\n");
		    }
		    if (i == 3){
		        printf("|      ");
		        iSetColor(I_COLOR_YELLOW);
		        printf("> RETURN to main menu");
		        iSetColor(I_COLOR_WHITE);
		        printf("         |\n");
		    }
		} else {
		    printf("%s\n", menu[i]);
		}
    }
}

/*
This function renders the menu outputs when the user is selecting through options in the changeProfile menu
@param selection - an integer that pertains to the current selection of the arrow
@return void
Pre-condition: selection is a non-negative integer 
*/
void renderMenuProfile(int selection) {
    char *menu[] = {
        "|         SELECT profile             |",
        "|         CREATE NEW profile         |",
        "|         DELETE profile             |",
        "|         RETURN to main menu        |"
    };

    for (int i = 0; i < 4; i++) {
		if (i == selection) {
		    if (i == 0){
		        printf("|       ");
		        iSetColor(I_COLOR_YELLOW); 
		        printf("> SELECT profile");
		        iSetColor(I_COLOR_WHITE);
		        printf("             |\n");
		    }
		    if (i == 1){
		        printf("|       ");
		        iSetColor(I_COLOR_YELLOW); 
		        printf("> CREATE NEW profile");
		        iSetColor(I_COLOR_WHITE);
		        printf("         |\n");
		    }
		    if (i == 2){
		        printf("|       ");
		        iSetColor(I_COLOR_YELLOW); 
		        printf("> DELETE profile");
		        iSetColor(I_COLOR_WHITE);
		        printf("             |\n");
		    }
		    if (i == 3){
		        printf("|       ");
		        iSetColor(I_COLOR_YELLOW);
		        printf("> RETURN to main menu");
		        iSetColor(I_COLOR_WHITE);
		        printf("        |\n");
		    }
		} else {
		    printf("%s\n", menu[i]);
		}
    }
}

