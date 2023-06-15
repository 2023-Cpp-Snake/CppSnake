#include <ncurses.h>
#include <deque>
#include <utility> // pair
#include <unistd.h> // usleep
#include <cstdlib> // rand

// 뱀의 방향을 정의
enum Direction {
    UP, DOWN, LEFT, RIGHT
};

// 벽을 나타내는 클래스
class Wall {
private:
    int width;
    int height;

public:
    Wall(int width, int height) : width(width), height(height) {}

    void draw() const {
        // 왼쪽 및 오른쪽 벽 그리기
        for (int i = 0; i < height; i++) {
            mvprintw(i, 0, "#");
            mvprintw(i, width - 1, "#");
        }

        // 위쪽 및 아래쪽 벽 그리기
        for (int i = 0; i < width; i++) {
            mvprintw(0, i, "#");
            mvprintw(height - 1, i, "#");
        }
    }

    bool isCollision(int x, int y) const {
        return x == 0 || x == height - 1 || y == 0 || y == width - 1;
    }
};

// 아이템을 나타내는 클래스
class Item {
private:
    int x;
    int y;

public:
    Item(int x, int y) : x(x), y(y) {}

    void draw() const {
        mvprintw(x, y, "X");
    }

    bool isCollision(int x, int y) const {
        return this->x == x && this->y == y;
    }
};

// 게임 실행
int main() {
    initscr(); // ncurses를 초기화
    noecho(); // 사용자의 입력을 화면에 표시하지 않음
    keypad(stdscr, TRUE); // 특수 키를 사용 가능하게 함
    curs_set(0); // 커서를 보이지 않게 함
    nodelay(stdscr, TRUE);
    timeout(1000); // 사용자의 입력을 기다리는 시간을 1000ms로 설정


    std::deque< std::pair<int, int> > snake; // 뱀을 나타내는 데크
    Direction dir = RIGHT; // 처음에는 오른쪽으로 움직임

    // 벽의 크기 설정
    int wall_width = 21;
    int wall_height = 21;

    // 뱀의 초기 상태 설정
    snake.push_back(std::make_pair(10, 10));
    snake.push_back(std::make_pair(10, 11));
    snake.push_back(std::make_pair(10, 12));


    Wall wall(wall_width, wall_height) ; // 벽 생성
    Item item(1 + rand() % (wall_width-2), 1 + rand() % (wall_height-2)); // 아이템 생성

    while (true) {
        clear(); // 화면을 지움
        wall.draw(); // 벽을 화면에 그림

        // 뱀을 화면에 그림
        for (auto p : snake) {
            mvprintw(p.first, p.second, "o");
        }

        item.draw(); // 아이템을 화면에 그림

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
        std::pair<int, int> new_head = snake.back();
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

        snake.push_back(new_head); // 새로운 머리를 추가

        if (wall.isCollision(new_head.first, new_head.second)) { // 벽과 충돌
            break; // 프로그램 종료
        }

        if (item.isCollision(new_head.first, new_head.second)) { // 아이템과 충돌
            item = Item(1 + rand() % (wall_width-2), 1 + rand() % (wall_height-2)); // 새로운 아이템 생성
        } else {
            snake.pop_front(); // 꼬리를 제거
        }
        

        refresh(); // 화면을 갱신
    }
    endwin(); // ncurses를 종료
    return 0;

}