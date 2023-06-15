class Snake {
    private:
    int x, y;
    bool isAlive;
    int length;
    char direction;


    public:
    Snake(int x, int y);
    int getX();
    int getY();
    void setX(int x);
    void setY(int y);
    void setDirection(char direction);
    char getDirection();
    void move();
    void draw();
    void collusion();
    void eat();
    bool getIsAlive();
    void setIsAlive(bool isAlive);
    int getLength();
    void setLength(int length);

};