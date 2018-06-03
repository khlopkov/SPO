#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc != 2) return 1;
	if (argv[1][0] < '0' || argv[1][0] > '3') return 1;
	int i, p;
	FILE* bin;
	char fileName[5] = "bin1\0";
	fileName[3] = argv[1][0];
	bin = fopen(fileName, "rb");
	for(i = 0; i < 10000; i++) {
		fread(&p, sizeof(int), 1, bin);
		printf("%d, ", p); 
	}
	fclose(bin);
	return 0;
}