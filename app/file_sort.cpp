#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <clocale>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

#include "sort.h"

static void setup_console() {
    std::setlocale(LC_ALL, "");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
#endif
}

int main() {
    setup_console();

    std::ifstream in("input_sort.txt");
    if (!in) {
        std::wcerr << L"Ошибка: не удалось открыть input_sort.txt\n";
        std::wcin.get();
        return 1;
    }

    std::vector<int> v;
    int x;
    while (in >> x) v.push_back(x);

    if (v.empty()) {
        std::wcerr << L"Ошибка: файл пуст или содержит неверные данные\n";
        std::wcin.get();
        return 2;
    }

    sort(v.data(), v.data() + v.size(), [](int a, int b) { return a < b; });

    std::ofstream out("output_sort.txt", std::ios::trunc);
    if (!out) {
        std::wcerr << L"Ошибка: не удалось создать output_sort.txt\n";
        std::wcin.get();
        return 3;
    }

    for (int val : v) out << val << "\n";

    std::wcout << L"Сортировка завершена\n";
    std::wcout << L"Элементов: " << v.size() << L"\n";
    std::wcout << L"Результат записан в output_sort.txt\n";

    std::wcout << L"\nНажмите Enter для выхода...";
    std::wcin.get();
    return 0;
}
