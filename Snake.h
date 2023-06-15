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
};