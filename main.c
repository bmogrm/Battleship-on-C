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

int hasShips(char field[SIZE][SIZE]);
void computerMove(char playerField[SIZE][SIZE], char computerBattleField[SIZE][SIZE]);
int isShipDestroyed(char field[SIZE][SIZE], int x, int y);
void markSurroundings(char field[SIZE][SIZE], int x, int y);



// Главная функция
int main() {
    srand(time(NULL));
    char playerField[SIZE][SIZE], pcField[SIZE][SIZE];
    char playerBattleField[SIZE][SIZE], computerBattleField[SIZE][SIZE];

    initializeField(playerField);
    initializeField(pcField);
    initializeField(playerBattleField);   // Поле игрока для ведения боя (то, что видит игрок)
    initializeField(computerBattleField); // Поле компьютера для ведения боя

    // Игрок и компьютер размещают корабли
    for (int i = 0; i < SHIP_TYPES; i++) {
        int ships_to_place = 5 - ship_sizes[i]; // Количество кораблей для каждого типа
        for (int j = 0; j < ships_to_place; j++) {
            while(!placeShip(pcField, ship_sizes[i])){} // Компьютер размещает корабли
            // Игрок размещает корабли
            printField(playerField);
            printf("\n\nРазместите корабль размером %d клеток\n", ship_sizes[i]);
            PlayerPlaceShip(playerField, ship_sizes[i]);
        }
    }

    printField(pcField);

    // Начало битвы
    while (hasShips(playerField) && hasShips(pcField)) {
        int x, y;

        // Ход игрока
        printf("Ваше поле для атаки:\n");
        printField(playerBattleField);
        
        while (1) { // Цикл для повторного ввода при некорректных координатах
            printf("Введите координаты атаки (формат: x y): ");
            scanf("%d %d", &x, &y);

            if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
                printf("Некорректные координаты, попробуйте снова.\n");
                continue; // Некорректный ввод
            }
            
            if (playerBattleField[x][y] != '.') {
                printf("Вы уже стреляли в эту клетку, попробуйте снова.\n");
                continue; // Клетка уже атакована
            }

            // Проверка попадания
            if (pcField[x][y] == 'S') {
                printf("Попал!\n");
                playerBattleField[x][y] = 'X'; // Пометить попадание
                pcField[x][y] = 'X'; // Пометить корабль уничтоженным
                
                // Проверяем, уничтожен ли корабль
                if (isShipDestroyed(pcField, x, y)) {
                    printf("Корабль уничтожен!\n");
                    markSurroundings(playerBattleField, x, y); // Отмечаем клетки вокруг всего корабля
                }
            } else {
                printf("Мимо!\n");
                playerBattleField[x][y] = '*'; // Пометить промах
            }
            break; // Завершаем ход игрока
        }

        // Проверяем, остались ли корабли у компьютера
        if (!hasShips(pcField)) {
            printf("Поздравляем! Вы победили!\n");
            break;
        }

        // Ход компьютера
        computerMove(playerField, computerBattleField);

        // Проверяем, остались ли корабли у игрока
        if (!hasShips(playerField)) {
            printf("Компьютер победил! Игра окончена.\n");
            break;
        }
    }

    return 0;
}



int isShipDestroyed(char field[SIZE][SIZE], int x, int y) {
    // Проверяем весь корабль — вертикальный или горизонтальный
    // Идем вправо и влево от точки попадания
    int i = x, j = y;
    
    // Проверка по горизонтали (вправо и влево)
    while (j >= 0 && field[i][j] == 'X') j--; // Идем влево, пока корабль
    j++; // Возвращаемся к последнему 'X'
    while (j < SIZE && (field[i][j] == 'X' || field[i][j] == 'S')) {
        if (field[i][j] == 'S') return 0; // Если есть части корабля, которые не уничтожены
        j++;
    }
    
    // Проверка по вертикали (вверх и вниз)
    i = x; j = y;
    while (i >= 0 && field[i][j] == 'X') i--; // Идем вверх
    i++;
    while (i < SIZE && (field[i][j] == 'X' || field[i][j] == 'S')) {
        if (field[i][j] == 'S') return 0; // Если есть не уничтоженные части
        i++;
    }

    return 1; // Корабль уничтожен полностью
}

void markSurroundings(char field[SIZE][SIZE], int x, int y) {
    int startX = x, startY = y;
    int endX = x, endY = y;

    // Находим границы корабля по горизонтали
    while (startY > 0 && field[x][startY - 1] == 'X') {
        startY--;
    }
    while (endY < SIZE - 1 && field[x][endY + 1] == 'X') {
        endY++;
    }

    // Находим границы корабля по вертикали
    while (startX > 0 && field[startX - 1][y] == 'X') {
        startX--;
    }
    while (endX < SIZE - 1 && field[endX + 1][y] == 'X') {
        endX++;
    }

    // Отмечаем все клетки вокруг корабля
    for (int i = startX - 1; i <= endX + 1; i++) {
        for (int j = startY - 1; j <= endY + 1; j++) {
            if (i >= 0 && i < SIZE && j >= 0 && j < SIZE && field[i][j] == '.') {
                field[i][j] = 'X'; // Помечаем как "мимо"
            }
        }
    }
}



// Проверка на наличие живых кораблей
int hasShips(char field[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (field[i][j] == 'S') {
                return 1; // Есть хотя бы один корабль
            }
        }
    }
    return 0; // Все корабли уничтожены
}

// Случайный ход компьютера
void computerMove(char playerField[SIZE][SIZE], char computerBattleField[SIZE][SIZE]) {
    int x, y;
    while (1) {
        x = rand() % SIZE;
        y = rand() % SIZE;

        // Проверяем, не стреляли ли в эту клетку раньше
        if (computerBattleField[x][y] == '.') {
            if (playerField[x][y] == 'S') {
                printf("Компьютер попал в ваш корабль!\n");
                computerBattleField[x][y] = 'X'; // Попадание
                playerField[x][y] = 'X'; // Пометить корабль уничтоженным
            } else {
                printf("Компьютер промахнулся.\n");
                computerBattleField[x][y] = '*'; // Промах
            }
            break;
        }
    }
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
