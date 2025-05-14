#include "update.h"
#include <iostream>


// Ham khoi tao cua class Update
Update::Update() {
    SDL_Init(SDL_INIT_VIDEO); // Khoi tao SDL
    TTF_Init(); // Khoi tao SDL_ttf de su dung font
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0); // Tao cua so cho game
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // Tao renderer cho cua so
    font = TTF_OpenFont("arial.ttf", 24); // Mo font cho game

    running = true; // Game chua ket thuc
    paused = false; // Game chua bi tam dung
    moveDelay = 120; // Thoi gian di chuyen giua moi lan (delay)
    lastMoveTime = SDL_GetTicks(); // Thoi gian di chuyen cuoi cung
    srand(static_cast<unsigned int>(time(0))); // Khoi tao ngau nhien cho vi tri thuc an


    snake = { {10, 10} }; // Khoi tao vi tri ban dau cho ran
    dir = RIGHT; // Huong di chuyen cua ran la phai
    grow = false; // Ran khong len dai khi moi bat dau
    score = 0; // Diem so ban dau
    targetScore = 15; // Diem so de thang

    createWalls(); // Tao tuong cho game
    spawnFood(); // Sinh thuc an cho game
}

// Ham huy cua class Update
Update::~Update() {
    TTF_CloseFont(font); // Dong font
    SDL_DestroyRenderer(renderer); // Huy renderer
    SDL_DestroyWindow(window); // Huy cua so
    TTF_Quit(); // Huy SDL_ttf
    SDL_Quit(); // Huy SDL
}


// Ham chay game
void Update::run() {
    while (running) { // Game chua ket thuc
        handleEvents(); // Xu ly su kien (nhap lieu, dong cua so, ...)
        if (!paused) { // Neu game khong bi tam dung
            Uint32 now = SDL_GetTicks(); // Lay thoi gian hien tai
            if (now - lastMoveTime > moveDelay) { // Neu thoi gian chay game da qua delay
                updateGame(); // Cap nhat trang thai game
                lastMoveTime = now; // Cap nhat thoi gian cuoi cung
            }
        }
        render(); // Ve man hinh
        SDL_Delay(1); // Tam dung 1 ms de giam tai trong
    }
}


// Ham xu ly su kien
void Update::handleEvents() {
    SDL_Event e; // Khai bao su kien
    while (SDL_PollEvent(&e)) { // Duyet qua danh sach su kien
        if (e.type == SDL_QUIT) { // Neu dong cua so
            running = false; // Dung game
        }
        else if (e.type == SDL_KEYDOWN) { // Neu nhan phim
            switch (e.key.keysym.sym) {
            case SDLK_UP:    if (dir != DOWN) dir = UP; break; // Neu nhan phim len va khong phai di chuyen xuong thi di len
            case SDLK_DOWN:  if (dir != UP) dir = DOWN; break; 
            case SDLK_LEFT:  if (dir != RIGHT) dir = LEFT; break; // Neu nhan phim trai va khong phai di chuyen sang phai thi di sang trai
            case SDLK_RIGHT: if (dir != LEFT) dir = RIGHT; break; 
            case SDLK_p:     paused = !paused; break; // Neu nhan phím P, game se dung hoac tiep tuc
            }
        }
    }
}


// Ham cap nhat trang thai game
void Update::updateGame() {
    Point head = snake.front(); // Lay dau cua con ran
    switch (dir) {
    case UP:    head.y--; break; // Neu di len thi giam y
    case DOWN:  head.y++; break; // Neu di xuong thi tang y
    case LEFT:  head.x--; break; // Neu di trai thi giam x
    case RIGHT: head.x++; break; // Neu di phai thi tang x
    }

    if (isWall(head) || isCollision(head)) { // Neu dau ran cham tuong hoac cham than
        std::cout << "Game Over! Final Score: " << score << "\n"; // In ra diem so khi ket thuc
        running = false; // Dung game
        return;
    }

    snake.push_front(head); // Them dau ran vao vi tri moi

    if (head == food) { // Neu dau ran an thuc an
        grow = true; // Ran dai them
        score++; // Tang diem
        spawnFood(); // Sinh thuc an moi
    }


    if (!grow) snake.pop_back(); // Neu khong can dau ra thi xoa duoi ran
    grow = false; // Dat lai trang thai grow

    // tang toc do game moi khi dat them 5 diem
    if (score % 5 == 0 && moveDelay > 50) {
        moveDelay -= 5;
    }

    if (score >= targetScore) { // Neu dat diem muc tieu
        std::cout << "You Win! Final Score: " << score << "\n"; // Thong bao thang game
        running = false; // Dung game
    }
}

// Kiem tra xem dau ran co cham tuong khong
bool Update::isWall(const Point& p) {
    for (const auto& wall : walls) { // Duyet qua danh sach tuong
        if (p == wall) return true; // Neu vi tri p trung voi tuong thi return true
    }
    return false; // Khong cham tuong
}

// Kiem tra xem dau ran co cham than ran khong
bool Update::isCollision(const Point& p) {
    for (size_t i = 1; i < snake.size(); ++i) { // Duyet qua cac phan cua than ran
        if (p == snake[i]) return true; // Neu vi tri p trung voi than ran thi return true
    }
    return false; // Khong cham than
}

// Tao tuong cho game
void Update::createWalls() {
    // Vien ngoai
    for (int x = 0; x < SCREEN_WIDTH / TILE_SIZE; ++x) {
        walls.push_back({ x, 0 }); // Tao tuong tren
        walls.push_back({ x, SCREEN_HEIGHT / TILE_SIZE - 1 }); // Tao tuong duoi
    }
    for (int y = 1; y < SCREEN_HEIGHT / TILE_SIZE - 1; ++y) {
        walls.push_back({ 0, y }); // Tao tuong trai
        walls.push_back({ SCREEN_WIDTH / TILE_SIZE - 1, y }); // Tao tuong phai
    }

    // Chuong ngai vat
    for (int x = 10; x <= 20; ++x) {
        walls.push_back({ x, 12 }); // Tao hang rao ngang
    }

    for (int y = 6; y <= 10; ++y) {
        walls.push_back({ 5, y }); // Tao hang rao doc
    }

    for (int x = 25; x <= 28; ++x) {
        for (int y = 7; y <= 9; ++y) {
            walls.push_back({ x, y }); // Tao chuong ngai vat
        }
    }
}

// Sinh thuc an moi
void Update::spawnFood() {
    Point p;
    do {
        p.x = rand() % (SCREEN_WIDTH / TILE_SIZE - 2) + 1; // Sinh thuc an ngau nhien
        p.y = rand() % (SCREEN_HEIGHT / TILE_SIZE - 2) + 1; // Sinh thuc an ngau nhien
    } while (isWall(p) || isCollision(p)); // Neu thuc an trong tuong hoac cham ran thi sinh lai

    food = p; // Gan vi tri thuc an moi
}

// Ve man hinh game
void Update::render() {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Set mau nen
    SDL_RenderClear(renderer); // Xoa man hinh

    // Ve tuong
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (const auto& wall : walls) {
        SDL_Rect rect = { wall.x * TILE_SIZE, wall.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        SDL_RenderFillRect(renderer, &rect);
    }

    // Ve thuc an
    SDL_Rect foodRect = { food.x * TILE_SIZE, food.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &foodRect);

    // Ve ran
    for (size_t i = 0; i < snake.size(); ++i) {
        SDL_Rect r = { snake[i].x * TILE_SIZE, snake[i].y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        if (i == 0) SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); // Ve dau ran
        else SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Ve than ran
        SDL_RenderFillRect(renderer, &r);
    }

    // Hien thi diem so
    renderText(("Score: " + std::to_string(score)).c_str(), 10, 10);

    SDL_RenderPresent(renderer); // Cap nhat man hinh
}

// Ham ve text (diem so)
void Update::renderText(const char* text, int x, int y) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, SDL_Color{ 255, 255, 255 });
    if (!surface) return; // Neu khong tao duoc surface thi thoat
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); // Tao texture tu surface
    SDL_Rect dstRect = { x, y, surface->w, surface->h }; // Xac dinh vi tri de ve text
    SDL_RenderCopy(renderer, texture, NULL, &dstRect); // Ve text
    SDL_FreeSurface(surface); // Huy surface
    SDL_DestroyTexture(texture); // Huy texture
}
