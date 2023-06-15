#include <ncurses.h>
#include "Snake.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

Snake::Snake(int x, int y) {
    this->x = x;
    this->y = y;
}

int Snake::getX() {
    return x;
}

int Snake::getY() {
    return y;
}

void Snake::setX(int x) {
    this->x = x;
}

void Snake::setY(int y) {
    this->y = y;
}

