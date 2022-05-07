#include "pch.h"
#include "Massdll.h"

using namespace std;

class Mass::TreeNode
{
public:
	int key;
	char* value;

	TreeNode* l;
	TreeNode* r;

	TreeNode() {};

	TreeNode(int i, char* s)
	{
		key = i;
		value = s;

		l = NULL;
		r = NULL;
	};
};

bool Mass::Conts(int i) {
	TreeNode* temp = fst;

	int f = 0; // зачем я это добавил...

	while (temp != NULL && f == 0) {
		if (i == temp->key) {
			f++;
			return true;
		}
		else if (i < temp->key) {
			temp = temp->l;
		}
		else {
			temp = temp->r;
		}
	}
	if (f == 0) {
		return false;
	}
};

Mass::Mass()
{
	fst = NULL;
};

DWORD WINAPI MyThread(PVOID p)
{
	Mass& m = *((Mass*)p);
	HANDLE stopThread = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"stopThread");
	HANDLE bufFull = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"bufFull");
	HANDLE bufEmpty = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"bufEmpty");

	while (WaitForSingleObject(stopThread, 0) != WAIT_OBJECT_0) {
		WaitForSingleObject(bufFull, INFINITE);

		if (WaitForSingleObject(stopThread, 0) != WAIT_OBJECT_0) {
			m.Add(m.buffer_key, _strdup(m.buffer_value));
			m.Save(1);
		}

		SetEvent(bufEmpty);
	}
	return 0;
}

Mass::Mass(int i)
{
	fst = NULL;
	buffer_key = 0;
	buffer_value = _strdup("");
	DWORD id;
	HANDLE fileTo = CreateFile(L"file.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(fileTo);
	AddThread = CreateThread(NULL, 0, MyThread, this, 0, &id);
};

void Mass::CallAdd(int key, const char* value)
{
	HANDLE bufFull = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"bufFull");
	HANDLE bufEmpty = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"bufEmpty");
	WaitForSingleObject(bufEmpty, INFINITE);
	buffer_key = key;
	buffer_value = _strdup(value);
	SetEvent(bufFull);
}

void Mass::Add(int i, char* s) {
	if (fst == NULL) {
		fst = (TreeNode*)malloc(sizeof(*fst));
		fst->key = i;
		fst->value = _strdup(s);
		fst->l = NULL;
		fst->r = NULL;
	}
	else
	{
		if (Conts(i)) {
			cout << "\nЭлемент с таким ключом уже существует\n";
			system("pause");
		}
		else {
			TreeNode* temp = fst;

			TreeNode* parent = NULL;

			while (temp != NULL) {
				parent = temp;
				if (i < temp->key) {
					temp = temp->l;
				}
				else {
					temp = temp->r;
				}
			}

			TreeNode* node = (TreeNode*)malloc(sizeof(*node));
			node->key = i;
			node->value = _strdup(s);
			node->l = NULL;
			node->r = NULL;

			if (i < parent->key)
				parent->l = node;
			else
				parent->r = node;
		}
	}
};

void Mass::Find(int i) {
	TreeNode* temp = fst;

	int f = 0;

	while (temp != NULL && f == 0) {
		if (i == temp->key) {
			cout << temp->value;
			f++;
		}
		else if (i < temp->key) {
			temp = temp->l;
		}
		else {
			temp = temp->r;
		}
	}
	if (f == 0) {
		cout << "Элемент с таким ключом не найден";
	}
};

void Mass::Print()
{
	this->Load(1);

	system("cls");
	if (fst == NULL) {
		cout << "Массив пуст";
	}
	else
	{
		Prt(fst);
	}
	cout << "\n";
}

void Mass::Prt(TreeNode* tn)
{
	if (tn == NULL) {

	}
	else
	{
		Prt(tn->l);
		Prt(tn->r);
		printf("key: %d; value: %s\n", tn->key, tn->value);
	}
}

Mass::~Mass()
{
	if (fst != NULL) {
		if (fst->l != NULL) {
			Dest(fst->l);
		}
		if (fst->r != NULL) {
			Dest(fst->r);
		}
		free(fst);
	}
}

void Mass::Dest(TreeNode* tn)
{
	if (tn->l != NULL) {
		Dest(tn->l);
	}
	if (tn->r != NULL) {
		Dest(tn->r);
	}
	free(tn);
	tn = NULL;
}


void Mass::Save()
{
	if (fst == NULL)
	{
		cout << "Массив пуст\n";
	}
	else
	{
		HANDLE file = CreateFile(L"file.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		Write(file, fst);
		CloseHandle(file);
		cout << "Сохранение успешно\n";
	}
	system("pause");
}

void Mass::Save(int i)
{
	HANDLE file = CreateFile(L"file.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	Write(file, fst);
	CloseHandle(file);
}

void Mass::Write(HANDLE file, TreeNode* nd)
{
	if (nd == NULL)
	{
	}
	else
	{
		DWORD dw = 0;
		int l = strlen(nd->value);
		int key = nd->key;
		char* value = nd->value;
		WriteFile(file, &key, sizeof(int), &dw, NULL);
		WriteFile(file, &l, sizeof(int), &dw, NULL);
		for (int i = 0; i < l; i++) {
			WriteFile(file, &value[i], sizeof(char), &dw, NULL);
		}
		Write(file, nd->l);
		Write(file, nd->r);
	}

}

void Mass::Load(int i)
{
	HANDLE file = CreateFile(L"file.dat", GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (this->fst != NULL) {
		Dest(this->fst);
	}

	this->fst = NULL;

	DWORD end = SetFilePointer(file, 0, 0, FILE_END);
	SetFilePointer(file, 0, 0, FILE_BEGIN);
	DWORD dw = 0;
	int key;
	int length = 0;

	while (SetFilePointer(file, 0, 0, FILE_CURRENT) < end)
	{
		ReadFile(file, &key, sizeof(int), &dw, NULL);
		ReadFile(file, &length, sizeof(int), &dw, NULL);

		char* value = (char*)malloc(sizeof(char) * 100);
		for (int i = 0; i < length; i++)
		{
			ReadFile(file, &value[i], sizeof(char), &dw, NULL);
		}
		value = (char*)realloc(value, sizeof(char) * (length + 1));
		value[length] = '\0';
		this->Add(key, value);
	}
	CloseHandle(file);
}