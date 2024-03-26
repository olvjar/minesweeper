#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "interface.c"

void controls(){
	int x, y;
    int key;

    iHideCursor();
    while(1){
        if(kbhit()){
            key = getch();
                if(key == 27){ // escape
                    break;
                }
                if(key == 80){ // down
                	printf("d");
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

int main(){
	controls();
}

