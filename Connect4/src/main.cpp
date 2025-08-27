#include <raylib.h>
#include<iostream>
#include<vector>
#include<string>
#include<array>
class Tile
{
private:
    int number{0};
public:
    Tile()=default;
    Tile(const int& a):number{a}{
    };
    int getNum();
    bool isEmpty();
    friend bool operator==(const Tile&a,const Tile&b);
};
bool operator==(const Tile &a, const Tile &b)
{
    return (a.number==b.number);
}
int Tile::getNum()
{
    return number;
}
bool Tile::isEmpty()
{
    return (!number);
}
class Direction
{
private:
    
public:
    std::string dierction{"No Direction Selected"};
    Direction(const int&a)
    {
        switch (a)
        {
        case KEY_UP:
            dierction="up";
            break;
        case KEY_LEFT:
            dierction="left";
            break;
        case KEY_DOWN:
            dierction="down";
            break;
        case KEY_RIGHT:
            dierction="right";
            break;
        default:
            break;
        }
    };
    Direction(const std::string&a):dierction{a}{};
    friend std::ostream& operator<<(std::ostream&out,const Direction&a);
    Direction operator!()const{
        if (dierction=="up")
        {
            return Direction{KEY_DOWN};
        }
        if (dierction=="down")
        {
            return Direction{KEY_UP};
        }
        if (dierction=="left")
        {
            return Direction{KEY_RIGHT};
        }
        if (dierction=="right")
        {
            return Direction{KEY_LEFT};
        }
        return *this;
    }
};
class Point
{
private:
    
public:
    int x{0};
    int y{0};
    Point(const int&a,const int&b):x{a},y{b}{
    };
    friend bool operator==(const Point&a,const Point&b);
    friend bool operator!=(const Point&a,const Point&b);
    Point getAdjacentPoint(const Direction&a);
    friend Point operator+(const Point&a,const Point&b);
};
bool operator==(const Point &a, const Point &b)
{
    return (a.x==b.x)&&(a.y==b.y);
}
bool operator!=(const Point &a, const Point &b)
{
    return !(a==b);
}
Point operator+(const Point &a, const Point &b)
{
    return Point{a.x+b.x,a.y+b.y};
}
Point Point::getAdjacentPoint(const Direction &a)
{
    if (a.dierction=="up")
    {
        return *this+Point{-1,0};
    }
    if (a.dierction=="right")
    {
        return *this+Point{0,1};
    }
    if (a.dierction=="down")
    {
        return *this+Point{1,0};
    }
    if (a.dierction=="left")
    {
        return *this+Point{0,-1};
    }
    return *this;
}
class Game
{
private:
    std::array<std::array<Tile,4>,4>grid;
public:
    std::vector<Direction>main_directions{{"up"},{"down"},{"left"},{"right"}};
    std::vector<Color> colors{BLACK,YELLOW};
    Point empty_tile_location{3,3};
    Game();
    ~Game();
    friend std::ostream &operator<<(std::ostream &out, const Game &a);
    bool moveTile(const Direction &a);
    void randomize();
    friend bool operator==(const Game&a,const Game&b);
    void playerWon();
    void Not_solved_text();
    void restart();
    void Initialize_Board();
    void Draw_grid();
    bool is_winner;
    Music music;
    Sound SlideSound;
    Sound WiningSound;
};
void Game::Initialize_Board()
{
    empty_tile_location=Point(3,3);
    int counter=0;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            counter++;
            grid[i][j]=Tile(counter%16);   
        }
    }
}
void Game::Draw_grid()
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            int num = grid[i][j].getNum();

            if (num == 0) 
            {
                continue;
            }

            DrawRectangle(100 * j + 10, 100 * i + 10, 90, 90, RAYWHITE);
            DrawRectangleLines(100 * j + 10, 100 * i + 10, 90, 90, GOLD);

            char buffer[4];
            sprintf(buffer, "%d", num);

            int fontSize = 40;
            int textWidth = MeasureText(buffer, fontSize);
            int posX = 100 * j + 10 + (90 - textWidth) / 2;
            int posY = 100 * i + 10 + (90 - fontSize) / 2;

            DrawText(buffer, posX, posY, fontSize, GOLD);
        }
    }
}
Game::Game()
{
    Initialize_Board();
    is_winner=false;
    music = LoadMusicStream("Sounds/background3.mp3");
    PlayMusicStream(music);
    SlideSound = LoadSound("Sounds/slide.mp3");
    WiningSound = LoadSound("Sounds/victory2.mp3");
}
Game::~Game()
{
    UnloadSound(SlideSound);
    UnloadSound(WiningSound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}
bool operator==(const Game &a, const Game &b)
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            if (!(a.grid[i][j]==b.grid[i][j]))
            {
                return false;
            }
        }
    }
    return true;
}
bool Game::moveTile(const Direction &a)
{
    Point target = empty_tile_location.getAdjacentPoint(a);
    if (target.x < 0 || target.x >= 4 || target.y < 0 || target.y >= 4)
    {
        return false; // Invalid move
    }
    Tile temp = grid[target.x][target.y];
    grid[target.x][target.y] = grid[empty_tile_location.x][empty_tile_location.y];
    grid[empty_tile_location.x][empty_tile_location.y] = temp;
    empty_tile_location = target;
    return true;
}
void Game::randomize()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    for (size_t i = 0; i < 1000; i++)
    {
        moveTile(main_directions[rand()%4]);
    }
}
void Game::playerWon()
{
    // Draw the white background rectangle
    DrawRectangle(475, 20, 150, 150, WHITE);

    const char* winText = "WON!!!";
    int fontSize = 40;

    int textWidth = MeasureText(winText, fontSize);
    int textHeight = fontSize;

    int rectX = 425;
    int rectY = 20;
    int rectWidth = 250;
    int rectHeight = 150;

    int textPosX = rectX + (rectWidth - textWidth) / 2;
    int textPosY = rectY + (rectHeight - textHeight) / 2;

    DrawText(winText, textPosX, textPosY, fontSize, GOLD);
    int restartRectX = 425;
    int restartRectY = rectY + rectHeight;
    int restartRectWidth = 250;
    int restartRectHeight = 150;

    DrawRectangle(restartRectX, restartRectY, restartRectWidth, restartRectHeight, WHITE);

    const char* restartText = "To restart\npress R";
    int restartFontSize = 35;

    int restartTextWidth = MeasureText(restartText, restartFontSize);
    int restartTextPosX = restartRectX + (restartRectWidth - restartTextWidth) / 2;
    int restartTextPosY = restartRectY + (restartRectHeight - restartFontSize) / 2 - 10; // Adjusted for a two-line message

    DrawText(restartText, restartTextPosX, restartTextPosY, restartFontSize, GOLD);
}
void Game::Not_solved_text(){
    // Draw the white background rectangle
    DrawRectangle(475, 20, 150, 150, WHITE);

    const char* winText = "TRY!!!";
    int fontSize = 40;

    int textWidth = MeasureText(winText, fontSize);
    int textHeight = fontSize;

    int rectX = 425;
    int rectY = 20;
    int rectWidth = 250;
    int rectHeight = 150;

    int textPosX = rectX + (rectWidth - textWidth) / 2;
    int textPosY = rectY + (rectHeight - textHeight) / 2;

    DrawText(winText, textPosX, textPosY, fontSize, GOLD);
    int restartRectX = 425;
    int restartRectY = rectY + rectHeight;
    int restartRectWidth = 250;
    int restartRectHeight = 150;

    DrawRectangle(restartRectX, restartRectY, restartRectWidth, restartRectHeight, WHITE);

    const char* restartText = "To Quit\npress Q\nR to restart ";
    int restartFontSize = 35;

    int restartTextWidth = MeasureText(restartText, restartFontSize);
    int restartTextPosX = restartRectX + (restartRectWidth - restartTextWidth) / 2;
    int restartTextPosY = restartRectY + (restartRectHeight - restartFontSize) / 2 -30; // Adjusted for a two-line message

    DrawText(restartText, restartTextPosX, restartTextPosY, restartFontSize, GOLD);
}
void Game::restart(){
    Initialize_Board();
    is_winner=false;
    randomize();
}
namespace UserInput
{
    bool validInput(const int &a)
    {
        return (a == KEY_UP) || (a == KEY_LEFT) || (a == KEY_DOWN) || (a == KEY_RIGHT) || (a == KEY_Q) || (a == KEY_R);
    }
    int getCommandFromUser()
    {
        int a=GetKeyPressed();
        return a;
    }
}
int main() 
{    
    
    InitWindow(700,400,"puzzel 15");// for the window size
    InitAudioDevice();
    SetTargetFPS(60);//defining the fps 

    Game game {};
    game.randomize();
    Game win {};

    while (WindowShouldClose()== false)
    {   
        UpdateMusicStream(game.music);
        BeginDrawing();
        ClearBackground(BLACK);
        game.Draw_grid();
        if (game.is_winner)
        {
            game.playerWon();
            if (GetKeyPressed()==KEY_R)
            {
                game.restart();
            }
        }
        else
        {
            game.Not_solved_text();
            int input=UserInput::getCommandFromUser();
            if (UserInput::validInput(input))
            {
                if (input==KEY_Q)
                {
                    CloseWindow();
                    return 0;
                }
                else if (input==KEY_R)
                {
                    game.restart();
                }
                else
                {
                    PlaySound(game.SlideSound);
                    game.moveTile(!Direction(input));
                    if (game==win)
                    {
                        PlaySound(game.WiningSound);
                        game.is_winner=true;
                    }
                }
            }
        }
        EndDrawing();
    }
    CloseWindow();
}