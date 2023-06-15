#include <ncurses.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Snake {
    private:
    int x, y;

    public:
    Snake(int x, int y) {
        this->x = x;
        this->y = y;
    }

    //getters, setters
    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    void setX(int x) {
        this->x = x;
    }

    void setY(int y) {
        this->y = y;
    }
};