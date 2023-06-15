#include <ncurses.h>

int main() {
    initscr(); // ncurses를 초기화
    refresh();

    // 뱀 게임을 위한 창 생성
    WINDOW *game_win = newwin(22, 22, 0, 0);
    box(game_win, 0, 0); // 창에 테두리를 그림
    wrefresh(game_win); // 게임 창을 갱신

    // 점수판을 위한 창 생성
    WINDOW *score_win = newwin(10, 20, 0, 23);
    box(score_win, 0, 0); // 창에 테두리를 그림
    mvwprintw(score_win, 1, 1, "Score: %d", 0); // 점수를 출력
    wrefresh(score_win); // 점수판 창을 갱신

    // ...
    getch();
    delwin(game_win); // 게임 창을 삭제
    delwin(score_win); // 점수판 창을 삭제
    endwin(); // ncurses를 종료
    return 0;
}
