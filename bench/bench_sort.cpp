#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include <iomanip>
#include <random>
#include <algorithm>
#include <fstream>
#include <string>
#include <clocale>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

#include "sort.h"

using SteadyClock = std::chrono::steady_clock;

static void setup_console() {
    std::setlocale(LC_ALL, "");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);
#endif
}

template <typename F>
static double mean10(F f) {
    std::vector<double> t(10);
    for (int i = 0; i < 10; ++i) {
        auto a = SteadyClock::now();
        f();
        auto b = SteadyClock::now();
        t[i] = std::chrono::duration<double, std::milli>(b - a).count();
    }
    return std::accumulate(t.begin(), t.end(), 0.0) / t.size();
}

static bool read_numbers(const std::wstring& path, std::vector<int>& out) {
    std::ifstream in(path);
    if (!in) return false;
    out.clear();
    int x;
    while (in >> x) out.push_back(x);
    return !out.empty();
}

int main() {
    setup_console();

    std::vector<int> data;
    if (read_numbers(L"input_sort.txt", data)) {
        std::wcout << L"Режим: замер по данным из файла input_sort.txt\n";
        std::wcout << L"Размер массива: " << data.size() << L"\n\n";

        double t_ins = mean10([&] {
            auto v = data;
            insertion_sort(v.data(), v.data() + v.size(),
                [](int a, int b) { return a < b; });
            });

        double t_qs = mean10([&] {
            auto v = data;
            quick_sort_no_cutoff(v.data(), v.data() + v.size(),
                [](int a, int b) { return a < b; });
            });

        std::wcout << L"Сортировка вставками (мс): " << t_ins << L"\n";
        std::wcout << L"Быстрая сортировка (мс):  " << t_qs << L"\n";
        std::wcout << L"Отношение quick/insert:  " << (t_qs / t_ins) << L"\n";

        std::wcout << L"\nНажмите Enter для выхода...";
        std::wcin.get();
        return 0;
    }

    std::wcout << L"Файл input_sort.txt не найден.\n";
    std::wcout << L"Положите файл рядом с bench_sort.exe\n";
    std::wcout << L"\nНажмите Enter для выхода...";
    std::wcin.get();
    return 0;
}
