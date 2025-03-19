#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>

using namespace std;

const int SCREEN_WIDTH = 30;
const int SCREEN_HEIGHT = 15;
const char SNAKE_CHAR = 'O';
const char FOOD_CHAR = 'X';
const char EMPTY_CHAR = ' ';
const char BORDER_CHAR = '#';

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Position {
    int x, y;
};

class Snake {
public:
    Snake(int startX, int startY) {
        body.push_back({ startX, startY });
        direction = RIGHT;
        growNextMove = false;
    }

    void Move() {
        Position newHead = body.front();
        switch (direction) {
        case UP:    newHead.y--; break;
        case DOWN:  newHead.y++; break;
        case LEFT:  newHead.x--; break;
        case RIGHT: newHead.x++; break;
        }
        body.insert(body.begin(), newHead);

        if (!growNextMove) {
            body.pop_back();
        }
        else {
            growNextMove = false;
        }
    }

    void Grow() {
        growNextMove = true; 
    }

    void SetDirection(Direction newDir) {
        if ((direction == UP && newDir == DOWN) || (direction == DOWN && newDir == UP) ||
            (direction == LEFT && newDir == RIGHT) || (direction == RIGHT && newDir == LEFT))
            return;
        direction = newDir;
    }

    bool CheckCollision() const {
        Position head = body.front();
        if (head.x <= 0 || head.x >= SCREEN_WIDTH - 1 || head.y <= 0 || head.y >= SCREEN_HEIGHT - 1)
            return true;
        for (size_t i = 1; i < body.size(); ++i)
            if (body[i].x == head.x && body[i].y == head.y)
                return true;
        return false;
    }

    bool IsOnPosition(int x, int y) const {
        for (const auto& segment : body)
            if (segment.x == x && segment.y == y)
                return true;
        return false;
    }

    Position GetHeadPosition() const {
        return body.front();
    }

    vector<Position> GetBody() const {
        return body;
    }

private:
    vector<Position> body;
    Direction direction;
    bool growNextMove;
};

class Food {
public:
    Food() { Respawn(vector<Position>()); }

    void Respawn(const vector<Position>& snakeBody) {
        do {
            x = rand() % (SCREEN_WIDTH - 2) + 1;
            y = rand() % (SCREEN_HEIGHT - 2) + 1;
        } while (IsOnSnake(snakeBody));
    }

    int GetX() const { return x; }
    int GetY() const { return y; }

private:
    int x, y;

    bool IsOnSnake(const vector<Position>& snakeBody) const {
        for (const auto& segment : snakeBody) {
            if (segment.x == x && segment.y == y)
                return true;
        }
        return false;
    }
};

class Game {
public:
    Game() : snake(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), food(), score(0) {
        isRunning = true;
        food.Respawn(snake.GetBody());
    }

    void Run() {
        while (isRunning) {
            Draw();
            HandleInput();
            Update();
            Sleep(100);
        }
        GameOverScreen();
    }

private:
    Snake snake;
    Food food;
    bool isRunning;
    int score;

    void Draw() const {
        COORD cursorPosition = { 0, 0 };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

        cout << "Score: " << score << "\n";
        for (int y = 0; y < SCREEN_HEIGHT; ++y) {
            for (int x = 0; x < SCREEN_WIDTH; ++x) {
                if (x == 0 || x == SCREEN_WIDTH - 1 || y == 0 || y == SCREEN_HEIGHT - 1)
                    cout << BORDER_CHAR;
                else if (snake.IsOnPosition(x, y))
                    cout << SNAKE_CHAR;
                else if (food.GetX() == x && food.GetY() == y)
                    cout << FOOD_CHAR;
                else
                    cout << EMPTY_CHAR;
            }
            cout << '\n';
        }
    }

    void HandleInput() {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) {
                key = _getch();
                switch (key) {
                case 72: snake.SetDirection(UP); break;
                case 80: snake.SetDirection(DOWN); break;
                case 75: snake.SetDirection(LEFT); break;
                case 77: snake.SetDirection(RIGHT); break;
                }
            }
        }
    }

    void Update() {
        snake.Move();
        if (snake.GetHeadPosition().x == food.GetX() && snake.GetHeadPosition().y == food.GetY()) {
            snake.Grow();
            food.Respawn(snake.GetBody());
            score += 10;
        }
        if (snake.CheckCollision())
            isRunning = false;
    }

    void GameOverScreen() const {
        system("cls");
        cout << "=============================\n";
        cout << "         GAME OVER!          \n";
        cout << "      Final Score: " << score << "\n";
        cout << "=============================\n";
        cout << "Press any key to exit...\n";
        _getch();
    }
};

int main() {
    srand(time(0));

    system("color 0A");
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.bVisible = false;
    cursorInfo.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    Game game;
    game.Run();
    return 0;
}
