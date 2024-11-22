#include <iostream>
#include <vector>
#include <iomanip>
#include <Windows.h>
using namespace std;

/// <summary>
/// �������� ������� ��������-�������
/// </summary>
/// <param name="table">��������-�������</param>
/// <param name="basis">������ �������� �������� ����������</param>
void printSimplexTable(vector<vector<double>>& table, vector<int>& basis) {
    cout << "�����\t";
    for (short j = 0; j < table[0].size() - 1; ++j)
        cout << "x" << j + 1 << "\t";
    cout << "��������� ����" << endl;

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
/// ������� ������� ������������ ��������
/// </summary>
/// <param name="row">������ ��������-�������</param>
/// <returns>������ ������� � ����������� ���������, ���� -1 ���� ������� ����������</returns>
int getPermElemColumn(vector<double>& row) {
    int permElemCol = -1;
    double minVal = 0.0; //���� ����������� ������������� �����������
    for (short j = 0; j < row.size() - 1; ++j) {
        if (row[j] < minVal) {
            minVal = row[j];
            permElemCol = j;
        }
    }
    return permElemCol;
}

/// <summary>
/// ������� ������ ������������ ��������
/// </summary>
/// <param name="table">��������-�������</param>
/// <param name="permElemCol">������ �������</param>
/// <returns>������ ������ � ����������� ���������� ���������� ����� � �������� �������, ���� -1 ���� ������� ���</returns>
int getPermElemRow(vector<vector<double>>& table, int permElemCol) {
    int permElemRow = -1;
    double minRatio = numeric_limits<double>::infinity(); //��� ���������
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
/// ��������� �������������� ��������-������� 
/// </summary>
/// <param name="table">��������-�������</param>
/// <param name="basis">������ �������� �������� ����������</param>
/// <param name="permElemRow">������ ������ ������������ ��������</param>
/// <param name="permElemCol">������ ������� ������������ ��������</param>
void simplexTableTransform(vector<vector<double>>& table, vector<int>& basis, int permElemRow, int permElemCol) {
    double permElem = table[permElemRow][permElemCol]; //����������� �������
    //����� ��� ������ ������������ �������� �� ��� ��������
    for (short j = 0; j < table[permElemRow].size(); ++j) table[permElemRow][j] /= permElem;
    //����������� ��������� ������
    for (short i = 0; i < table.size(); ++i) {
        if (i != permElemRow) {
            double factor = table[i][permElemCol];
            for (short j = 0; j < table[i].size(); ++j)
                table[i][j] -= factor * table[permElemRow][j];
        }
    }

    basis[permElemRow] = permElemCol; //��������� �����
}

/// <summary>
/// �������� ������� �������� ������� � �������� ������� �������
/// </summary>
/// <param name="table">��������-�������</param>
/// <param name="basis">������ �������� �������� ����������</param>
void printCurrentSolution(vector<vector<double>>& table, vector<int>& basis) {
    vector<double> solution(table[0].size() - 1, 0.0);
    for (short i = 0; i < basis.size(); ++i)
        solution[basis[i]] = table[i].back();

    cout << "������� �������� �������: X = {";
    for (short i = 0; i < solution.size(); ++i) {
        if (i > 0) cout << ", ";
        cout << solution[i];
    }
    cout << "}" << endl;
    cout << "������� �������� F = " << table.back().back() << endl << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    //�������� ��������-������� (������������ ����������� � ������� �������)
    vector<vector<double>> table = {
        {3, 1, 4, 4, 1, 0, 0, 100}, //����������� 1
        {4, 2, 5, 5, 0, 1, 0, 100}, //����������� 2
        {5, 5, 4, 0, 0, 0, 1, 100}, //����������� 3
        {-2.5, -2, -5, -3, 0, 0, 0, 0} //������� �������
    };

    vector<int> basis = { 4, 5, 6 }; //������� �������� ���������� (x5, x6, x7)

    cout << "��������� ��������-�������: " << endl;
    printSimplexTable(table, basis);

    short step = 0; //����� �������� ����

    while (true) {
        int permElemCol = getPermElemColumn(table.back());
        if (permElemCol == -1) { //���� ��� ������������ � ������ F >= 0
            cout << "����������� ������� �������" << endl;
            break;
        }
        
        int permElemRow = getPermElemRow(table, permElemCol);
        if (permElemRow == -1) { //���� ��� ������������� ��������� � ������� permElemCol
            cout << "������� ���" << endl;
            return 1;
        }

        cout << "��� " << ++step << ": " << endl;

        cout << "���������� x" << permElemCol + 1 << " ������ � �����, ";
        cout << "���������� x" << basis[permElemRow] + 1 << " ������� �� ������" << endl;
        cout << "����������� �������: ������ " << permElemRow + 1 << ", ������� " << permElemCol + 1 << "." << endl;

        simplexTableTransform(table, basis, permElemRow, permElemCol);
        printSimplexTable(table, basis);
        printCurrentSolution(table, basis);
    }
    //����� ������������ �������
    vector<double> solution(table[0].size() - 1, 0.0);
    for (short i = 0; i < basis.size(); ++i)
        solution[basis[i]] = table[i].back();

    cout << "����������� �������� �������: X = {";
    for (short i = 0; i < solution.size(); ++i) {
        if (i > 0) cout << ", ";
        cout << solution[i];
    }
    cout << "}" << endl;
    cout << "F = " << table.back().back() << endl;

    return 0;
}
