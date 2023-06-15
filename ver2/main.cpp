#pragma once
#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
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
    while(true) {
        //first stage
        //initialize map
        int height, width;
        height = 21;
        width = 21;
        WINDOW* game_win = newwin(height, width, 0, 0);
        nodelay(game_win, TRUE);
        wtimeout(game_win, 1000);
        keypad(game_win, TRUE); // 특수 키를 사용 가능하게 함

        wclear(game_win);
        wrefresh(game_win);

        Map map(height, width);
        Snake snake(height, width, RIGHT);

        map.makeWall();
        map.makeImmuneWall();
        map.makeGate();
        map.makeGate();
        map.makeItem();
        map.makePoison();

        map.draw(game_win);

        while(true) {
            map.draw(game_win);
            snake.draw(game_win);

            int ch = wgetch(game_win);
            switch(ch) {
                case KEY_UP:
                    snake.setDir(UP);
                    break;
                case KEY_DOWN:
                    snake.setDir(DOWN);
                    break;
                case KEY_LEFT:
                    snake.setDir(LEFT);
                    break;
                case KEY_RIGHT:
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
            snake.interact(y, x, point, map);
            
            wrefresh(game_win);
            if(snake.getsize() == 25) {
                break;
            }
            if(snake.getIsDead()) {
                break;
            }
        }
    }
    return 0;
}
