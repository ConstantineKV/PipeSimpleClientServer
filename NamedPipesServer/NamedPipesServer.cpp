// NamedPipesServer.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <iostream>
#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096

OVERLAPPED overlap;
HANDLE hPipe;
HANDLE hEvent;
TCHAR chRequest[BUFSIZE];
TCHAR toWrite[BUFSIZE];
LPTSTR toWriteMessage;
using namespace std;
int main()
{   
  
    LPTSTR Pipename = (LPTSTR)L"\\\\.\\pipe\\Konstantine";   //создаем имя для именованого канала
    hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);           //создаем событие
    if (hEvent == NULL)
    {
        printf("CreateEvent failed with %d.\n", GetLastError());
        return 0;
    }
    overlap.hEvent = hEvent;
    hPipe = CreateNamedPipe(Pipename,                       //создаем именованый канал
        PIPE_ACCESS_DUPLEX |
        FILE_FLAG_OVERLAPPED,
        PIPE_TYPE_MESSAGE |
        PIPE_READMODE_MESSAGE |
        PIPE_WAIT,
        1,
        BUFSIZE,
        BUFSIZE,
        PIPE_TIMEOUT,
        NULL);
    cout << "Name pipe has been created" << endl;
    if (hPipe == INVALID_HANDLE_VALUE)
    {
        cout << "CreateNamedPipe failed with " << GetLastError() << endl;
        return 0;
    }
    bool fConnected;
    bool fSuccess;
    fConnected = ConnectNamedPipe(hPipe, &overlap);         //подсоединяемся к именованому каналу

    if (fConnected)
    {
        printf("ConnectNamedPipe failed with %d.\n", GetLastError());
        return 0;
    }
    cout << "Connect to named pipe has been approach" << endl;
    WaitForSingleObject(hEvent, 10000);                   //ждем запроса от клиента
    cout << "Got Event" << endl;
    DWORD NumberOfBytes;
    GetOverlappedResult(hPipe, &overlap, &NumberOfBytes, false);    //получаем данные о запросе
    cout << "Got request from client" << endl;
    ReadFile(hPipe, chRequest, BUFSIZE*sizeof(TCHAR), &NumberOfBytes, &overlap);  //считывание полученных данных

    _tprintf(TEXT("%s"), chRequest);
    toWriteMessage = (LPTSTR)L"Default message from server";
    DWORD toWriteSize = (lstrlen(toWriteMessage) + 1) * sizeof(TCHAR);
    WriteFile(hPipe, toWriteMessage, toWriteSize, &NumberOfBytes, &overlap);
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
