#include "Snake.h"

class Wall {

protected:
    char direction; // 'l'eft, 'r'ight, 'u'p, 'd'own
    int x, y;
 
public:
    Wall(char direction);
    void collusion(Snake snake);
    void draw();
};

class ImmuneWall : public Wall {
public:
    ImmuneWall(char direction);
    void draw();
};