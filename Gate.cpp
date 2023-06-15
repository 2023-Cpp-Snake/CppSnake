#include "Gate.h"

Gate::Gate(int x, int y, char direction) {
    this->x = x;
    this->y = y;
    this->direction = direction;
}

int Gate::getX() {
    return x;
}

int Gate::getY() {
    return y;
}

void Gate::setX(int x) {
    this->x = x;
}

void Gate::setY(int y) {
    this->y = y;
}

