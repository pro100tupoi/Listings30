// Listing30_4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <windows.h> 
#include <fstream> 
#include <iostream>
using namespace std;
int main()
{
	int a[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	wchar_t file_name[] = L"D:\\Demo.txt";
	wchar_t mapping_name[] = L"MappingName";
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
	//---------------------------------------------------------------------- 
	 // открываем файл для отображения в память 
	hFile = CreateFile(file_name, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
		mapping_name); // имя объекта отображения 
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
	// сбрасываем весь вид на диск 
	if (!FlushViewOfFile(ptr, 0))
	{
		cerr << "Flush view of file failed." << endl;
		return GetLastError();
	}
	//---------------------------------------------------------------------- 
	// создаем процесс, который будет читать данные из отображаемого 
	// в память файла 
	wchar_t lpszAppName[] = L"D:\\Работы\\Listings30\\Listing30_5\\x64\\Debug\\Listing30_5.exe";
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	// создаем новый консольный процесс 
	if (!CreateProcess(lpszAppName, NULL, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
	{
		cerr << "Create process failed." << endl;
		return GetLastError();
	}
	// ждем завершения созданного прцесса 
	WaitForSingleObject(piApp.hProcess, INFINITE);
	// закрываем дескрипторы этого процесса в текущем процессе 
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	//---------------------------------------------------------------------- 
	// отменяем отображение файла в память 
	if (!UnmapViewOfFile(ptr))
	{
		cerr << "Unmap view of file failed." << endl;
		return GetLastError();
	}
	// закрываем объкт отображения файла в память 
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
	// ждем команду на завершение процесса 
	char c;
	cout << "Input any char to exit: ";
	cin >> c;
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
