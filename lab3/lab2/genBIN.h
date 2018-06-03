#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int genBin(long size) {
	int  p;
	FILE* bin;
	
	srand(time(NULL));
	for(int j = 1; j < 4; j++){
		char c = '0' + j;
		char fileName[5] = "bin";
		fileName[3] = c;
		fileName[4] = '\0';
		bin = fopen(fileName, "wb");
		for(long i = 0; i < size; i++) {
			p = rand() % 10240;
			//printf("%d, ", p); 
			fwrite(&p, sizeof(int), 1, bin);
		}
		fclose(bin);
	}
	return 0;
}