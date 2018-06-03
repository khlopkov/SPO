// lab6.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <fstream>
using namespace std;

#define MAX_KEYSTROKES 255
#define CONTINUE_INPUT 0
#define STOP_INPUT	   1
#define ACTION_HOTKEY_ID 1
#define EXIT_HOTKEY_ID	 2
#define VK_K 0x4B
#define VK_Q 0x51

INPUT keystrokes[MAX_KEYSTROKES];
unsigned char keyCount = 0;

int getKeystroke(ostream* ofile)
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD inputBuffer[MAX_KEYSTROKES];
	DWORD inputCount = 0;

	ReadConsoleInput(hStdin, inputBuffer, MAX_KEYSTROKES, &inputCount);
	for (int i = 0; i < inputCount; i++)
	{
		if (inputBuffer[i].EventType != KEY_EVENT) continue;

		auto keyEvent = inputBuffer[i].Event.KeyEvent;

		INPUT input;
		auto vk = keyEvent.wVirtualScanCode;
		
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = keyEvent.wVirtualKeyCode;
		input.ki.wScan = keyEvent.wVirtualScanCode;
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		input.ki.dwFlags = KEYEVENTF_UNICODE;
		if (keyEvent.bKeyDown == FALSE) {
			input.ki.dwFlags |= KEYEVENTF_KEYUP;
		}
		else {
			char ch = tolower(MapVirtualKey(input.ki.wVk, MAPVK_VK_TO_CHAR));
			if (ch!=0)
				*ofile << ch << " ";
			if (keyCount == MAX_KEYSTROKES) return STOP_INPUT;
			
		}
	}

	return 0;
}

int main()
{
	HANDLE hMutex = CreateMutex(NULL, TRUE, L"spo_lab6_mutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		puts("Program is already running!");

		puts("Press any key to exit.");
		getchar();
		return EXIT_FAILURE;
	}
	ofstream ofile;
	ofile.open("output.txt", std::ios::out);

	puts("Enter your keystroke sequence.\n");
	while (keyCount < MAX_KEYSTROKES)
	{
		if (getKeystroke(&ofile) == STOP_INPUT) break;
	}

	ofile.close();
}