#include <iostream>
#include <conio.h>
#include <Windows.h>


#include <ctime>

// Объявление переменной для управления цветом в консоли

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

using namespace std;

// Константы для размеров игрового поля
const int WEIGHT = 40;
const int HEIGHT = 20;

// Перечисление для возможных движений змеи

enum moves { STOP = 0, W, S, LEFT, RIGHT };

// Символ, представляющий змею

#define snake '$'

// Функция для проверки, достигла ли змея стенки

bool walls(int X, int Y, int score) {
    if (X >= WEIGHT || Y >= HEIGHT || X < 0 || Y < 0) {

        // Установка цвета текста в консоли (синий)

        SetConsoleTextAttribute(hConsole, 9);

        // Вывод сообщения о завершении игры и текущем счете

        cout << "\t\t\t\t" << "-GAME OVER- " << " Your score: " << score << endl;

        // Восстановление цвета текста в консоли (белый)

        SetConsoleTextAttribute(hConsole, 7);

        return true;
    }
    return false;
}

// Функция для вывода символа фрукта в консоль (красный)
void printFruits() {

    SetConsoleTextAttribute(hConsole, 12);

    cout << '*';

    SetConsoleTextAttribute(hConsole, 7);
}

// Функция для вывода символа хвоста змеи в консоль (зеленый)
void tailprint(int k, int tX[], int tY[]) {

    SetConsoleTextAttribute(hConsole, 2);

    cout << "^";

    SetConsoleTextAttribute(hConsole, 7);
}

// Функция обработки логики игры при съедании фрукта
void Logic(int X, int Y, int& score, int& fX, int& fY, int& tail, int size) {
    if (X == fX && Y == fY) {
        score++;
        if (tail < size) {
            tail++;
        }
        
        do {
            fX = rand() % WEIGHT;
            fY = rand() % HEIGHT;
        } while (fX != X && fY != Y);
    }
}


// Функция отображения игрового поля в консоли
void place(int X, int Y, int fX, int fY, int score, int tX[], int tY[], int tail) {
    // Очистка экрана консоли
    system("cls");

    // Вывод счета игры в красном цвете

    SetConsoleTextAttribute(hConsole, 4);

    cout << "Score: " << score << endl;

    SetConsoleTextAttribute(hConsole, 7);

    // Вывод верхней границы игрового поля
    for (int i = 0; i < WEIGHT + 2; i++) {
        cout << "_";
    }
    cout << "\n";

    // Отображение игрового поля
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WEIGHT; j++) {
            if (j == 0 || j == WEIGHT - 1) {
                cout << "|";
                cout << " ";
            }
            else if (i == Y && j == X) {

                // Отображение головы змеи

                SetConsoleTextAttribute(hConsole, 2);

                cout << snake;

                SetConsoleTextAttribute(hConsole, 7);

            }
            else if (i == fY && j == fX) {

                // Отображение фрукта

                printFruits();
            }

            else {
                // Проверка и отображение хвоста змеи

                bool print = false;
                int l = 0;
                while (l < tail) {
                    if (tX[l] == j && tY[l] == i) {
                        tailprint(l, tX, tY);
                        print = true;
                    }
                    l++;
                }

                // Отображение пустого места, если нет змеи или фрукта

                if (!print) {
                    cout << " ";
                }
            }
        }
        cout << "\n";
    }

    // Вывод нижней границы игрового поля

    for (int i = 0; i < WEIGHT + 2; i++) {
        cout << "-";
    }

    cout << endl;
}

// Функция для обработки действий пользователя (нажатия клавиш)

void Actions(moves& direction) {
    if (_kbhit()) {
        char move = _getch();

        switch (move) {
        case 'w':
            direction = W;
            break;
        case 's':
            direction = S;
            break;
        case 'a':
            direction = LEFT;
            break;
        case 'd':
            direction = RIGHT;
            break;
        }
    }
}

// Функция для обновления координат змеи в зависимости от направления

void Move(int& X, int& Y, moves direction, int tX[], int tY[], int tail) {

    // Сохранение текущих координат головы змеи в массиве хвоста
    tX[tail] = X;
    tY[tail] = Y;

    // Обновление координат хвоста

    for (int i = 0; i < tail; i++) {
        tX[i] = tX[i + 1];
        tY[i] = tY[i + 1];
    }

    // Обновление координат головы змеи в зависимости от направления

    switch (direction) {
    case W:
        Y--;
        break;
    case S:
        Y++;
        break;
    case LEFT:
        X--;
        break;
    case RIGHT:
        X++;
        break;
    }
}

// Основная функция программы

int main() {
    int X, Y, fX, fY, score;
    const int SIZE = 70;
    int tX[SIZE];
    int tY[SIZE];
    int tail = 0;

    moves direction;
    bool gamestart;

    // Инициализация генератора случайных чисел
    srand(time(NULL));

    // Инициализация начальных координат фрукта
    fX = rand() % WEIGHT;
    fY = rand() % HEIGHT;

    // Инициализация начальных координат головы змеи
    X = WEIGHT / 2;
    Y = HEIGHT / 2;

    direction = STOP;
    score = 0;

    gamestart = true;
    int answer;

    // Вывод меню игры
    SetConsoleTextAttribute(hConsole, 2);
    cout << "----------------------------" << endl;
    cout << "\t --Snake-- " << "         | " << endl;
    cout << "----------------------------" << endl;
    SetConsoleTextAttribute(hConsole, 7);
    SetConsoleTextAttribute(hConsole, 5);
    cout << "Welcome to the game named SNAKE " << endl;
    cout << "1) - Start the game " << endl;
    cout << "2) - About the game " << endl;
    cout << "3) - Exit " << endl;
    SetConsoleTextAttribute(hConsole, 7);

    do {
        // Чтение выбора пользователя
        cin >> answer;

        switch (answer) {
        case 1:
            gamestart = true;

            // Цикл игры
            while (gamestart) {
                // Отображение игрового поля
                place(X, Y, fX, fY, score, tX, tY, tail);

                // Обработка действий пользователя
                Actions(direction);

                // Проверка на столкновение со стенками
                if (walls(X, Y, score)) {
                    gamestart = false;
                }

                // Обновление координат змеи
                Move(X, Y, direction, tX, tY, tail);

                // Обработка логики игры при съедании фрукта
                Logic(X, Y, score, fX, fY, tail, SIZE);
            }
            break;

        case 2:
            // Вывод информации о игре
            cout << "This game named snake. You need to press buttons on your keyboard to move the snake." << endl;
            cout << "If you will be behind of walls, snake will away and you will lose!" << endl;
            break;

        case 3:
            // Вывод прощального сообщения
            cout << "Good Luck!";
            break;

        default:
            // Вывод сообщения об ошибке при неверной команде
            cout << "WRONG COMMAND!!! " << endl;
            break;
        }
    } while (answer != 3);
}

