#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int copy_c(char* originalName, char* copyName)
{
	char buffer[BUFSIZ];

	FILE* oldFile = fopen(originalName, "rb");
    if (oldFile == NULL)
	{
		perror(originalName);
		return EXIT_FAILURE;
	}

	FILE* newFile = fopen(copyName, "wb");
	if (newFile == NULL)
	{
		perror(copyName);
		return EXIT_FAILURE;
	}

	size_t inBytes, outBytes;
	while ((inBytes = fread(buffer, 1, BUFSIZ, oldFile)) > 0)
	{
		outBytes = fwrite(buffer, 1, inBytes, newFile);

		if (outBytes != inBytes)
		{
			perror("Failed to copy file via C functions.");
			return EXIT_FAILURE;
		}
	}

	fclose(oldFile);
	fclose(newFile);

    return EXIT_SUCCESS;
}

int copy_windows(char* originalName, char* copyName)
{
	CHAR buffer[BUFSIZ];

    HANDLE hIn = CreateFile(originalName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hIn == INVALID_HANDLE_VALUE)
	{
		cout << "Failed to open input file: error code " << GetLastError();
		return EXIT_FAILURE;
	}

    HANDLE hOut = CreateFile(copyName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, NULL);
    if (hOut == INVALID_HANDLE_VALUE) 
    {
		cout << "Failed to open output file: error code " << GetLastError();
		return EXIT_FAILURE;
	}

	DWORD nIn, nOut;
    while (ReadFile(hIn, buffer, BUFSIZ, &nIn, NULL) && nIn > 0)
	{
		WriteFile(hOut, buffer, nIn, &nOut, NULL);
		if (nIn != nOut)
		{
			cout << "Failed to copy file: error code " << GetLastError();
			return EXIT_FAILURE;
		}
	}

	CloseHandle(hIn);
	CloseHandle(hOut);

    return EXIT_SUCCESS;
}

int copy_copyfile(char* originalName, char* copyName)
{
    int success = CopyFile(originalName, copyName, FALSE);
    if (!success) cout << "Failed to copy file: error code " << GetLastError();
	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}