#include "raylib.h"
#include <deque>
#include <raymath.h>

// Screen dimensions and game constants
const int screenWidth = 800;
const int screenHeight = 600;
const int cellSize = 20;
const int cellCountX = screenWidth / cellSize;
const int cellCountY = screenHeight / cellSize;
const double updateTime = 0.2; // Time in seconds between snake moves
Sound item_collected;
Sound game_over;
Music backgound;
struct Snake {
    std::deque<Vector2> body;
    Vector2 direction;
    bool isAlive;
};
struct Food {
    Vector2 position;
};

Snake snake;
Food food;
double lastUpdateTime = 0;
int score = 0;

bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

Vector2 generateRandomPos() {
    float x = GetRandomValue(0, cellCountX - 1);
    float y = GetRandomValue(0, cellCountY - 1);
    return (Vector2){ x, y };
}

void InitGame() {
    snake.body = { (Vector2){ 5, 10 }, (Vector2){ 6, 10 }, (Vector2){ 7, 10 } };
    snake.direction = { 1, 0 };
    snake.isAlive = true;
    food.position = generateRandomPos();
    score = 0;
}

void UpdateGame() {
    if (snake.isAlive) {
        if (IsKeyPressed(KEY_UP) && snake.direction.y != 1) 
        {
            snake.direction = { 0, -1 };
        } 
        else if (IsKeyPressed(KEY_DOWN) && snake.direction.y != -1) 
        {
            snake.direction = { 0, 1 };
        } 
        else if (IsKeyPressed(KEY_LEFT) && snake.direction.x != 1) 
        {
            snake.direction = { -1, 0 };
        } 
        else if (IsKeyPressed(KEY_RIGHT) && snake.direction.x != -1) 
        {
            snake.direction = { 1, 0 };
        }
        if (eventTriggered(updateTime)) 
        {
            Vector2 newHead = Vector2Add(snake.body.back(), snake.direction);
            snake.body.push_back(newHead);

            if (Vector2Equals(newHead, food.position)) 
            {
                PlaySound(item_collected);
                score++;
                food.position = generateRandomPos();
            } 
            else 
            {
                // Remove the tail if no food was eaten
                snake.body.pop_front();
            }
            if (newHead.x < 0 || newHead.x >= cellCountX || newHead.y < 0 || newHead.y >= cellCountY) 
            {
                snake.isAlive = false;
                PlaySound(game_over);
            }
            for (size_t i = 0; i < snake.body.size() - 1; i++) 
            {
                if (Vector2Equals(newHead, snake.body[i])) 
                {
                    snake.isAlive = false;
                    PlaySound(game_over);
                    break;
                }
            }
        }
    }
}

void DrawGame() {
    BeginDrawing();
    ClearBackground(BLACK);
    for (int i = 0; i < cellCountX; i++)
    {
        for (int j = 0; j < cellCountY; j++)
        {
            DrawRectangleLines(i * cellSize, j * cellSize, cellSize, cellSize, GRAY);
        }
    }
    DrawRectangle(food.position.x * cellSize, food.position.y * cellSize, cellSize, cellSize, RED);
    for (size_t i = 0; i < snake.body.size(); i++)
    {
        DrawRectangle(snake.body[i].x * cellSize, snake.body[i].y * cellSize, cellSize, cellSize, DARKGREEN);
    }
    DrawText(TextFormat("Score: %i", score), 10, 10, 20, WHITE);

    if (!snake.isAlive) 
    {
        DrawText("GAME OVER! Press R to restart", screenWidth / 2 - MeasureText("GAME OVER! Press R to restart", 30) / 2, screenHeight / 2, 30, YELLOW);
        if (IsKeyPressed(KEY_R)) {
            InitGame();
        }
    }
    EndDrawing();
}

int main() {
    InitWindow(screenWidth, screenHeight, "Snake Game");
    SetTargetFPS(60);
    InitAudioDevice();
    item_collected= LoadSound("music/item_collected.mp3");
    game_over= LoadSound("music/game_over.mp3");
    backgound=LoadMusicStream("music/background.mp3");
    InitGame();
    PlayMusicStream(backgound);
    while (!WindowShouldClose()) 
    {
        UpdateMusicStream(backgound);
        UpdateGame();
        DrawGame();
    }
    UnloadMusicStream(backgound);
    UnloadSound(game_over);
    UnloadSound(item_collected);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}