// Listing30_1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <windows.h> 
#include <fstream> 
#include<iostream>

using namespace std;
int main()
{
	int a[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	wchar_t file_name[] = L"Demo.txt";
	HANDLE hFile, hMapping; // дескрипторы файла и объекта отображения 
	int* ptr; // для указателя на массив 
	// открываем файл для вывода 
	ofstream out(file_name, ios::out | ios::binary);
	if (!out)
	{
		cerr << "File constructor failed." << endl;
		return 0;
	}
	// выводим исходный массив в файл и на консоль 
	cout << "Initial array: ";
	for (int i = 0; i < 10; ++i)
	{
		out.write((char*)&a[i], sizeof(int));
		cout << a[i] << ' ';
	}
	cout << endl;
	// закрываем выходной файл 
	out.close();
	// ---------------------------------------------------------------------- 
	// открываем файл для отображения в память 
	hFile = CreateFile(file_name, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		cerr << "Create file failed." << endl;
		return GetLastError();
	}
	// открываем объект, отображающий файл в память 
	hMapping = CreateFileMapping(
		hFile, // дескриптор открытого файла 
		NULL, // атрибуты защиты по умолчанию 
		PAGE_READWRITE, // режим доступа 
		0, 0, // размер объекта отображения равен размеру файла 
		NULL); // имя не используем 
	if (!hMapping)
	{
		cerr << "Create file mapping failed." << endl;
		return GetLastError();
	}
	// создаем вид файла 
	ptr = (int*)MapViewOfFile(
		hMapping, // дескриптор объекта отображения 
		FILE_MAP_WRITE, // режим доступа к виду 
		0, 0, // отображаем файл с начала 
		0); // отображаем весь файл 
	// изменяем значения элементов массива 
	for (int i = 0; i < 10; ++i)
		ptr[i] += 10;
	// отменяем отображение файла в память 
	if (!UnmapViewOfFile(ptr))
	{
		cerr << "Unmap view of file failed." << endl;
		return GetLastError();
	}
	// закрываем объект отображения файла в память 
	if (!CloseHandle(hMapping))
	{
		cerr << "Close file failed." << endl;
		return GetLastError();
	}
	// закрываем файл 
	if (!CloseHandle(hFile))
	{
		cerr << "Close file failed." << endl;
		return GetLastError();
	}
	// ---------------------------------------------------------------------- 
	 // открываем файл для ввода 
	ifstream in(file_name, ios::in | ios::binary);
	if (!in)
	{
		cerr << "File constructor failed." << endl;
		return 0;
	}
	// вводим финальный массив из файла и выводим на консоль 
	cout << "Final array: ";
	for (int i = 0; i < 10; ++i)
	{
		in.read((char*)&a[i], sizeof(int));
		cout << a[i] << ' ';
	}
	cout << endl;
	// закрываем входной файл 
	in.close();
	return 0;
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
