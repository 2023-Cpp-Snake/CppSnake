#include "Map.h"
#include "Snake.h"
#include "Wall.h"
#include <ncurses.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

Map::Map(int width, int height, Snake& snake, Wall& wall) :  width(width), height(height), snake(snake), wall(wall) {
}