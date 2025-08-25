#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <ctime>
using namespace std;

void gotoxy(int column, int line) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)column, (SHORT)line };
    SetConsoleCursorPosition(hConsole, pos);
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
// TODO: Cần thêm xử lý game over khi rắn đụng tường hoặc tự cắn mình
int main() {
    SNAKE r;
    Point food;
    int score = 0;   // điểm ban đầu
srand(time(NULL));    // khởi tạo random seed
generateFood(food);   // tạo mồi ban đầu

    int direction = 0;
    char t;

    system("cls");
    while (1) {
        gotoxy(0, 0);
cout << "Score: " << score;

        // Vẽ mồi
gotoxy(food.x, food.y);
cout << "@";

        if (_kbhit()) {
            t = _getch();
            if (t == 'a') direction = 2;
            if (t == 'w') direction = 3;
            if (t == 'd') direction = 0;
            if (t == 's') direction = 1;
            if (t == 'q') break;
        }
        system("cls");
        r.Draw();
        r.Move(direction);
        // Kiểm tra ăn mồi
if (r.A[0].x == food.x && r.A[0].y == food.y) {
    r.snake_length++;
    r.A[r.snake_length - 1] = r.A[r.snake_length - 2]; 
    generateFood(food);  // tạo mồi mới
    score += 10;   // tăng điểm mỗi lần ăn mồi
}

        Sleep(200); // 200ms
       


    }
    return 0;
}
