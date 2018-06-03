#include <stdio.h>
#include <Windows.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdbool.h>
#include "genBIN.h"
#include "cpy.h"
#include "lab1_dll.h"
#include "sort.h"
using namespace std;
using namespace std::chrono;

#define MAX_BUFFER 1024
#define INITIAL_SIZE 512
#define SORT_FILE_1 "bin1"
#define SORT_FILE_2 "bin2"
#define SORT_FILE_3 "bin3"

enum methods{
    COPYMETHOD_C,
    COPYMETHOD_WINDOWS,
    COPYMETHOD_COPYFILE,
    COPYMETHOD_BENCHMARK,
    COPYMETHOD_INVALID = -1
};


#define FILENAME_SIZE           16
#define INITIAL_FILESIZE_KB     1 << 7     //128 KB

void calculateAveragePoints(string filename)
{
	ifstream input(filename, ios_base::in);

	int count = 0;
	float sum = 0;

    int semester, points;
    string discipline, teacher;
	while (input >> semester >> discipline >> points >> teacher)
	{
		sum += points;
		count++;
	}

	input.close();

    float avg = sum / count;
    if (!isnan(avg)) cout << "Average points: " << avg << endl;
	else cout << "Incorrect file format; unable to parse points." << endl;
}

int getCopyMethod(string argument)
{
    if (argument == "c") return COPYMETHOD_C;
    else if (argument == "windows") return COPYMETHOD_WINDOWS;
    else if (argument == "copyfile") return COPYMETHOD_COPYFILE;
    else if (argument == "benchmark") return COPYMETHOD_BENCHMARK;
    return COPYMETHOD_INVALID;
}

time_t execTime(int (*function)(char*, char*), char* originalName, char* copyName)
{
    auto startTime = steady_clock::now();
    int result = function(originalName, copyName);
    if (result == EXIT_FAILURE) return -1;
    
    auto duration = duration_cast<microseconds>(steady_clock::now() - startTime);
    return duration.count();
}

int benchmark()
{
    char originalName[FILENAME_SIZE], copyName[FILENAME_SIZE];
    tmpnam_s(originalName, FILENAME_SIZE);
    tmpnam_s(copyName, FILENAME_SIZE);

    FILE* original;

    cout << "+--------+----------------+----------------+----------------+" << endl <<
            "|  SIZE  |       C        |     WINDOWS    |    COPYFILE    |" << endl <<
            "+--------+----------------+----------------+----------------+" << endl;

    size_t size_kb = INITIAL_FILESIZE_KB, oldSize_kb = 0;
    time_t time1, time2, time3;

    char buffer[1024];
    for (int i = 0; i < 1024; i++) buffer[i] = '1';

    for (int i = 0; i < 5; i++)
    {
        for (int k = 0; k < size_kb - oldSize_kb; k++)
        {
            original = fopen(originalName + 1, "ab");
            fwrite(buffer, 1, 1024, original);
            fclose(original);
        }

        size_t displaySize = size_kb;
        bool megabytes = displaySize >= 1024;
        if (megabytes) displaySize = displaySize >> 10;
        time1 = execTime(&copy_c, originalName + 1, copyName + 1);
        time2 = execTime(&copy_windows, originalName + 1, copyName + 1);
        time3 = execTime(&copy_copyfile, originalName + 1, copyName + 1);
        printf("|%5i %cB|%12i mcs|%12i mcs|%12i mcs|\n", displaySize, megabytes ? 'M' : 'K', time1, time2, time3);

        oldSize_kb = size_kb; 
        size_kb = size_kb << 1;
    }

    remove(originalName + 1);
    remove(copyName + 1);

    cout << "+--------+----------------+----------------+----------------+" << endl;
    return EXIT_SUCCESS;
}

bool validateArgs(int argc, char* argv[])
{
    if (argc != 2 && argc != 4 && argc != 5) return false;
    if (argc == 5 && (string)argv[4] != "--avg") return false;

    int copyMethod = getCopyMethod(argv[1]);
    if (copyMethod == COPYMETHOD_INVALID) return false;
    if (argc == 2 == !(copyMethod == COPYMETHOD_BENCHMARK)) return false;

    return true;
}

int lab1()
{
    string copymethods, tempstr;
    bool avg = false;
    char *inputFileName, *outputFileName;
    cout << "Input copymethod {c|windows|copyfile}: ";
    cin >> copymethods;
    cin.clear();
    int copyMethodCode = getCopyMethod(copymethods);
    while (copyMethodCode == -1){
        cout << "Incorrect copy method, please input correct {c|windows|copyfile}: ";
        cin >> copymethods;
        cin.clear();
        copyMethodCode = getCopyMethod(copymethods);
    }
    if (copyMethodCode != COPYMETHOD_BENCHMARK){
        cout << "Please, input input file name: ";
        cin >> tempstr;
        inputFileName = new char[tempstr.length() + 1];
        strcpy(inputFileName, tempstr.c_str());
        cin.clear();
        cout << "Please, input output file name: ";
        cin >> tempstr;
        outputFileName = new char[tempstr.length() + 1];
        strcpy(outputFileName, tempstr.c_str());
        cin.clear();
        cout << "Calculate average points? (y/n)";
        cin >> tempstr;
        if (tempstr == "y"){
            avg = true;
        }
        cin.clear();
    }
    int copyExitcode;
    switch (copyMethodCode)
    {
        case COPYMETHOD_C:
            copyExitcode = copy_c(inputFileName, outputFileName);
            if(copyExitcode == EXIT_FAILURE) return EXIT_FAILURE;
            break;

        case COPYMETHOD_WINDOWS:
            copyExitcode = copy_windows(inputFileName, outputFileName);
            if(copyExitcode == EXIT_FAILURE) return EXIT_FAILURE;
            break;

        case COPYMETHOD_COPYFILE:
            copyExitcode = copy_copyfile(inputFileName, outputFileName);
            if(copyExitcode == EXIT_FAILURE) return EXIT_FAILURE;
            break;

        case COPYMETHOD_BENCHMARK: 
            return benchmark();
    }

    cout << "File was succesfully copied." << endl;
    if (avg) calculateAveragePoints(inputFileName); 
    cin >> tempstr;
    cin.clear();
    delete [] inputFileName;
    delete [] outputFileName;
    return copyExitcode;
}
///////////////////////////////////////////////////////////////////
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
    string str = "Binary inserts ";
    char *method = new char[str.length() + 1];
    strcpy(method, str.c_str());
    Line3(179, 32, width, one, two, three, (char*)method, size);
    Line(192, 193, 217, 196, width);
}

void memory_benchmark(long size){
    int one, two, three, four, five, six;
    __int64 StartTicks, EndTicks, Freq;
    QueryPerformanceCounter( (LARGE_INTEGER *)&StartTicks );
    string str = SORT_FILE_1;
    char *sort_file = new char[str.length() + 1];
    strcpy(sort_file, str.c_str());
    sort_heaps(sort_file);
    QueryPerformanceCounter( (LARGE_INTEGER *)&EndTicks );
    QueryPerformanceFrequency( (LARGE_INTEGER *)&Freq );
    one = (EndTicks - StartTicks) * 1000 / Freq;
    one = check(one);

    QueryPerformanceCounter( (LARGE_INTEGER *)&StartTicks );
    str = SORT_FILE_2;
    char *sort_file2 = new char[str.length() + 1];
    strcpy(sort_file2, str.c_str());
    sort_mapping(sort_file2);
    QueryPerformanceCounter( (LARGE_INTEGER *)&EndTicks );
    QueryPerformanceFrequency( (LARGE_INTEGER *)&Freq );
    two = (EndTicks - StartTicks) * 1000 / Freq;
    two = check(two);

    QueryPerformanceCounter( (LARGE_INTEGER *)&StartTicks );
    str = SORT_FILE_3;
    char *sort_file3 = new char[str.length() + 1];
    strcpy(sort_file3, str.c_str());
    sort_basepoints(sort_file3);
    QueryPerformanceCounter( (LARGE_INTEGER *)&EndTicks );
    QueryPerformanceFrequency( (LARGE_INTEGER *)&Freq );
    three = (EndTicks - StartTicks) * 1000 / Freq;
    three = check(three);
    
    write_table(one, two, three, size);
}

int lab2() {    
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
