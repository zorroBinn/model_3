#include <iostream>
#include <vector>
#include <iomanip>
#include <Windows.h>
using namespace std;

/// <summary>
/// Печатает текущую симплекс-таблицу
/// </summary>
/// <param name="table">Симплекс-таблица</param>
/// <param name="basis">Вектор индексов базисных переменных</param>
void printSimplexTable(vector<vector<double>>& table, vector<int>& basis) {
    cout << "Базис\t";
    for (short j = 0; j < table[0].size() - 1; ++j)
        cout << "x" << j + 1 << "\t";
    cout << "Свободный член" << endl;

    for (short i = 0; i < table.size(); ++i) {
        if (i < basis.size())
            cout << "x" << basis[i] + 1 << "\t";
        else
            cout << "F\t";
        for (short j = 0; j < table[i].size(); ++j)
            cout << table[i][j] << "\t";
        cout << endl;
    }
    cout << endl;
}

/// <summary>
/// Находит столбец разрешающего элемента
/// </summary>
/// <param name="row">Строка симплекс-таблицы</param>
/// <returns>Индекс столбца с минимальным значением, либо -1 если решение оптимально</returns>
int getPermElemColumn(vector<double>& row) {
    int permElemCol = -1;
    double minVal = 0.0; //Ищем минимальный отрицательный коэффициент
    for (short j = 0; j < row.size() - 1; ++j) {
        if (row[j] < minVal) {
            minVal = row[j];
            permElemCol = j;
        }
    }
    return permElemCol;
}

/// <summary>
/// Находит строку разрешающего элемента
/// </summary>
/// <param name="table">Симплекс-таблица</param>
/// <param name="permElemCol">Индекс столбца</param>
/// <returns>Индекс строки с минимальным отношением свободного члена к элементу столбца, либо -1 если решения нет</returns>
int getPermElemRow(vector<vector<double>>& table, int permElemCol) {
    int permElemRow = -1;
    double minRatio = numeric_limits<double>::infinity(); //Для сравнения
    for (short i = 0; i < table.size() - 1; ++i) {
        if (table[i][permElemCol] > 0 && table[i].back() > 0 || table[i][permElemCol] < 0 && table[i].back() < 0) {
            double ratio = table[i].back() / table[i][permElemCol];
            if (ratio < minRatio) {
                minRatio = ratio;
                permElemRow = i;
            }
        }
    }
    return permElemRow;
}

/// <summary>
/// Выполняет преобразование симплекс-таблицы 
/// </summary>
/// <param name="table">Симплекс-таблица</param>
/// <param name="basis">Вектор индексов базисных переменных</param>
/// <param name="permElemRow">Индекс строки разрешающего элемента</param>
/// <param name="permElemCol">Индекс столбца разрешающего элемента</param>
void simplexTableTransform(vector<vector<double>>& table, vector<int>& basis, int permElemRow, int permElemCol) {
    double permElem = table[permElemRow][permElemCol]; //Разрешающий элемент
    //Делим всю строку разрешающего элемента на его значение
    for (short j = 0; j < table[permElemRow].size(); ++j) table[permElemRow][j] /= permElem;
    //Преобразуем остальные строки
    for (short i = 0; i < table.size(); ++i) {
        if (i != permElemRow) {
            double factor = table[i][permElemCol];
            for (short j = 0; j < table[i].size(); ++j)
                table[i][j] -= factor * table[permElemRow][j];
        }
    }

    basis[permElemRow] = permElemCol; //Обновляем базис
}

/// <summary>
/// Печатает текущее базисное решение и значение целевой функции
/// </summary>
/// <param name="table">Симплекс-таблица</param>
/// <param name="basis">Вектор индексов базисных переменных</param>
void printCurrentSolution(vector<vector<double>>& table, vector<int>& basis) {
    vector<double> solution(table[0].size() - 1, 0.0);
    for (short i = 0; i < basis.size(); ++i)
        solution[basis[i]] = table[i].back();

    cout << "Текущее базисное решение: X = {";
    for (short i = 0; i < solution.size(); ++i) {
        if (i > 0) cout << ", ";
        cout << solution[i];
    }
    cout << "}" << endl;
    cout << "Текущее значение F = " << table.back().back() << endl << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    //Исходная симплекс-таблица (коэффициенты ограничений и целевой функции)
    vector<vector<double>> table = {
        {3, 1, 4, 4, 1, 0, 0, 100}, //Ограничение 1
        {4, 2, 5, 5, 0, 1, 0, 100}, //Ограничение 2
        {5, 5, 4, 0, 0, 0, 1, 100}, //Ограничение 3
        {-2.5, -2, -5, -3, 0, 0, 0, 0} //Целевая функция
    };

    vector<int> basis = { 4, 5, 6 }; //Индексы базисных переменных (x5, x6, x7)

    cout << "Начальная симплекс-таблица: " << endl;
    printSimplexTable(table, basis);

    short step = 0; //Номер текущего шага

    while (true) {
        int permElemCol = getPermElemColumn(table.back());
        if (permElemCol == -1) { //Если все коэффициенты в строке F >= 0
            cout << "Оптимальное решение найдено" << endl;
            break;
        }
        
        int permElemRow = getPermElemRow(table, permElemCol);
        if (permElemRow == -1) { //Если нет положительных элементов в столбце permElemCol
            cout << "Решения нет" << endl;
            return 1;
        }

        cout << "Шаг " << ++step << ": " << endl;

        cout << "Переменная x" << permElemCol + 1 << " входит в базис, ";
        cout << "переменная x" << basis[permElemRow] + 1 << " выходит из базиса" << endl;
        cout << "Разрешающий элемент: строка " << permElemRow + 1 << ", столбец " << permElemCol + 1 << "." << endl;

        simplexTableTransform(table, basis, permElemRow, permElemCol);
        printSimplexTable(table, basis);
        printCurrentSolution(table, basis);
    }
    //Вывод оптимального решения
    vector<double> solution(table[0].size() - 1, 0.0);
    for (short i = 0; i < basis.size(); ++i)
        solution[basis[i]] = table[i].back();

    cout << "Оптимальное базисное решение: X = {";
    for (short i = 0; i < solution.size(); ++i) {
        if (i > 0) cout << ", ";
        cout << solution[i];
    }
    cout << "}" << endl;
    cout << "F = " << table.back().back() << endl;

    return 0;
}
