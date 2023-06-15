#include "Wall.h"

Wall::Wall(char direction) {
    this->direction = direction;
}

ImmuneWall::ImmuneWall(char direction) : Wall(direction) {
    this->direction = direction;
}