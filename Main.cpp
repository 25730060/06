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
        A[0].x = 14; A[0].y = 10;
        A[1].x = 13; A[1].y = 10;
        A[2].x = 12; A[2].y = 10;
        A[3].x = 11; A[3].y = 10;
        A[4].x = 10; A[4].y = 10;
    }

    void Draw() {
        for (int i = 0; i < snake_length; i++) {
            gotoxy(A[i].x, A[i].y);
            cout << (i == 0 ? "O" : "X"); // đầu khác thân
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

    // Hàm xử lý game over
    bool CheckGameOver(int width, int height, int mode) {
        // Mode 1: Đụng tường Game Over
        // Mode 2: Đụng tường -> xuất hiện phía đối diện
        if (mode == 1) {
            // Đụng tường (khung có viền )
            if (A[0].x <= 0 || A[0].x >= width-1 || A[0].y <= 0 || A[0].y >= height-1)
                    return true;
        } else {
            // Đụng tường -> xuất hiện phía đối diện
            if (A[0].x <= 0) A[0].x = width - 2;
            else if (A[0].x >= width - 1) A[0].x = 1;
            if (A[0].y <= 0) A[0].y = height - 2;
            else if (A[0].y >= height - 1) A[0].y = 1;
        }

        // Tự cắn thân
        for (int i = 1; i < snake_length; i++) {
            if (A[0].x == A[i].x && A[0].y == A[i].y)
                return true;
        }
        return false;
    }

    // tránh mồi sinh trùng thân
    bool occupies(int x, int y) {
        for (int i = 0; i < snake_length; ++i)
            if (A[i].x == x && A[i].y == y) return true;
        return false;
    }
};

// Hàm tạo mồi random trong khung ( Trừ viền và vị trí rắn )
void generateFood(Point &food, int width, int height, SNAKE &r) {
    do {
        food.x = rand() % (width  - 2) + 1;
        food.y = rand() % (height - 2) + 1;
    } while (r.occupies(food.x, food.y));
}

// Vẽ khung
void drawFrame(int width, int height) {
    for (int x = 0; x < width; x++) {
        gotoxy(x, 0); cout << "#";
        gotoxy(x, height - 1); cout << "#";
    }
    for (int y = 0; y < height; y++) {
        gotoxy(0, y); cout << "#";
        gotoxy(width - 1, y); cout << "#";
    }
}

// Show menu chọn chế độ chơi
// Mode 1: Đụng tường Game Over
// Mode 2: Đụng tường -> xuất hiện phía đối diện
int showMenu() {
    int choice = 0;
    while (true) {
        system("cls");
        cout << "=============================\n";
        cout << "         SNAKE GAME          \n";
        cout << "=============================\n";
        cout << "1. Wall Collision: Game Over\n";
        cout << "2. Wall Collision: Wrap Around\n";
        cout << "Choose mode (1-2): ";
        cin >> choice;
        if (choice == 1 || choice == 2) break;
    }
    return choice;
}

int main() {
    const int WIDTH = 40, HEIGHT = 20; // khung bao gồm viền ngoài
    SNAKE r;
    Point food;
    int score = 0;   // điểm ban đầu
    int direction = 0;

    // --- MENU ---
    int mode = showMenu(); // 1 = Game Over, 2 = Xuyên tường

    srand(time(NULL));    // khởi tạo random seed
    system("cls");
    drawFrame(WIDTH, HEIGHT);
    generateFood(food, WIDTH, HEIGHT, r);   // tạo mồi ban đầu
  // Vòng lặp chính của game
    while (true) {
        // Xoá bên trong khung (không xoá viền)
        for (int y = 1; y < HEIGHT-1; ++y) {
            gotoxy(1, y);
            for (int x = 1; x < WIDTH-1; ++x) cout << " ";
        }

    if (_kbhit()) {
            char t = _getch();
            if (t == 'q' || t == 27) break;              // Q hoặc ESC thoát
            if (t == 'a' && direction != 0) direction = 2; // không cho quay ngược từ phải -> trái ngay
            if (t == 'd' && direction != 2) direction = 0; // không cho quay ngược từ trái -> phải
            if (t == 'w' && direction != 1) direction = 3; // không cho quay ngược từ xuống -> lên
            if (t == 's' && direction != 3) direction = 1; // không cho quay ngược từ lên -> xuống
        }
   // Di chuyển
    r.Move(direction);

    // Kiểm tra Game Over
    if (r.CheckGameOver(WIDTH, HEIGHT, mode)) {
        gotoxy(0, HEIGHT + 1);
        cout << "GAME OVER! Score = " << score << endl;
        break;
    }

   // Kiểm tra ăn mồi
    if (r.A[0].x == food.x && r.A[0].y == food.y) {
        if (r.snake_length < 100) {
                r.A[r.snake_length] = r.A[r.snake_length - 1];
                r.snake_length++;
        }
        score += 10;   // tăng điểm mỗi lần ăn mồi
        generateFood(food, WIDTH, HEIGHT,r);  // tạo mồi mới
    }

    // Vẽ mồi + rắn
    gotoxy(food.x, food.y);
    cout << "@";
    r.Draw();


    // Hiển thị điểm
        gotoxy(0, HEIGHT);
        cout << "Score: " << score << " (WASD di chuyen, Q/ESC thoat)  ";

        Sleep(200); // 200ms
    }
    return 0;
}
