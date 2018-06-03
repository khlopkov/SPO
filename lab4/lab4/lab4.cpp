// lab4.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>

using namespace std;

time_t filetimeToTime_t(FILETIME filetime)
{
	return ((time_t)filetime.dwHighDateTime << 32) + filetime.dwLowDateTime;
}

time_t processTime(HANDLE hProcess)
{
	FILETIME creationTime, exitTime, kernelTime, userTime;
	ZeroMemory(&creationTime, sizeof(creationTime));
	ZeroMemory(&exitTime, sizeof(exitTime));

	GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime);

	time_t startTime = filetimeToTime_t(creationTime);
	time_t endTime = filetimeToTime_t(exitTime);
	return (endTime - startTime) / 10;
}

int wmain(int argc, wchar_t* argv[])
{
	if (argc != 3 || (wstring)argv[1] != L"asc" && (wstring)argv[1] != L"desc")
	{
		cout << "Usage: " << endl << "spo4 {asc|desc} x" << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	STARTUPINFO startupInfo[3];
	PROCESS_INFORMATION processInfo[3];
	HANDLE hProcess[3];

	wstring args = L"copy.exe " + (wstring)argv[1] + L" " + (wstring)argv[2];

	for (int i = 0; i < 3; i++)
	{
		ZeroMemory(&startupInfo[i], sizeof(startupInfo[i]));
		startupInfo[i].cb = sizeof(startupInfo[i]);
		ZeroMemory(&processInfo[i], sizeof(processInfo[i]));

		bool success = CreateProcessW(L".\\sort.exe", (LPWSTR)args.c_str(),
			NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
			&startupInfo[i], &processInfo[i]);

		if (!success)
		{
			cout << "CreateProcess failed: " << GetLastError() << endl;
			system("pause");
			return EXIT_FAILURE;
		}

		hProcess[i] = processInfo[i].hProcess;
	}

	WaitForMultipleObjects(3, hProcess, TRUE, INFINITE);

	DWORD handleCount;
	GetProcessHandleCount(GetCurrentProcess(), &handleCount);

	cout << "+-------+----------------+" << endl <<
		"|  PID  |      TIME      |" << endl <<
		"+-------+----------------+" << endl;

	for (int i = 0; i < 3; i++)
	{
		time_t time = processTime(hProcess[i]);
		printf("|%7u|%12lli mcs|\n", GetProcessId(hProcess[i]), time);

		CloseHandle(hProcess[i]);
	}

	cout << "+-------+----------------+" << endl << endl;
	cout << "Handles count: " << handleCount << endl << endl;

	system("pause");
}

