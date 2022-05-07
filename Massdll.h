#pragma once

#ifdef MASSDLL_EXPORTS
#define MASSDLL_API __declspec(dllexport)
#else
#define MASSDLL_API __declspec(dllimport)
#endif

#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <stdio.h>

class Mass {

private:
	MASSDLL_API class TreeNode;

	TreeNode* fst;

	int buffer_key;
	char* buffer_value;
	

	friend DWORD WINAPI MyThread(PVOID p);

	MASSDLL_API bool Conts(int i);

	MASSDLL_API void Prt(TreeNode* tn);

	MASSDLL_API void Dest(TreeNode* tn);

	MASSDLL_API void Write(HANDLE file, TreeNode* nd);

public:
	HANDLE AddThread;

	MASSDLL_API Mass();

	MASSDLL_API Mass(int i);

	MASSDLL_API void CallAdd(int key, const char* value);

	MASSDLL_API void Add(int i, char* s);

	MASSDLL_API void Find(int i);

	MASSDLL_API void Print();

	MASSDLL_API void Save();

	MASSDLL_API void Save(int i);

	MASSDLL_API void Load(int i);

	MASSDLL_API ~Mass();
};
