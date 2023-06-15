#include <ncurses.h>
#include <deque>
#include <utility> // pair
#include <unistd.h> // usleep
#include <cstdlib> // rand
#include <ctime> // time

using namespace std;

// 뱀의 방향을 정의
enum Direction {
    UP, DOWN, LEFT, RIGHT
};

// 벽을 나타내는 클래스
class Wall {
protected:
    int width;
    int height;

public:
    Wall(int width, int height) : width(width), height(height) {}

    void draw(WINDOW* game_win) const {
        // 왼쪽 및 오른쪽 벽 그리기
        for (int i = 0; i < height; i++) {
            mvwprintw(game_win, i, 0, "#");
            mvwprintw(game_win, i, width - 1, "#");
        }

        // 위쪽 및 아래쪽 벽 그리기
        for (int i = 0; i < width; i++) {
            mvwprintw(game_win, 0, i, "#");
            mvwprintw(game_win, height - 1, i, "#");
        }
    }   

    bool isCollision(int x, int y) const {
        return x == 0 || x == height - 1 || y == 0 || y == width - 1;
    }
};

// 게이트가 생성되지 않는 벽을 나타내는 클래스, wall의 꼭지점은 게이트가 생성되지 않음
class ImmuneWall : public Wall{
    public:
    ImmuneWall(int width, int height) : Wall(width, height) {}
    void draw(WINDOW* game_win) const {

        //꼭지점 그리기
        mvwprintw(game_win, 0, 0, "*");
        mvwprintw(game_win, 0, width-1, "*");
        mvwprintw(game_win, height-1, 0, "*");
        mvwprintw(game_win, height-1, width-1, "*");
    }
};

// 게이트를 나타내는 클래스
class Gate {
private:
    int width;
    int height;
    int x, y;

public:
    Gate(int width, int height) : width(width), height(height){
        //게이트가 벽위에 생성되도록 설정, ImmuneWall의 꼭지점은 게이트가 생성되지 않으므로 벽의 크기에서 2를 뺌
        int dir = rand()%4;
        if (dir == 0) {
            x = 0;
            y = 1 + rand() % (width-2);
        } else if (dir == 1) {
            x = height-1;
            y = 1 + rand() % (width-2);
        } else if (dir == 2) {
            x = 1 + rand() % (height-2);
            y = 0;
        } else {
            x = 1 + rand() % (height-2);
            y = width-1;
        }
    }

    void draw(WINDOW* game_win) const {
        mvwprintw(game_win, x, y, "G");
    }

    bool isCollision(int x, int y) const {
        return (this->x == x && this->y == y);
    }
    
    //GetDirection
    Direction getDir() const {
        if (x == 0)
            return DOWN;
        else if (x == height-1)
            return UP;
        else if (y == 0)
            return RIGHT;
        else
            return LEFT;
    }

    void reset(WINDOW* game_win) {
        int dir = rand()%4;
        if (dir == 0) {
            x = 0;
            y = 1 + rand() % (width-2);
        } else if (dir == 1) {
            x = height-1;
            y = 1 + rand() % (width-2);
        } else if (dir == 2) {
            x = 1 + rand() % (height-2);
            y = 0;
        } else {
            x = 1 + rand() % (height-2);
            y = width-1;
        }

        draw(game_win);
    }

    //getter, setter
    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    void setX(int x) {
        this->x = x;
    }

    void setY(int y) {
        this->y = y;
    }
};


// 아이템을 나타내는 클래스
class Item {
protected:
    int x;
    int y;

public:
    Item(int x, int y) : x(x), y(y) {}

    void draw(WINDOW* game_win) const {
        mvwprintw(game_win, x, y, "X");
    }

    bool isCollision(int x, int y) const {
        return this->x == x && this->y == y;
    }

    void reset(int wall_width, int wall_height, deque < pair<int, int> > snake, WINDOW* game_win) {
        x = 1 + rand() % (wall_width-2);
        y = 1 + rand() % (wall_height-2);
        // 뱀의 머리 혹은 몸통과 충돌하면 다시 생성
        for(auto p : snake) {
            if (isCollision(p.first, p.second)) {
                reset(wall_width, wall_height, snake, game_win);
                break;
            } else {
                draw(game_win);
            }
        }
        
    }

    //getter, setter
    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    void setX(int x) {
        this->x = x;
    }

    void setY(int y) {
        this->y = y;
    }
};

class PoisonItem : public Item {
public:
    PoisonItem(int x, int y) : Item(x, y) {}

    void draw(WINDOW* game_win) const {
        mvwprintw(game_win, x, y, "P");
    }
};

// 점수를 나타내는 클래스
class Score {
private:
    int score;
    int level;
    int speed;

public:
    Score() : score(0), level(1), speed(1000) {}

    void draw(WINDOW* score_win) const {
        mvwprintw(score_win, 1, 1, "Score: %d", score);
        mvwprintw(score_win, 2, 1, "Level: %d", level);
    }

    void addScore(int score) {
        this->score += score;
    }

    void addLevel() {
        level++;
    }

    void addSpeed() {
        if(speed > 100) speed -= 100;
    }

    //getter, setter
    int getScore() const {
        return score;
    }

    int getLevel() const {
        return level;
    }

    int getSpeed() const {
        return speed;
    }

    void setScore(int score) {
        this->score = score;
    }

    void setLevel(int level) {
        this->level = level;
    }

    void setSpeed(int speed) {
        this->speed = speed;
    }
};

class TitleScreen {
private:
    int width;
    int height;
public:
    TitleScreen(int width, int height) : width(width), height(height) {}

    void draw(WINDOW* win) const {
        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, height/2, (width/2)-5, "SNAKE GAME");
        mvwprintw(win, height/2+1, (width/2)-6, "PRESS ANY KEY");
        wrefresh(win);
    }
};

class GameOverScreen {
private:
    int width;
    int height;
public:
    GameOverScreen(int width, int height) : width(width), height(height) {}

    void draw(WINDOW* win) const {
        wclear(win);
        box(win, 0, 0);
        mvwprintw(win, height/2, (width/2)-5, "GAME OVER");
        mvwprintw(win, height/2+1, (width/2)-5, "PRESS ANY KEY");
        mvwprintw(win, height/2+1, (width/2)-5, "TO SHUTDOWN");
        wrefresh(win);
    }
};

// 게임 실행
int main() {
    srand(time(0)); // 난수 생성기 초기화
    Score score; // 점수
    initscr();
    refresh();

    int wall_width = 23;
    int wall_height = 23;

    WINDOW* score_win = newwin(4, wall_width, wall_height, 0);
    WINDOW* game_win = newwin(wall_height, wall_width, 0, 0);
    keypad(game_win, TRUE); // 특수 키를 사용 가능하게 함
    noecho(); // 사용자의 입력을 화면에 표시하지 않음
    curs_set(0); // 커서를 보이지 않게 함


    TitleScreen title(wall_width, wall_height);
    GameOverScreen gameover(wall_width, wall_height);

    title.draw(game_win);
    wrefresh(game_win);
    getch();

    nodelay(game_win, TRUE); // 사용자의 입력을 기다리지 않음
    wtimeout(game_win, score.getSpeed()); // 사용자의 입력을 기다리는 시간을 1000ms로 설정
    box(game_win, 0, 0); // 게임 화면을 테두리로 감싸줌
    box(score_win, 0, 0); // 점수 화면을 테두리로 감싸줌


    deque< pair<int, int> > snake; // 뱀을 나타내는 데크
    Direction dir = RIGHT; // 처음에는 오른쪽으로 움직임

   

    // 뱀의 초기 상태 설정
    snake.push_back(make_pair(10, 10));
    snake.push_back(make_pair(10, 11));
    snake.push_back(make_pair(10, 12));


    Wall wall(wall_width, wall_height) ; // 벽 생성
    ImmuneWall iWall(wall_width, wall_height); // 게이트가 생성되지 않는 벽 생성
    Gate gate1(wall_width, wall_height); // 게이트 생성
    Gate gate2(wall_width, wall_height); // 게이트 생성
    while(gate1.isCollision(gate2.getX(), gate2.getY())) { // 게이트가 겹치지 않도록 설정
        gate2 = Gate(wall_width, wall_height);
    }

    Item item(1 + rand() % (wall_width-2), 1 + rand() % (wall_height-2)); // 아이템 생성
    PoisonItem pItem(1 + rand() % (wall_width-2), 1 + rand() % (wall_height-2)); // 독 아이템 생성

    //만약, 초기 뱀의 위치와 겹친다면 다시 생성
    while (item.isCollision(snake.front().first, snake.front().second)) {
        item = Item(1 + rand() % (wall_width-2), 1 + rand() % (wall_height-2));
    }
    while(item.isCollision(pItem.getX(), pItem.getY())) {
        pItem = PoisonItem(1 + rand() % (wall_width-2), 1 + rand() % (wall_height-2));
    }

    while (true) {
        wclear(game_win); // 게임 창을 지움
        box(game_win, 0, 0); // 창에 테두리를 그림
        box(score_win, 0, 0); // 점수 화면을 테두리로 감싸줌
        score.draw(score_win); // 점수를 화면에 그림
        wrefresh(score_win); // 점수 창을 화면에 그림
        wrefresh(game_win); // 게임 창을 화면에 그림
        //wall.draw(game_win); // 벽을 화면에 그림
        iWall.draw(game_win); // 게이트가 생성되지 않는 벽을 화면에 그림
        gate1.draw(game_win); // 게이트를 화면에 그림
        gate2.draw(game_win); // 게이트를 화면에 그림
        

        // 뱀을 화면에 그림
        for (auto p : snake) { //뱀의 머리는 화살표로 표시
            if (p == snake.back()) {
                if (dir == UP) {
                    mvwprintw(game_win, p.first, p.second, "^");
                } else if (dir == DOWN) {
                    mvwprintw(game_win, p.first, p.second, "v");
                } else if (dir == LEFT) {
                    mvwprintw(game_win, p.first, p.second, "<");
                } else if (dir == RIGHT) {
                    mvwprintw(game_win, p.first, p.second, ">");
                }
            } else {
                mvwprintw(game_win, p.first, p.second, "o");
            }
        }

        item.draw(game_win); // 아이템을 화면에 그림
        pItem.draw(game_win); // 독 아이템을 화면에 그림

        // 키 입력을 처리
        int ch = wgetch(game_win);

        if (ch == KEY_UP && dir != DOWN) {
            dir = UP;
        } else if (ch == KEY_DOWN && dir != UP) {
            dir = DOWN;
        } else if (ch == KEY_LEFT && dir != RIGHT) {
            dir = LEFT;
        } else if (ch == KEY_RIGHT && dir != LEFT) {
            dir = RIGHT;
        }

        // 뱀의 움직임을 처리
        pair<int, int> new_head = snake.back();
        if (dir == UP) {
            new_head.first--;
        } else if (dir == DOWN) {
            new_head.first++;
        } else if (dir == LEFT) {
            new_head.second--;
        } else if (dir == RIGHT) {
            new_head.second++;
        }

        //몸통과 충돌
        for (auto p : snake) {
            if (p.first == new_head.first && p.second == new_head.second) {
                endwin();
                return 0;
            }
        }

        if(gate1.isCollision(new_head.first, new_head.second)) { // 게이트와 충돌
            dir = gate2.getDir(); // 방향을 바꿈
            if(dir == UP) {
                new_head = make_pair(gate2.getX()-1, gate2.getY());
            } else if(dir == DOWN) {
                new_head = make_pair(gate2.getX()+1, gate2.getY());
            } else if(dir == LEFT) {
                new_head = make_pair(gate2.getX(), gate2.getY()-1);
            } else if(dir == RIGHT) {
                new_head = make_pair(gate2.getX(), gate2.getY()+1);
            }
        }

        if(gate2.isCollision(new_head.first, new_head.second)) { // 게이트와 충돌
            dir = gate1.getDir(); // 방향을 바꿈
            if(dir == UP) {
                new_head = make_pair(gate1.getX()-1, gate1.getY());
            } else if(dir == DOWN) {
                new_head = make_pair(gate1.getX()+1, gate1.getY());
            } else if(dir == LEFT) {
                new_head = make_pair(gate1.getX(), gate1.getY()-1);
            } else if(dir == RIGHT) {
                new_head = make_pair(gate1.getX(), gate1.getY()+1);
            }
        }

        snake.push_back(new_head); // 새로운 머리를 추가

        if (wall.isCollision(new_head.first, new_head.second)) { // 벽과 충돌
            break; // 프로그램 종료
        }

        if (pItem.isCollision(new_head.first, new_head.second)) { // 독 아이템과 충돌
            //꼬리를 1칸 제거
            snake.pop_front();
            //독 아이템을 새로 생성
            pItem.reset(wall_width, wall_height, snake, game_win);
            //snake의 길이가 3 미만이라면, 게임 끝
            if(snake.size() <= 3) {
                break;
            }
        }

        if (item.isCollision(new_head.first, new_head.second)) { // 아이템과 충돌
            score.setScore(score.getScore() + 10); // 점수를 10 증가
            item.reset(wall_width, wall_height, snake, game_win); // 아이템을 새로 생성
            pItem.reset(wall_width, wall_height, snake, game_win); // 독 아이템을 새로 생성
            gate1.reset(game_win); // 게이트를 새로 생성
            gate2.reset(game_win); // 게이트를 새로 생성

        } else {
            snake.pop_front(); // 꼬리를 제거
        }
        
        // 스코어가 50점이 넘을 때 마다, 레벨 증가
        if (score.getScore() / score.getLevel() >= 50) {
            score.addLevel();
            score.addSpeed();
            wtimeout(game_win, score.getSpeed());
        }

        score.draw(score_win); // 점수를 화면에 그림

        wrefresh(game_win); // 게임 창을 갱신
        wrefresh(score_win); // 점수 창을 갱신
    }
    //게임오버 메시지를 화면에 출력
    gameover.draw(game_win);
    wrefresh(game_win); // 게임 창을 갱신
    getch(); // 사용자가 키를 입력할 때까지 대기


    endwin(); // ncurses를 종료
    return 0;

}