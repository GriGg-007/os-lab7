#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <stdio.h>
#include <windows.h>

#include "Massdll.h"

using namespace std;


int main()
{
	HANDLE bufEmpty = CreateEvent(NULL, FALSE, TRUE, L"bufEmpty");
	HANDLE bufFull = CreateEvent(NULL, FALSE, FALSE, L"bufFull");
	HANDLE stopThread = CreateEvent(NULL, TRUE, FALSE, L"stopThread");

	system("chcp 1251");
	system("cls");

	Mass* m = new Mass(1);

	bool f = true;
	int i;

	while (f) {
		m->Print();

		cout << "\n1 - Добавить пару\n";
		cout << "0 - Выход\n";

		cin >> i;

		switch (i)
		{
		case 1: {
			cout << "Введите ключ:\n";
			int k;
			cin >> k;

			cout << "Введите значение:\n";

			char c1[100];
			getchar();
			gets_s(c1);

			m->CallAdd(k, c1);
			WaitForSingleObject(bufEmpty, INFINITE);
			SetEvent(bufEmpty);
		}
			  break;
		case 0: {
			f = false;
		}
			  break;

		default: {}
			   break;
		}
	}

	SetEvent(stopThread);
	SetEvent(bufFull);
	WaitForSingleObject(m->AddThread, INFINITE);
	system("pause");
	return 0;
}