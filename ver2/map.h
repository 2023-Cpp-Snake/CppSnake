#pragma once
#include <iostream>
#include <ncurses.h>
#include <cstdlib> //rand()
#include <ctime> //time()
#include <vector>

using namespace std;

class Map {
private:
    int width;
    int height;
    vector< vector<int> > map;

public:
    Map(int width, int height);
    
    void setObject(int y, int x, int point);
    void setWall(int y, int x);
    void setImmuneWall(int y, int x);
    void setGate(int y, int x);
    void setItem(int y, int x);
    void setPoison(int y, int x);
    int getObject(int y, int x);
    pair<int, int> getPoint(int y, int x);
    
    void makeWall();
    void makeImmuneWall();
    void makeGate();
    void makeItem();
    void makePoison();
    void makeSnake(int y, int x);

    int getWidth();
    int getHeight();

    void setWidth(int width);
    void setHeight(int height);
    

    void draw(WINDOW* win);
    bool isCollusion(int y, int x, int point);
};