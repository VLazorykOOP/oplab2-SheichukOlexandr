#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <mutex>

using namespace std;

const int WIDTH = 100;
const int HEIGHT = 100;
const int NUM_HOUSES = 10;
const int SPEED = 5;
mutex mtx;

struct Point {
    int x;
    int y;
};

class House {
public:
    House(int startX, int startY, int endX, int endY)
        : position{ startX, startY }, endPoint{ endX, endY } {}

    virtual void move() = 0;

    Point getPosition() {
        lock_guard<mutex> lock(mtx);
        return position;
    }

protected:
    Point position;
    Point endPoint;
};

class CapitalHouse : public House {
public:
    CapitalHouse(int startX, int startY, int endX, int endY)
        : House(startX, startY, endX, endY) {}

    void move() override {
        while (position.x != endPoint.x || position.y != endPoint.y) {
            this_thread::sleep_for(chrono::milliseconds(1000 / SPEED));
            lock_guard<mutex> lock(mtx);
            if (position.x < endPoint.x) position.x++;
            if (position.x > endPoint.x) position.x--;
            if (position.y < endPoint.y) position.y++;
            if (position.y > endPoint.y) position.y--;
            cout << "CapitalHouse moving to (" << position.x << ", " << position.y << ")" << endl;
        }
        cout << "CapitalHouse reached destination (" << endPoint.x << ", " << endPoint.y << ")" << endl;
        cout << endl;
    }
};

class WoodenHouse : public House {
public:
    WoodenHouse(int startX, int startY, int endX, int endY)
        : House(startX, startY, endX, endY) {}

    void move() override {
        while (position.x != endPoint.x || position.y != endPoint.y) {
            this_thread::sleep_for(chrono::milliseconds(1000 / SPEED));
            lock_guard<mutex> lock(mtx);
            if (position.x < endPoint.x) position.x++;
            if (position.x > endPoint.x) position.x--;
            if (position.y < endPoint.y) position.y++;
            if (position.y > endPoint.y) position.y--;
            cout << "WoodenHouse moving to (" << position.x << ", " << position.y << ")" << endl;
        }
        cout << "WoodenHouse reached destination (" << endPoint.x << ", " << endPoint.y << ")" << endl;
        cout << endl;
    }
};

int main() {
    srand(time(0));
    vector<thread> threads;

    // Creating Capital Houses
    for (int i = 0; i < NUM_HOUSES; i++) {
        int startX = rand() % (WIDTH / 2);
        int startY = rand() % (HEIGHT / 2);
        int endX = rand() % (WIDTH / 2);
        int endY = rand() % (HEIGHT / 2);
        CapitalHouse* house = new CapitalHouse(startX, startY, endX, endY);
        threads.push_back(thread(&CapitalHouse::move, house));
    }

    // Creating Wooden Houses
    for (int i = 0; i < NUM_HOUSES; i++) {
        int startX = rand() % (WIDTH / 2) + WIDTH / 2;
        int startY = rand() % (HEIGHT / 2) + HEIGHT / 2;
        int endX = rand() % (WIDTH / 2) + WIDTH / 2;
        int endY = rand() % (HEIGHT / 2) + HEIGHT / 2;
        WoodenHouse* house = new WoodenHouse(startX, startY, endX, endY);
        threads.push_back(thread(&WoodenHouse::move, house));
    }

    // Join threads
    for (auto& th : threads) {
        th.join();
    }

    cout << "Simulation complete." << endl;
    return 0;
}
