#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//insert function for reading .dat file per path

void transferSnapshot(char destFile[], char sourceFile[]){
	FILE *fsource;
	FILE *fdest;
	char n;
	
	fsource = fopen(sourceFile, "r");
	fdest = fopen(destFile, "w");
	
	n = fgetc(fsource);
	while (n != EOF){
		fputc(n, fdest);
		n = fgetc(fsource);
	}
	
	printf("copy success");
	
	fclose(fsource);
	fclose(fdest);
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
