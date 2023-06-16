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
    start_color();
    noecho();
    curs_set(0);
    refresh();
    int stage = 1, maxStage = 4;
    while(true) {
        //first stage
        //initialize map
        int height, width;
        height = 22;
        width = 22;
        Score score(5, 3, 1, 1, 4);
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
            score.set(7, 5, 3, 3, maxStage);
            score.setCurrentStage(stage);
            wtimeout(game_win, 800);
            wclear(game_win);
            wclear(score_win);
            map.setWall(height/2, width/2);
            map.setWall(height/2, width/2+1);
            map.setWall(height/2, width/2+2);
            map.setWall(height/2, width/2+3);
            map.setWall(height/2, width/2-1);
            map.setWall(height/2, width/2-2);
            map.setWall(height/2, width/2-3);
            map.setWall(height/2, width/2+4);
            map.setWall(height/2, width/2-4);
        } else if (stage == 3) {
            score.clear();
            score.set(10, 10, 5, 4, maxStage);
            score.setCurrentStage(stage);
            wtimeout(game_win, 600);
            wclear(game_win);
            wclear(score_win);


            map.setWall(height/2-2, width/2);
            map.setWall(height/2-1, width/2);
            map.setWall(height/2, width/2);
            map.setWall(height/2+1, width/2);
            map.setWall(height/2+2, width/2);
            map.setWall(height/2+3, width/2);
            map.setWall(height/2+3, width/2+1);
            map.setWall(height/2+3, width/2+2);
            map.setWall(height/2+3, width/2+3);
            map.setWall(height/2+3, width/2+4);
            map.setWall(height/2+3, width/2+5);
            map.setWall(height/2+3, width/2+6);
            map.setWall(height/2+3, width/2+7);
        } else if (stage == 4) {
            score.clear();
            score.set(13, 11, 7, 5, maxStage);
            score.setCurrentStage(stage);
            wtimeout(game_win, 400);
            wclear(game_win);
            wclear(score_win);


            map.setWall(height/2, width/2);
            map.setWall(height/2, width/2-1);
            map.setWall(height/2, width/2-2);
            map.setWall(height/2, width/2-3);
            map.setWall(height/2, width/2-4);
            map.setWall(height/2, width/2+1);
            map.setWall(height/2, width/2+2);
            map.setWall(height/2, width/2+3);
            map.setWall(height/2, width/2+4);
            map.setWall(height/2+1, width/2);
            map.setWall(height/2+2, width/2);
            map.setWall(height/2+3, width/2);
            map.setWall(height/2+4, width/2);
            map.setWall(height/2-1, width/2);
            map.setWall(height/2-2, width/2);
            map.setWall(height/2-3, width/2);
            map.setWall(height/2-4, width/2);
            map.setWall(height/2+1, width/2+1);
            map.setWall(height/2+2, width/2+2);
            map.setWall(height/2+3, width/2+3);
            map.setWall(height/2+4, width/2+4);
            map.setWall(height/2-1, width/2-1);
            map.setWall(height/2-2, width/2-2);
            map.setWall(height/2-3, width/2-3);
            map.setWall(height/2-4, width/2-4);
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
            snake.move(map);
            //interact with snake's head
            int y, x;
            y = snake.getHead().first;
            x = snake.getHead().second;
            int point = map.getObject(y, x);
            snake.makeSnake(map);
            snake.interact(y, x, point, map, score);

            score.setCurrentLength(snake.getsize());
            
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
            init_pair(8, COLOR_RED, COLOR_BLACK);
            wattron(game_win, COLOR_PAIR(8));
            mvwprintw(game_win, height/2, width/2-4, "Game Over");
            wattroff(game_win, COLOR_PAIR(8));
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
            init_pair(9, COLOR_WHITE, COLOR_BLACK);
            wattron(game_win, COLOR_PAIR(9));
            mvwprintw(game_win, height/2, width/2-5, "Game Clear");
            wattroff(game_win, COLOR_PAIR(9));
            wrefresh(game_win);
            sleep(1);
            getch();
            endwin();
            break;
        }

        score.clear();
    }

    return 0;
}
