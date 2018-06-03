#pragma warning(disable:4047)
#pragma warning(disable:4133)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "genBIN.h"

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
//сортировка бмнарными вставками
void sort_binary_inserts(int* arrayPtr, int size) 
{
	int x;
    int left;
    int right;
    int mid;
    for (int i = 1;  i < size; i++) 
        if (arrayPtr[i-1] > arrayPtr[i]){
            x = arrayPtr[i];
            left = 0;  
            right = i-1;  
            do {
                mid = (left + right)/2;    
                if  (arrayPtr[mid] < x ) left = mid + 1;  
                else  right = mid - 1;      
            } while (left <= right);
            for (int j = i - 1; j >= left; j--)
                arrayPtr[j+1] = arrayPtr[j];  
            arrayPtr[left] = x;
      }
}

int cmp(const void * a, const void * b) {
   return (*(int*)a - *(int*)b);
}

void sort_heaps(char* filename) {
	FILE* file;
	long int size;
	int count;
	LPVOID lpMem;		
	int* buffer;
	HANDLE heap;
	int a;
	
	file = fopen(filename, "rb");
	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	count = size / sizeof(int);
	heap = HeapCreate(0, 0x01000, 0);
	lpMem = HeapAlloc(heap, 0, size);
	buffer = (int*)lpMem;
	fread(buffer, sizeof(int), count, file);
	fclose(file);
	
	sort_binary_inserts(buffer, count);

	HeapFree(heap, 0, lpMem);
	HeapDestroy(heap);
}

void sort_mapping(char* filename) {
	HANDLE hFile, hMap;
	DWORD dwFileSize;
	int* memory;
	
	hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	memory = (int*)MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, dwFileSize);
	sort_binary_inserts(memory, dwFileSize/sizeof(int));
	UnmapViewOfFile(memory);
	CloseHandle(hMap);
	CloseHandle(hFile);
}

void sort_basepoints(char* filename) {
	int *it, *end;
	HANDLE hFile, hMap;
	DWORD dwFileSize;
	char* memory;
	int* data;
	int i, count;
	
	hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	count = dwFileSize / sizeof(int);
	memory = (char*)MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, dwFileSize);
	data = (int*)malloc(dwFileSize);
	
	i = 0;
	it = (int*) memory;
	end = it + dwFileSize/sizeof(int);
	for(it; it < end; it++)
		data[i++] = *it;
	

	sort_binary_inserts(data, count);
	
	i = 0;
	it = (int*)memory;
	end = it + count;
	for(it; it < end; it++)
		(*it) = data[i++];

	UnmapViewOfFile(memory);
	CloseHandle(hMap);
	CloseHandle(hFile);
}

void memory_benchmark(long size){
	SYSTEMTIME st, ft;
	int one, two, three, four, five, six;

	GetSystemTime(&st);
	sort_heaps(SORT_FILE_1);
	GetSystemTime(&ft);
	one = ft.wMilliseconds - st.wMilliseconds;
	one = check(one);

	GetSystemTime(&st);
	sort_mapping(SORT_FILE_2);
	GetSystemTime(&ft);
	two = ft.wMilliseconds - st.wMilliseconds;
	two = check(two);

	GetSystemTime(&st);
	sort_basepoints(SORT_FILE_3);
	GetSystemTime(&ft);
	three = ft.wMilliseconds - st.wMilliseconds;
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
