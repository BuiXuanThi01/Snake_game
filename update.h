#ifndef UPDATE_H
#define UPDATE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <deque>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 20;

struct Point {
    int x, y;
    bool operator==(const Point& p) const {
        return x == p.x && y == p.y;
    }
};

enum Direction { UP, DOWN, LEFT, RIGHT };

class Update {
public:
    Update();
    ~Update();
    void run();

private:
    void handleEvents();
    void updateGame();
    void render();
    void renderText(const char* text, int x, int y);
    void spawnFood();
    bool isWall(const Point& p);
    bool isCollision(const Point& p);
    void createWalls();

    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool running;
    bool paused;
    int moveDelay;
    Uint32 lastMoveTime;
    std::deque<Point> snake;
    Direction dir;
    bool grow;
    int score;
    int targetScore;
    Point food;
    std::vector<Point> walls;
};

#endif
