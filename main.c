#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10 // Размер поля

// Типы кораблей и их размеры
#define SHIP_TYPES 4
int ship_sizes[SHIP_TYPES] = {4, 3, 2, 1}; // Линкор, Крейсер, Эсминец, Катер

// Функции
void initializeField(char field[SIZE][SIZE]);
void printField(char field[SIZE][SIZE]);
int isValidPlacement(char field[SIZE][SIZE], int x, int y, int shipSize, int direction);
int checkSurroundings(char field[SIZE][SIZE], int x, int y, int shipSize, int direction);
void PlayerPlaceShip(char field[SIZE][SIZE], int shipSize);
int placeShip(char field[SIZE][SIZE], int shipSize);

// Главная функция
int main() {
    srand(time(NULL));

    char playerField[SIZE][SIZE]; // Поле игрока
    initializeField(playerField);
    char pcField[SIZE][SIZE]; // Поле компьютера
    initializeField(pcField);

    // Игрок размещает корабли
    for (int i = 0; i < SHIP_TYPES; i++) {
        int ships_to_place = 5 - ship_sizes[i]; // Количество кораблей для каждого типа
        for (int j = 0; j < ships_to_place; j++) {
            printField(playerField);
            printf("\n\nРазместите корабль размером %d клеток\n", ship_sizes[i]);
            PlayerPlaceShip(playerField, ship_sizes[i]); // Игрок размещает корабль
        }
    }

    // Компьютер размещает корабли
    for (int i = 0; i < SHIP_TYPES; i++) {
        int ships_to_place = 5 - ship_sizes[i]; // Количество кораблей для каждого типа
        for (int j = 0; j < ships_to_place; j++) {
            while(!placeShip(pcField, ship_sizes[i])){} // Компьютер размещает корабль
        }
    }

    // Выводим поле игрока на экран после размещения всех кораблей
    printf("Ваше поле:\n");
    printField(playerField);

    printf("\n\nПоле противника:\n");
    printField(pcField);

    return 0;
}

// Инициализация поля
void initializeField(char field[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            field[i][j] = '.'; // Пустое поле
        }
    }
}

// Вывод поля на экран
void printField(char field[SIZE][SIZE]) {
    printf("  ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", field[i][j]);
        }
        printf("\n");
    }
}

// Размещение корабля игроком
void PlayerPlaceShip(char field[SIZE][SIZE], int shipSize) {
    int x, y, direction;
    
    while (1) { // Цикл для повторного ввода при неправильном размещении
        printf("Введите координаты для начала корабля (формат: x y): ");
        scanf("%d %d", &x, &y);
        printf("Выберите направление корабля (0 - горизонтально, 1 - вертикально): ");
        scanf("%d", &direction);
        if (direction > 1 || direction < 0) {
            printf("Ошибка: некорректное размещение коробля. Попробуйте снова.\n");
            continue;
        }
        // Проверяем корректность ввода
        if (isValidPlacement(field, x, y, shipSize, direction)) {
            // Размещаем корабль
            for (int i = 0; i < shipSize; i++) {
                if (direction == 0) { // Горизонтальное размещение
                    field[x][y + i] = 'S';
                } else { // Вертикальное размещение
                    field[x + i][y] = 'S';
                }
            }
            break; // Выход из цикла, если корабль размещен успешно
        } else {
            printf("Ошибка: некорректное размещение корабля. Попробуйте снова.\n");
        }
    }
}

// Случайное размещение коробля
int placeShip(char field[SIZE][SIZE], int shipSize) {
    int x = rand() % SIZE;       // Случайная позиция по x
    int y = rand() % SIZE;       // Случайная позиция по y
    int direction = rand() % 2;  // Случайное направление: 0 - горизонтально, 1 - вертикально

    if (isValidPlacement(field, x, y, shipSize, direction)) {
        // Размещаем корабль
        for (int i = 0; i < shipSize; i++) {
            if (direction == 0) { // Горизонтально
                field[x][y + i] = 'S';
            } else {              // Вертикально
                field[x + i][y] = 'S';
            }
        }
        return 1; // Успешное размещение
    }
    return 0; // Не удалось разместить корабль
}

// Проверка возможности размещения корабля и радиуса вокруг него
int isValidPlacement(char field[SIZE][SIZE], int x, int y, int shipSize, int direction) {
    // Проверяем границы поля
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return 0; // Корабль выходит за пределы поля
    if (direction == 0) { // Горизонтально
        if (y + shipSize > SIZE) return 0; // Корабль выходит за пределы поля
    } else { // Вертикально
        if (x + shipSize > SIZE) return 0; // Корабль выходит за пределы поля
    }

    // Проверка на возможность размещения самого корабля и пространства вокруг него
    return checkSurroundings(field, x, y, shipSize, direction);
}

// Проверка клеток вокруг корабля
int checkSurroundings(char field[SIZE][SIZE], int x, int y, int shipSize, int direction) {
    int startX = x - 1, startY = y - 1;
    int endX = x + shipSize, endY = y + shipSize;

    if (direction == 0) { // Горизонтально
        endX = x + 1;
        endY = y + shipSize; // Конец по оси y

        for (int i = startX; i <= endX; i++) {
            for (int j = startY; j <= endY; j++) {
                if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) { // Проверяем только существующие клетки
                    if (field[i][j] != '.') return 0; // Если клетка занята
                }
            }
        }
    } else { // Вертикально
        endX = x + shipSize; // Конец по оси x
        endY = y + 1;

        for (int i = startX; i <= endX; i++) {
            for (int j = startY; j <= endY; j++) {
                if (i >= 0 && i < SIZE && j >= 0 && j < SIZE) { // Проверяем только существующие клетки
                    if (field[i][j] != '.') return 0; // Если клетка занята
                }
            }
        }
    }
    return 1; // Размещение возможно
}
