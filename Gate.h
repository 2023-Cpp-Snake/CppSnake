class Gate {
    private :
    int x, y;
    char direction;

    public :
    Gate(int x, int y, char direction);
    int getX();
    int getY();
    char getDirection();
    void setX(int x);
    void setY(int y);
    void setDirection(char direction);
};