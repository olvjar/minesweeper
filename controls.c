#include <stdio.h>
#include <stdlib.h>

#include "interface.c"
/*
void controls(int x, int y){
    int key;

    iHideCursor();
    while(1){
        if(kbhit()){
            key = getch();
            iClear(x, y, 1, 1);
                if(key == 27){ // escape
                    break;
                }
                if(key == 80){ // down
                    y--;
                }
                if(key == 72){ // up
                    y++;
                }
                if(key == 75){ // left
                    x--;
                }
                if(key == 77){ // right
                    x++;
                }
            iMoveCursor(x, y);
        }
    }
}
*/

// ANSI escape codes for arrow keys
#define ARROW_UP    "\x1b[A"
#define ARROW_DOWN  "\x1b[B"
#define ARROW_RIGHT "\x1b[C"
#define ARROW_LEFT  "\x1b[D"

// Function to handle arrow key input
void controls() {
    char input[3];
    while (1) {
        fgets(input, 3, stdin); // Capture arrow key input
        
        // Check which arrow key is pressed
        if (input[0] == '\x1b' && input[1] == '[') {
            switch (input[2]) {
                case 'A':
                    printf("Up arrow key pressed\n");
                    break;
                case 'B':
                    printf("Down arrow key pressed\n");
                    break;
                case 'C':
                    printf("Right arrow key pressed\n");
                    break;
                case 'D':
                    printf("Left arrow key pressed\n");
                    break;
                default:
                    // Do nothing if other keys are pressed
                    break;
            }
        }
        
        if (input[0] == 27) // Escape key
            break;
    }
}
