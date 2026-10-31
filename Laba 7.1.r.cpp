#include <iostream>
#include <iomanip>
#include <time.h>

using namespace std;

void CreateRows(int** a, const int rowCount, const int colCount, const int Low, const int High, int rowNo);
void CreateRow(int** a, const int rowNo, const int colCount, const int Low, const int High, int colNo);
void PrintRows(int** a, const int rowCount, const int colCount, int rowNo);
void PrintRow(int** a, const int rowNo, const int colCount, int colNo);
void Sort(int** a, const int rowCount, const int colCount, int pass);
void SortPass(int** a, const int rowCount, const int colCount, int col, const int last);
void Change(int** a, const int col1, const int col2, const int rowCount, int rowNo);
void CalcRows(int** a, const int rowCount, const int colCount, int rowNo, int& S, int& k);
void CalcRow(int** a, const int rowNo, const int colCount, int colNo, int& S, int& k);

int main()
{
    srand((unsigned)time(NULL));    // ініціалізація генератора випадкових чисел

    int Low = -6;                   // нижня межа діапазону значень елементів матриці
    int High = 31;                  // верхня межа діапазону значень елементів матриці
    int rowCount = 7;               // кількість рядків матриці
    int colCount = 5;               // кількість стовпчиків матриці

    int** a = new int* [rowCount];   // створили динамічну матрицю: масив вказівників на рядки
    for (int i = 0; i < rowCount; i++)
        a[i] = new int[colCount];

    CreateRows(a, rowCount, colCount, Low, High, 0);
    cout << "Matrix:" << endl;
    PrintRows(a, rowCount, colCount, 0);

    Sort(a, rowCount, colCount, 0);
    cout << "Sorted matrix:" << endl;
    PrintRows(a, rowCount, colCount, 0);

    int S = 0;                      // сума елементів, які задовольняють критерію
    int k = 0;                      // кількість елементів, які задовольняють критерію
    CalcRows(a, rowCount, colCount, 0, S, k);
    cout << "S = " << S << endl;
    cout << "k = " << k << endl;
    cout << "Modified matrix:" << endl;
    PrintRows(a, rowCount, colCount, 0);

    for (int i = 0; i < rowCount; i++)
        delete[] a[i];              // знищили рядки матриці
    delete[] a;                     // знищили масив вказівників на рядки

    return 0;
}

// формування рядків матриці випадковими числами з [Low; High]; rowNo - номер поточного рядка
void CreateRows(int** a, const int rowCount, const int colCount, const int Low, const int High, int rowNo)
{
    CreateRow(a, rowNo, colCount, Low, High, 0);
    if (rowNo < rowCount - 1)
        CreateRows(a, rowCount, colCount, Low, High, rowNo + 1);
}

// формування поточного рядка матриці; colNo - номер поточного стовпчика
void CreateRow(int** a, const int rowNo, const int colCount, const int Low, const int High, int colNo)
{
    a[rowNo][colNo] = Low + rand() % (High - Low + 1);
    if (colNo < colCount - 1)
        CreateRow(a, rowNo, colCount, Low, High, colNo + 1);
}

// виведення рядків матриці у вигляді таблиці; rowNo - номер поточного рядка
void PrintRows(int** a, const int rowCount, const int colCount, int rowNo)
{
    PrintRow(a, rowNo, colCount, 0);
    if (rowNo < rowCount - 1)
        PrintRows(a, rowCount, colCount, rowNo + 1);
    else
        cout << endl;
}

// виведення поточного рядка матриці; colNo - номер поточного стовпчика
void PrintRow(int** a, const int rowNo, const int colCount, int colNo)
{
    cout << setw(4) << a[rowNo][colNo];
    if (colNo < colCount - 1)
        PrintRow(a, rowNo, colCount, colNo + 1);
    else
        cout << endl;
}

// впорядкування стовпчиків матриці методом обміну ("бульбашки"); pass - номер поточного проходу:
// елементи 1-го рядка - за зростанням;
// при однакових елементах 1-го рядка - елементи 2-го рядка за зростанням;
// при однакових елементах 1-го і 2-го рядків - елементи 3-го рядка за спаданням
void Sort(int** a, const int rowCount, const int colCount, int pass)
{
    SortPass(a, rowCount, colCount, 0, colCount - pass - 1);
    if (pass < colCount - 2)
        Sort(a, rowCount, colCount, pass + 1);
}

// один прохід методу обміну: порівнює сусідні стовпчики col та col+1 до стовпчика last
void SortPass(int** a, const int rowCount, const int colCount, int col, const int last)
{
    if (col < last)
    {
        if ((a[0][col] > a[0][col + 1])
            ||
            (a[0][col] == a[0][col + 1] &&
                a[1][col] > a[1][col + 1])
            ||
            (a[0][col] == a[0][col + 1] &&
                a[1][col] == a[1][col + 1] &&
                a[2][col] < a[2][col + 1]))
            Change(a, col, col + 1, rowCount, 0);
        SortPass(a, rowCount, colCount, col + 1, last);
    }
}

// обмін місцями двох стовпчиків матриці з номерами col1 та col2; rowNo - номер поточного рядка
void Change(int** a, const int col1, const int col2, const int rowCount, int rowNo)
{
    int tmp = a[rowNo][col1];
    a[rowNo][col1] = a[rowNo][col2];
    a[rowNo][col2] = tmp;
    if (rowNo < rowCount - 1)
        Change(a, col1, col2, rowCount, rowNo + 1);
}

// обчислення суми S та кількості k елементів, які задовольняють критерію, у рядках матриці
// (додатні, крім тих, хоча б один індекс яких кратний 3), та заміна цих елементів нулями;
// початкові значення S та k задає викликаюча функція; rowNo - номер поточного рядка
void CalcRows(int** a, const int rowCount, const int colCount, int rowNo, int& S, int& k)
{
    CalcRow(a, rowNo, colCount, 0, S, k);
    if (rowNo < rowCount - 1)
        CalcRows(a, rowCount, colCount, rowNo + 1, S, k);
}

// те саме для поточного рядка матриці; colNo - номер поточного стовпчика
void CalcRow(int** a, const int rowNo, const int colCount, int colNo, int& S, int& k)
{
    if (a[rowNo][colNo] > 0 && !(rowNo % 3 == 0 || colNo % 3 == 0))
    {
        S += a[rowNo][colNo];
        k++;
        a[rowNo][colNo] = 0;
    }
    if (colNo < colCount - 1)
        CalcRow(a, rowNo, colCount, colNo + 1, S, k);
}