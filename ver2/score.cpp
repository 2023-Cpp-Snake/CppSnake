#include "score.h"

Score::Score(int maxLength, int maxGrowth, int maxPoison, int maxGate, int maxStage) {
    this->currentLength = 0;
    this->maxLength = maxLength;
    this->currentGrowth = 0;
    this->maxGrowth = maxGrowth;
    this->currentPoison = 0;
    this->maxPoison = maxPoison;
    this->currentGate = 0;
    this->maxGate = maxGate;
    this->currentStage = 1;
    this->maxStage = maxStage;
    this->timer = 0;
}

int Score::getCurrentLength() {
    return currentLength;
}

int Score::getMaxLength() {
    return maxLength;
}

int Score::getCurrentGrowth() {
    return currentGrowth;
}

int Score::getMaxGrowth() {
    return maxGrowth;
}

int Score::getCurrentPoison() {
    return currentPoison;
}

int Score::getMaxPoison() {
    return maxPoison;
}

int Score::getCurrentGate() {
    return currentGate;
}

int Score::getMaxGate() {
    return maxGate;
}

void Score::setCurrentLength(int currentLength) {
    this->currentLength = currentLength;
}

void Score::setMaxLength(int maxLength) {
    this->maxLength = maxLength;
}

void Score::setCurrentGrowth(int currentGrowth) {
    this->currentGrowth = currentGrowth;
}

void Score::setMaxGrowth(int maxGrowth) {
    this->maxGrowth = maxGrowth;
}

void Score::setCurrentPoison(int currentPoison) {
    this->currentPoison = currentPoison;
}

void Score::setMaxPoison(int maxPoison) {
    this->maxPoison = maxPoison;
}

void Score::setCurrentGate(int currentGate) {
    this->currentGate = currentGate;
}

void Score::setMaxGate(int maxGate) {
    this->maxGate = maxGate;
}

int Score::getCurrentStage() {
    return currentStage;
}

void Score::setCurrentStage(int currentStage) {
    this->currentStage = currentStage;
}

int Score::getMaxStage() {
    return maxStage;
}

void Score::setMaxStage(int maxStage) {
    this->maxStage = maxStage;
}


void Score::draw(WINDOW* win) {
    box(win, 0, 0);
    init_pair(10, COLOR_WHITE, COLOR_BLACK);
    wattron(win, COLOR_PAIR(10));
    mvwprintw(win, 1, 1, "Score Board");
    mvwprintw(win, 2, 1, "Timer: %d", timer);
    mvwprintw(win, 3, 1, "Stage: %d", currentStage);

    mvwprintw(win, 4, 1, "B: %d", currentLength);
    mvwprintw(win, 5, 1, "+: %d", currentGrowth);
    mvwprintw(win, 6, 1, "-: %d", currentPoison);
    mvwprintw(win, 7, 1, "G: %d", currentGate);
    
    mvwprintw(win, 9, 1, "B: %d (%s)", maxLength, currentLength>=maxLength?"V":" ");
    mvwprintw(win, 10, 1, "+: %d (%s)", maxGrowth, currentGrowth>=maxGrowth?"V":" ");
    mvwprintw(win, 11, 1, "-: %d (%s)", maxPoison, currentPoison>=maxPoison?"V":" ");
    mvwprintw(win, 12, 1, "G: %d (%s)", maxGate, currentGate>=maxGate?"V":" ");
    wattroff(win, COLOR_PAIR(10));
}

bool Score::checkAllMax() {
    return currentLength >= maxLength && currentGrowth >= maxGrowth && currentPoison >= maxPoison && currentGate >= maxGate;
}

void Score::addTimer() {
    timer++;
}

void Score::clear(){
    currentLength = 0;
    currentGrowth = 0;
    currentPoison = 0;
    currentGate = 0;
    maxLength = 0;
    maxGrowth = 0;
    maxPoison = 0;
    maxGate = 0;
    timer = 0;

}

void Score::set(int maxLength, int maxGrowth, int maxPoison, int maxGate, int maxStage) {
    this->currentLength = 0;
    this->maxLength = maxLength;
    this->currentGrowth = 0;
    this->maxGrowth = maxGrowth;
    this->currentPoison = 0;
    this->maxPoison = maxPoison;
    this->currentGate = 0;
    this->maxGate = maxGate;
    this->currentStage = 1;
    this->maxStage = maxStage;
    this->timer = 0;
}