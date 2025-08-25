#include <iostream>
#include <cstdlib>
#include <unistd.h>   // sleep, usleep
#include <termios.h>  // getch
#include <fcntl.h>    // kbhit
#include <ctime>   // để random mồi
using namespace std;

void gotoxy(int column, int line) {
    printf("\033[%d;%dH", line, column);
}

int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

struct Point {
    int x, y;
};

class SNAKE {
public:
    Point A[100];
    int snake_length;

    SNAKE() {
        snake_length = 5;
        A[0].x = 10; A[0].y = 10;
        A[1].x = 11; A[1].y = 10;
        A[2].x = 12; A[2].y = 10;
        A[3].x = 13; A[3].y = 10;
        A[4].x = 14; A[4].y = 10;
    }

    void Draw() {
        for (int i = 0; i < snake_length; i++) {
            gotoxy(A[i].x, A[i].y);
            cout << "X";
        }
    }

    void Move(int direction) {
        for (int i = snake_length - 1; i > 0; i--)
            A[i] = A[i - 1];

        if (direction == 0) A[0].x = A[0].x + 1; // phải
        if (direction == 1) A[0].y = A[0].y + 1; // xuống
        if (direction == 2) A[0].x = A[0].x - 1; // trái
        if (direction == 3) A[0].y = A[0].y - 1; // lên
    }
};
// Hàm tạo mồi random trong khung 40x20
void generateFood(Point &food) {
    food.x = rand() % 40 + 1;  // ngang
    food.y = rand() % 20 + 1;  // dọc
}

int main() {
    SNAKE r;
    Point food;
    int score = 0;   // điểm ban đầu
srand(time(NULL));    // khởi tạo random seed
generateFood(food);   // tạo mồi ban đầu

    int direction = 0;
    char t;

    system("clear");
    while (1) {
        // Vẽ mồi
gotoxy(food.x, food.y);
cout << "O";

        if (kbhit()) {
            t = getch();
            if (t == 'a') direction = 2;
            if (t == 'w') direction = 3;
            if (t == 'd') direction = 0;
            if (t == 's') direction = 1;
            if (t == 'q') break;
        }
        system("clear");
        r.Draw();
        r.Move(direction);
        // Kiểm tra ăn mồi
if (r.A[0].x == food.x && r.A[0].y == food.y) {
    r.snake_length++;
    r.A[r.snake_length - 1] = r.A[r.snake_length - 2]; 
    generateFood(food);  // tạo mồi mới
    score += 10;   // tăng điểm mỗi lần ăn mồi
}

        usleep(200000); // 200ms
       


    }
    return 0;
}
