#include "lab1_dll.h"
#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;
int main(){
	string item = "";
	while (item != "3"){
		cout << "Chose laboratory work:\n";
		cout << "1. First laboratory work of SPO\n";
		cout << "2. Second laboratory work of SPO\n";
		cout << "3. Exit\n";
		cin >> item;
		if (item == "1"){
			lab1();
		}else {
			if (item == "2"){
				lab2();
			}
		}

		cin.clear();
	}
}