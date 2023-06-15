#include <deque>
#include <ncurses.h>
#include "score.h"
#include "map.h"
#include "direction.h"

using namespace std;

class Snake {
private:
    deque< pair<int, int> > body;
    Direction dir;
    bool isDead;
    
public:
    Snake(int height, int width, Direction dir);

    void setDir(Direction dir);
    Direction getDir();
    int getsize();
    bool getIsDead();
    pair<int, int> getHead();

    void move();
    void gate(int y, int x, Map map);
    void grow();
    void degrow();

    void interact(int y, int x, int point, Map& map, Score& score);

    void draw(WINDOW* win);

};