#include <ncurses.h>
#include <deque>
#include <utility> // pair
#include <unistd.h> // usleep
#include <cstdlib> // rand

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
        mvwprintw(game_win, y, x, "X");
    }

    bool isCollision(int x, int y) const {
        return this->x == x && this->y == y;
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
        mvwprintw(game_win, y, x, "P");
    }

    void reset(int wall_width, int wall_height, deque < pair<int, int> > snake, WINDOW* game_win) {
        x = 1 + rand() % (wall_width-2);
        y = 1 + rand() % (wall_height-2);
        if (isCollision(snake.front().first, snake.front().second))
            reset(wall_width, wall_height, snake, game_win);
        else
            draw(game_win);
    }
};

// 게임 실행
int main() {
    initscr();
    clear();
    wrefresh(stdscr);

    int wall_width = 21;
    int wall_height = 21;
    WINDOW* game_win = newwin(wall_height, wall_width, 0, 0);

    timeout(1000); // 사용자의 입력을 기다리는 시간을 1000ms로 설정
    nodelay(game_win, TRUE);

    box(game_win, 0, 0); // 게임 화면을 테두리로 감싸줌
    srand(time(0)); // 난수 생성기 초기화
    noecho(); // 사용자의 입력을 화면에 표시하지 않음
    keypad(game_win, TRUE); // 특수 키를 사용 가능하게 함
    curs_set(0); // 커서를 보이지 않게 함
    
    


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
        int ch = getch();

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
            break; // 프로그램 종료
        }

        if (item.isCollision(new_head.first, new_head.second)) { // 아이템과 충돌
            item = Item(1 + rand() % (wall_width-2), 1 + rand() % (wall_height-2)); // 새로운 아이템 생성
            pItem.reset(wall_width, wall_height, snake, game_win); // 독 아이템을 새로 생성
        } else {
            snake.pop_front(); // 꼬리를 제거
        }
        

        wrefresh(game_win); // 게임 창을 갱신
    }
    endwin(); // ncurses를 종료
    return 0;

}