// Listing30_2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <windows.h> 
#include <fstream> 
#include <iostream>
using namespace std;
int main()
{
	wchar_t MappingName[] = L"MappingName";
	HANDLE hMapping; // дескриптор объекта, отображающего файл 
	int* ptr; // для указателя на массив 
	const int n = 10; // размерность массива 
	cout << "This is a parent process." << endl;
	// открываем объект отображения файла в память 
	hMapping = CreateFileMapping(
		INVALID_HANDLE_VALUE, // файл подкачки страниц 
		NULL, // атрибуты защиты по умолчанию 
		PAGE_READWRITE, // режим доступа: чтение и запись 
		0, // старшее слово = 0 
		n * sizeof(int), // младшее слово = длине массива 
		MappingName); // имя объекта отображения 
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
	// инициализируем массив и выводим его на консоль 
	cout << "Array: ";
	for (int i = 0; i < n; ++i)
	{
		ptr[i] = i;
		cout << ptr[i] << ' ';
	}
	cout << endl;
	//---------------------------------------------------------------------- 
	// создаем процесс, который будет читать данные из отображаемого 
	// в память файла 
	wchar_t lpszAppName[] = L"D:\\Работы\\Listings30\\Listing30_3\\x64\\Debug\\Listing30_3.exe";
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
	// закрываем объект отображения файла в память 
	if (!CloseHandle(hMapping))
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
