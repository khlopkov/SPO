#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

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