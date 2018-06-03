#include <stdio.h>
#include <Windows.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "cpy.h"
using namespace std;
using namespace std::chrono;

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

int main()
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