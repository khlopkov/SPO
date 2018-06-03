#pragma warning(disable:4047)
#pragma warning(disable:4133)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "genBIN.h"
#include "lab2_dll.h"

#define MAX_BUFFER 1024
#define INITIAL_SIZE 512
#define SORT_FILE_1 "bin1"
#define SORT_FILE_2 "bin2"
#define SORT_FILE_3 "bin3"

int check(long num){
	if (num < 0) 
	{
		num = num*(-1);
	}
	return num;
};

int symbols(int Y)
{
	int count = 0;
	do {
		Y = Y / 10;
		count++;
	} while (Y >= 1);
	return count;
}

void Line(int symb1, int symb2, int symb3, int symb_, int width) {
	printf("%c", symb1);                           // Г
	for (int j = 0; j<width; j++) printf("%c", symb_); // -
	printf("%c", symb2);                           // T наоборот
	for (int j = 0; j<width; j++) printf("%c", symb_); // -
	printf("%c", symb2);                           // T наоборот
	for (int j = 0; j<width; j++) printf("%c", symb_); // -
	printf("%c", symb2);                           // T наоборот
	for (int j = 0; j<width; j++) printf("%c", symb_); // -
	printf("%c", symb3);                           // L влево
	printf("\n");

}

void Line2(int palka, int space, int width) {
	int j;
	printf("%c", palka);
	for (j = 0; j < width; j++) printf("%c", space);
	printf("%c", palka);
	for (j = 0; j < ((width - 4) / 2+1); j++) printf("%c", space);
	printf("Heap");
	for (j = 0; j < ((width - 4) / 2); j++) printf("%c", space);
	printf("%c", palka);
	for (j = 0; j < ((width - 7)/2); j++) printf("%c", space);
	printf("Mapping");
	for (j = 0; j < ((width - 7)/2); j++) printf("%c", space);
	printf("%c", palka);
	for (j = 0; j < ((width - 11)/2+1); j++) printf("%c", space);
	printf("Basepoints");
	for (j = 0; j < ((width - 11)/2); j++) printf("%c", space);
	printf("%c\n", palka);
}

void Line3(int palka, int space, int width, int one, int two, int three, char* method, long size) {
	int j;
	printf("%c", palka);
	for (j = 0; j < ((width - strlen(method) - 2 - symbols(size)) / 2); j++) printf("%c", space);
	printf("%s(%d)", method, size);
	for (j = 0; j < ((width - strlen(method)) / 2 - (strlen(method) + 2 + symbols(size)) % 2) - 2; j++) printf("%c", space);
	printf("%c", palka);
	for (j = 0; j < ((width - symbols(one) - 3) / 2 + symbols(one) % 2); j++) printf("%c", space);
	printf("%d ms", one);
	for (j = 0; j < ((width - symbols(one) - 3) / 2); j++) printf("%c", space);
	printf("%c", palka);
	for (j = 0; j < ((width - symbols(two) - 3)/2 + symbols(two) % 2); j++) printf("%c", space);
	printf("%d ms", two);
	for (j = 0; j < ((width - symbols(two) - 3)/2); j++) printf("%c", space);
	printf("%c", palka);
	for (j = 0; j < ((width - symbols(three) - 3)/2 + symbols(three) % 2); j++) printf("%c", space);
	printf("%d ms", three);
	for (j = 0; j < ((width - symbols(three) - 3)/2); j++) printf("%c", space);
	printf("%c\n", palka);
}

void write_table(int one, int two, int three, long size) {
	int width = 25;
	Line3(179, 32, width, one, two, three, "Binary inserts ", size);
	Line(192, 193, 217, 196, width);
}

void memory_benchmark(long size){
	int one, two, three, four, five, six;
	__int64 StartTicks, EndTicks, Freq;
	QueryPerformanceCounter( (LARGE_INTEGER *)&StartTicks );
	sort_heaps(SORT_FILE_1);
	QueryPerformanceCounter( (LARGE_INTEGER *)&EndTicks );
	QueryPerformanceFrequency( (LARGE_INTEGER *)&Freq );
	one = (EndTicks - StartTicks) * 1000 / Freq;
	one = check(one);

	QueryPerformanceCounter( (LARGE_INTEGER *)&StartTicks );
	sort_mapping(SORT_FILE_2);
	QueryPerformanceCounter( (LARGE_INTEGER *)&EndTicks );
	QueryPerformanceFrequency( (LARGE_INTEGER *)&Freq );
	two = (EndTicks - StartTicks) * 1000 / Freq;
	two = check(two);

	QueryPerformanceCounter( (LARGE_INTEGER *)&StartTicks );
	sort_basepoints(SORT_FILE_3);
	QueryPerformanceCounter( (LARGE_INTEGER *)&EndTicks );
	QueryPerformanceFrequency( (LARGE_INTEGER *)&Freq );
	three = (EndTicks - StartTicks) * 1000 / Freq;
	three = check(three);
	
	write_table(one, two, three, size);
}

int main() {	
	int width = 25;
	Line(218, 194, 191, 196, width);
	Line2(179, 32, width);
	Line(195, 197, 180, 196, width);

	long size = INITIAL_SIZE;
	for (int i = 0; i < 5; i++){
		genBin(size);
		memory_benchmark(size);
		size*=2;
	}
}
