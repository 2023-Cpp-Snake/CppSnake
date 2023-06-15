#pragma once
#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "score.h"
#include "map.h"
#include "snake.h"
#include "direction.h"


int main(int argc, char const *argv[]) 
{
    srand(time(0));
    initscr();
    noecho();
    curs_set(0);
    refresh();
    int stage = 1, maxStage = 4;
    while(true) {
        //first stage
        //initialize map
        int height, width;
        height = 21;
        width = 21;
        Score score(0, 0, 0, 1, 1);
        Map map(height, width);
        Snake snake(height, width, RIGHT);
        WINDOW* game_win = newwin(height, width, 0, 0);
        WINDOW* score_win = newwin(14, 22, 0, width+1);
        nodelay(game_win, true);
        nodelay(score_win, true);
        keypad(game_win, true);

        if(stage == 1) {
            wtimeout(game_win, 1000);
            wclear(game_win);
            wclear(score_win);
        } else if (stage == 2) {
            score.clear();
            score.set(0, 0, 0, 2, maxStage);
            score.setCurrentStage(stage);
            wtimeout(game_win, 1000);
            wclear(game_win);
            wclear(score_win);
        } else if (stage == 3) {
            score.clear();
            score.set(0, 0, 0, 3, maxStage);
            score.setCurrentStage(stage);
            wtimeout(game_win, 1000);
            wclear(game_win);
            wclear(score_win);
        } else if (stage == 4) {
            score.clear();
            score.set(0, 0, 0, 4, maxStage);
            score.setCurrentStage(stage);
            wtimeout(game_win, 1000);
            wclear(game_win);
            wclear(score_win);
        }
        
   
        map.makeWall();
        map.makeImmuneWall();
        map.makeGate();
        map.makeGate();
        map.makeItem();
        map.makePoison();
        

        map.draw(game_win);
        score.draw(score_win);

        wrefresh(game_win);
        wrefresh(score_win);


        while(true) {
            score.draw(score_win);
            map.draw(game_win);
            snake.draw(game_win);

            int ch = wgetch(game_win);
            switch(ch) {
                case KEY_UP:
                    if (snake.getDir() == DOWN) break;
                    snake.setDir(UP);
                    break;
                case KEY_DOWN:
                    if (snake.getDir() == UP) break;
                    snake.setDir(DOWN);
                    break;
                case KEY_LEFT:
                    if (snake.getDir() == RIGHT) break;
                    snake.setDir(LEFT);
                    break;
                case KEY_RIGHT:
                    if (snake.getDir() == LEFT) break;
                    snake.setDir(RIGHT);
                    break;
                case 'q':
                    endwin();
                    return 0;
            }
            snake.move();
            //interact with snake's head
            int y, x;
            y = snake.getHead().first;
            x = snake.getHead().second;
            int point = map.getObject(y, x);
            snake.interact(y, x, point, map, score);
            
            wrefresh(game_win);
            wrefresh(score_win);
            score.addTimer();
            
            if(snake.getIsDead() == true) {
                break;
            }

            if(score.checkAllMax()) {
                stage += 1;
                break;
            }
        }

        if (snake.getIsDead()) {
            //game over
            wclear(game_win);
            box(game_win, 0, 0);
            mvwprintw(game_win, height/2, width/2-4, "Game Over");
            wrefresh(game_win);
            sleep(1);
            getch();
            endwin();
            return 0;
        }

        if (stage > maxStage) {
            //game over
            wclear(game_win);
            box(game_win, 0, 0);
            mvwprintw(game_win, height/2, width/2-5, "Game Clear");
            wrefresh(game_win);
            sleep(1);
            getch();
            endwin();
            break;
        }

        score.clear();
    }
    //game over
    return 0;
}
