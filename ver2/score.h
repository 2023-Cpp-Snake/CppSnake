#pragma once
#include <ncurses.h>


class Score {
private:
    int currentStage;
    int maxStage;

    int currentLength;
    int maxLength;

    int currentGrowth;
    int maxGrowth;

    int currentPoison;
    int maxPoison;

    int currentGate;
    int maxGate;

    int timer;
public:
    Score(int maxLength, int maxGrowth, int maxPoison, int maxGate, int maxStage);
    void set(int maxLength, int maxGrowth, int maxPoison, int maxGate, int maxStage);

    int getCurrentLength();
    int getMaxLength();
    int getCurrentGrowth();
    int getMaxGrowth();
    int getCurrentPoison();
    int getMaxPoison();
    int getCurrentGate();
    int getMaxGate();
    int getCurrentStage();
    int getMaxStage();  

    void setCurrentLength(int currentLength);
    void setMaxLength(int maxLength);
    void setCurrentGrowth(int currentGrowth);
    void setMaxGrowth(int maxGrowth);
    void setCurrentPoison(int currentPoison);
    void setMaxPoison(int maxPoison);
    void setCurrentGate(int currentGate);
    void setMaxGate(int maxGate);
    void setCurrentStage(int currentStage);
    void setMaxStage(int maxStage);

    void clear();


    void addTimer();

    bool checkAllMax();

    void draw(WINDOW* win);
};