#include <iostream>       // Підключаємо бібліотеку для введення-виведення
#include <thread>         // Підключаємо бібліотеку для роботи з потоками
#include <vector>         // Підключаємо бібліотеку для роботи з вектором
#include <cstdlib>        // Підключаємо бібліотеку для генерації випадкових чисел
#include <ctime>          // Підключаємо бібліотеку для роботи з часом
#include <chrono>         // Підключаємо бібліотеку для роботи з часом і тривалістю
#include <mutex>          // Підключаємо бібліотеку для роботи з м'ютексами (блокуваннями)

using namespace std;      // Дозволяє уникнути постійного використання префіксу std::

const int WIDTH = 100;    // Ширина області симуляції
const int HEIGHT = 100;   // Висота області симуляції
const int NUM_HOUSES = 10;// Кількість будинків кожного типу
const int SPEED = 1;      // Швидкість руху будинків
mutex mtx;                // Об'єкт м'ютексу для синхронізації доступу до ресурсів

struct Point {            // Структура для представлення точки в 2D-просторі
    int x;                // Координата X
    int y;                // Координата Y
};

class House {             // Базовий клас для будинків
public:
    House(int startX, int startY, int endX, int endY) // Конструктор класу House
        : position{ startX, startY }, endPoint{ endX, endY } {} // Ініціалізація початкової та кінцевої точок

    virtual void move() = 0;  // Віртуальний метод move, який буде реалізовано в підкласах

    Point getPosition() {    // Метод для отримання поточної позиції будинку
        lock_guard<mutex> lock(mtx); // Захист доступу до позиції за допомогою м'ютексу
        return position;     // Повернення поточної позиції
    }

protected:
    Point position;          // Поточна позиція будинку
    Point endPoint;          // Кінцева точка руху будинку
};

class CapitalHouse : public House { // Клас для капітальних будинків, наслідує клас House
public:
    CapitalHouse(int startX, int startY, int endX, int endY) // Конструктор класу CapitalHouse
        : House(startX, startY, endX, endY) {} // Виклик конструктора базового класу House

    void move() override { // Реалізація віртуального методу move
        while (position.x != endPoint.x || position.y != endPoint.y) { // Поки будинок не досягне кінцевої точки
            this_thread::sleep_for(chrono::milliseconds(1000 / SPEED)); // Затримка для імітації швидкості руху
            lock_guard<mutex> lock(mtx); // Захист доступу до позиції за допомогою м'ютексу
            if (position.x < endPoint.x) position.x++; // Рух по осі X до кінцевої точки
            if (position.x > endPoint.x) position.x--;
            if (position.y < endPoint.y) position.y++; // Рух по осі Y до кінцевої точки
            if (position.y > endPoint.y) position.y--;
            cout << "CapitalHouse moving to (" << position.x << ", " << position.y << ")" << endl; // Вивід поточної позиції
        }
        cout << "CapitalHouse reached destination (" << endPoint.x << ", " << endPoint.y << ")" << endl; // Повідомлення про досягнення кінцевої точки
    }
};

class WoodenHouse : public House { // Клас для дерев'яних будинків, наслідує клас House
public:
    WoodenHouse(int startX, int startY, int endX, int endY) // Конструктор класу WoodenHouse
        : House(startX, startY, endX, endY) {} // Виклик конструктора базового класу House

    void move() override { // Реалізація віртуального методу move
        while (position.x != endPoint.x || position.y != endPoint.y) { // Поки будинок не досягне кінцевої точки
            this_thread::sleep_for(chrono::milliseconds(1000 / SPEED)); // Затримка для імітації швидкості руху
            lock_guard<mutex> lock(mtx); // Захист доступу до позиції за допомогою м'ютексу
            if (position.x < endPoint.x) position.x++; // Рух по осі X до кінцевої точки
            if (position.x > endPoint.x) position.x--;
            if (position.y < endPoint.y) position.y++; // Рух по осі Y до кінцевої точки
            if (position.y > endPoint.y) position.y--;
            cout << "WoodenHouse moving to (" << position.x << ", " << position.y << ")" << endl; // Вивід поточної позиції
        }
        cout << "WoodenHouse reached destination (" << endPoint.x << ", " << endPoint.y << ")" << endl; // Повідомлення про досягнення кінцевої точки
    }
};

int main() {
    srand(time(0)); // Ініціалізація генератора випадкових чисел
    vector<thread> threads; // Вектор для зберігання потоків

    // Створення потоків для капітальних будинків
    for (int i = 0; i < NUM_HOUSES; i++) {
        int startX = rand() % (WIDTH / 2); // Випадкова початкова координата X в лівій верхній чверті
        int startY = rand() % (HEIGHT / 2); // Випадкова початкова координата Y в лівій верхній чверті
        int endX = rand() % (WIDTH / 2); // Випадкова кінцева координата X в лівій верхній чверті
        int endY = rand() % (HEIGHT / 2); // Випадкова кінцева координата Y в лівій верхній чверті
        CapitalHouse* house = new CapitalHouse(startX, startY, endX, endY); // Створення об'єкта CapitalHouse
        threads.push_back(thread(&CapitalHouse::move, house)); // Створення і запуск потоку для об'єкта CapitalHouse
    }

    // Створення потоків для дерев'яних будинків
    for (int i = 0; i < NUM_HOUSES; i++) {
        int startX = rand() % (WIDTH / 2) + WIDTH / 2; // Випадкова початкова координата X в нижній правій чверті
        int startY = rand() % (HEIGHT / 2) + HEIGHT / 2; // Випадкова початкова координата Y в нижній правій чверті
        int endX = rand() % (WIDTH / 2) + WIDTH / 2; // Випадкова кінцева координата X в нижній правій чверті
        int endY = rand() % (HEIGHT / 2) + HEIGHT / 2; // Випадкова кінцева координата Y в нижній правій чверті
        WoodenHouse* house = new WoodenHouse(startX, startY, endX, endY); // Створення об'єкта WoodenHouse
        threads.push_back(thread(&WoodenHouse::move, house)); // Створення і запуск потоку для об'єкта WoodenHouse
    }

    // Приєднання потоків до основного потоку
    for (auto& th : threads) {
        th.join(); // Чекаємо завершення роботи потоку
    }

    cout << "Simulation complete." << endl; // Повідомлення про завершення симуляції
    return 0; // Завершення програми
}
