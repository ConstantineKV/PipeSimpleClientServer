// NamedPipesClient.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>
#define PIPE_TIMEOUT 5000
#define BUFSIZE 512

OVERLAPPED overlap;
HANDLE hPipe;
HANDLE hEvent;
TCHAR chRequest[BUFSIZE];
TCHAR toWrite[BUFSIZE];
DWORD cbRead, NumbertoWrite, cbWritten, dwMode;

using namespace std;

int main()
{
    LPTSTR Pipename = (LPTSTR)L"\\\\.\\pipe\\Konstantine";
    LPTSTR Message = (LPTSTR)L"Default message from client";
    hPipe = CreateFile(
        Pipename,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        printf("Invalid handle");
        getchar();
        return -1;
    }

    if (GetLastError() == ERROR_PIPE_BUSY)
    {
        printf("Could not open pipe GLE=%d", GetLastError());
        getchar();
        return -1;
    }
    bool fSuccess;
    dwMode = PIPE_READMODE_MESSAGE;
    fSuccess = SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL);
    if (!fSuccess)
    {
        cout<<"SetNamedPipeHandleState failed"<<endl;
        getchar();
        return -1;
    }

    DWORD NumberToWrite = (lstrlen(Message) + 1) * sizeof(TCHAR);

    fSuccess = WriteFile(hPipe, Message, NumberToWrite, &cbWritten, NULL);
    if (!fSuccess)
    {
        cout << "Write to pipe field" << endl;
        getchar();
        return -1;
    }
    printf("\nMessage sent to server, receiving reply as follows:\n");

    fSuccess = ReadFile(hPipe, chRequest, BUFSIZE, &cbRead, NULL);
    if (!fSuccess)
    {
        cout << "Read from pipe field" << endl;
        getchar();
        return -1;
    }
    _tprintf(TEXT("%s\n"), chRequest);
    printf("\n<End of message, press ENTER to terminate connection and exit>");
    getchar();
    CloseHandle(hPipe);
   
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
