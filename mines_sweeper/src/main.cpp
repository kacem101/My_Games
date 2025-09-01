#include<iostream>
#include<raylib.h>
#include<array>
#include<vector>
const int Number_of_rows=16;
const int Number_of_cols=18;
const int Max_Bomb_number=30;
class Tile
{
private:
    int number_of_adjacent_bombs{0};
	bool open{false};
	bool bomb{false};
	bool flaged{false};
public:
    Tile()=default;
    Tile(const int& a):number_of_adjacent_bombs{a}{};
	Tile(const int&a,const bool&b):number_of_adjacent_bombs{a},open{b}{};
    int getNumOfAjacentBomb();
	void SetBomb(const bool&a){bomb=a;}
	bool GetBomb(){return bomb;}
	void SetNumberOfBombs(const int&a){number_of_adjacent_bombs=a;}
	bool GetOpen(){return open;}
	void SetOpen(const bool&a){open=a;}
	void SetFlaged(const bool&a){flaged=a;}
	bool GetFlaged(){return flaged;}
    friend bool operator==(const Tile&a,const Tile&b);
};
bool operator==(const Tile &a, const Tile &b)
{
    return (a.number_of_adjacent_bombs==b.number_of_adjacent_bombs
			&&a.bomb==b.bomb
			&&a.open==b.open);
}
int Tile::getNumOfAjacentBomb()
{
    return number_of_adjacent_bombs;
}
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
    std::vector<Point> getAdjacentPoint();
    friend Point operator+(const Point&a,const Point&b);
	bool isValidPoint();
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
std::vector<Point> Point::getAdjacentPoint()
{
    std::vector<Point> result;
    int dx[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int dy[] = {-1, -1, -1, 0, 0, 1, 1, 1};

    for (int i = 0; i < 8; ++i) {
        Point adjacentPoint = *this + Point(dx[i], dy[i]);
        if (adjacentPoint.isValidPoint()) {
            result.push_back(adjacentPoint);
        }
    }
    return result;
}
bool Point::isValidPoint()
{
    return x>=0&&x<Number_of_rows&&y>=0&&y<Number_of_cols;
}
class Game
{
private:
	std::array<std::array<Tile,Number_of_cols>,Number_of_rows>grid;
	bool validindex(const int&a,const int&b){return a>=0&&a<Number_of_rows&&b>=0&&b<Number_of_cols;};
	int bomb_counter=0;
	int flag_counter=0;
	bool is_winner;
	bool is_lost;
public:
    std::vector<Color> colors{BLACK,YELLOW,WHITE};
	void inintailize_grid();
	void DrawGrid();
	bool Handle_user_input();
    void OpenAdjacentTiles(int row, int col);
	void checkwin();
	void checklost();
    void userWon();
    bool GetIsWinner() { return is_winner; }
    bool GetIsLost(){return is_lost;}
    void userLost();
    void DrawNormalState();
    void restart();
	void showGrid();
	Music backGround;
	Sound BombExplosion;
	Sound winningSound;
	Sound openingTile;
	Sound losing;
    Game();
    ~Game();
};

void Game::inintailize_grid()
{
	for (size_t i = 0; i < Number_of_rows; i++)
	{
		for (size_t j = 0; j<Number_of_cols; j++)
		{
			grid[i][j]=Tile(0,false);
		}
	}
	for (size_t i = 0; i < Max_Bomb_number; i++)
	{
		int row=rand()%Number_of_rows,col=rand()%Number_of_cols;
		if (grid[row][col].GetBomb())
		{
			continue;
		}
		bomb_counter++;
		grid[row][col].SetBomb(true);
		std::vector<Point>res=Point(row,col).getAdjacentPoint();
		for (auto &&i : res)
		{
			grid[i.x][i.y].SetNumberOfBombs(grid[i.x][i.y].getNumOfAjacentBomb()+1);
		}
	}
}

void Game::DrawGrid()
{
    std::vector<Color> numberColors = {
        WHITE,      
        BLUE,       
        GREEN,      
        RED,        
        VIOLET,     
        ORANGE,     
        BROWN,      
        DARKGRAY,   
        BLACK       
    };

    for (int i = 0; i < Number_of_rows; ++i) {
        for (int j = 0; j < Number_of_cols; ++j) {
            int tileSize = 40;
            int padding = 2;
            int x = j * (tileSize + padding);
            int y = i * (tileSize + padding);
            
            Tile currentTile = grid[i][j];

            if (currentTile.GetOpen()) 
			{

                if (currentTile.GetBomb()) 
				{

                    DrawRectangle(x, y, tileSize, tileSize, RED);
                    DrawCircle(x + tileSize / 2, y + tileSize / 2, tileSize / 4, BLACK);
                } 
				else 
				{
                    DrawRectangle(x, y, tileSize, tileSize, GRAY);
                    int bombCount = currentTile.getNumOfAjacentBomb();
                    if (bombCount > 0) {
                        Color numberColor = numberColors[bombCount];
                        std::string numberText = std::to_string(bombCount);
                        Vector2 textPosition = { (float)(x + tileSize / 2 - MeasureText(numberText.c_str(), 20) / 2), (float)(y + tileSize / 2 - 10) };
                        DrawText(numberText.c_str(), textPosition.x, textPosition.y, 30, numberColor);
                    }
                }
            } 
			else 
			{
                DrawRectangle(x, y, tileSize, tileSize, DARKGRAY);
				if (currentTile.GetFlaged()) 
                {
                    DrawRectangle(x + tileSize * 0.7f, y + tileSize * 0.2f, tileSize * 0.1f, tileSize * 0.6f, BLACK);
                    DrawRectangle(x + tileSize * 0.4f, y + tileSize * 0.2f, tileSize * 0.3f, tileSize * 0.3f, RED);
                    DrawRectangle(x + tileSize * 0.65f, y + tileSize * 0.8f, tileSize * 0.2f, tileSize * 0.1f, BLACK);
                }
            }
        }
    }
}

bool Game::Handle_user_input() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
	{
        Vector2 click = GetMousePosition();
        int tileSize = 40; // The same value used in DrawGrid()
        int padding = 2; // The same value used in DrawGrid()
        int col = (int)click.x / (tileSize + padding);
        int row = (int)click.y / (tileSize + padding);

        if (validindex(row, col)) 
		{
            if (!grid[row][col].GetOpen()) 
			{
                grid[row][col].SetOpen(true);
                if (grid[row][col].getNumOfAjacentBomb() == 0) 
				{
                    OpenAdjacentTiles(row, col);
                }
                return true;
            }
        }
		else
		{
			return false;
		}
    }
	else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		Vector2 click = GetMousePosition();
		int tileSize = 40; // The same value used in DrawGrid()
        int padding = 2; // The same value used in DrawGrid()
        int col = (int)click.x / (tileSize + padding);
        int row = (int)click.y / (tileSize + padding);
        
        if (validindex(row, col)) 
		{
            // Only allow flagging if the tile is not already open
            if (!grid[row][col].GetOpen()) 
			{
                bool isFlagged = grid[row][col].GetFlaged();
				if (!isFlagged&&flag_counter>=bomb_counter)
				{
					return false;
				}
				isFlagged ? flag_counter--:flag_counter++;
                grid[row][col].SetFlaged(!isFlagged);
				return true;
            }
			else
			{
				return false;
			}
        }
		else
		{
			return false;
		}
	}
    return false;
}
void Game::OpenAdjacentTiles(int row, int col) {
    grid[row][col].SetOpen(true);
    std::vector<Point> adjacentPoints = Point(row, col).getAdjacentPoint();
    for (const auto& point : adjacentPoints) 
	{
        if (!grid[point.x][point.y].GetOpen()) 
		{
            grid[point.x][point.y].SetOpen(true);
            if (grid[point.x][point.y].getNumOfAjacentBomb() == 0) 
			{
                OpenAdjacentTiles(point.x, point.y);
            }
        }
    }
}
void Game::checkwin()
{
	for (size_t i = 0; i < Number_of_rows; i++)
	{
		for (size_t j = 0; j<Number_of_cols; j++)
		{
			if (!grid[i][j].GetFlaged()&&grid[i][j].GetBomb())
			{
				is_winner=false;
				return;
			}
		}
	}
	is_winner=true;
}
void Game::checklost()
{
	for (size_t i = 0; i < Number_of_rows; i++)
	{
		for (size_t j = 0; j<Number_of_cols; j++)
		{
			if ((grid[i][j].GetOpen()&&grid[i][j].GetBomb()))
			{
				is_lost=true;
				return;
			}
		}
	}
	is_lost=false;
}
void Game::userWon()
{
    // Background panel on the right side
    int gridWidth = Number_of_cols * (40 + 2);
    int panelX = gridWidth;
    int panelWidth = GetScreenWidth() - gridWidth;
    DrawRectangle(panelX, 0, panelWidth, GetScreenHeight(), Fade(GREEN, 0.4f));

    // Main message
    const char* msg = "YOU WON!";
    int fontSize = 50;
    int textWidth = MeasureText(msg, fontSize);
    DrawText(msg, panelX + (panelWidth - textWidth)/2, GetScreenHeight()/2 - 60, fontSize, WHITE);

    // Restart instruction
    const char* restartMsg = "Press R to restart";
    int restartSize = 25;
    int restartWidth = MeasureText(restartMsg, restartSize);
    DrawText(restartMsg, panelX + (panelWidth - restartWidth)/2, GetScreenHeight()/2 + 10, restartSize, YELLOW);
}

void Game::userLost()
{

    int gridWidth = Number_of_cols * (40 + 2);
    int panelX = gridWidth;
    int panelWidth = GetScreenWidth() - gridWidth;
    DrawRectangle(panelX, 0, panelWidth, GetScreenHeight(), Fade(RED, 0.4f));

    const char* msg = "YOU LOST!";
    int fontSize = 50;
    int textWidth = MeasureText(msg, fontSize);
    DrawText(msg, panelX + (panelWidth - textWidth)/2, GetScreenHeight()/2 - 60, fontSize, WHITE);

    const char* restartMsg = "Press R to restart";
    int restartSize = 25;
    int restartWidth = MeasureText(restartMsg, restartSize);
    DrawText(restartMsg, panelX + (panelWidth - restartWidth)/2, GetScreenHeight()/2 + 10, restartSize, YELLOW);
}
void Game::DrawNormalState()
{
    int gridWidth = Number_of_cols * (40 + 2);
    int panelX = gridWidth;
    int panelWidth = GetScreenWidth() - gridWidth;

    DrawRectangle(panelX, 0, panelWidth, GetScreenHeight(), Fade(BLUE, 0.3f));

    const char* msg = "Try harder!\nflag all the\n bombs to win";
    int fontSize = 40;
    int textWidth = MeasureText(msg, fontSize);
    DrawText(msg, panelX + (panelWidth - textWidth)/2, GetScreenHeight()/2 - 120, fontSize, WHITE);

    const char* restartMsg = "Press R to restart";
    int restartSize = 20;
    int restartWidth = MeasureText(restartMsg, restartSize);
    DrawText(restartMsg, panelX + (panelWidth - restartWidth)/2, GetScreenHeight()/2 + 10, restartSize, YELLOW);
}
void Game::restart()
{
	inintailize_grid();
	is_lost=false;
	is_winner=false;
}
void Game::showGrid()
{
	for (size_t i = 0; i < Number_of_rows; i++)
	{
		for (size_t j = 0; j<Number_of_cols; j++)
		{
			grid[i][j].SetOpen(true);
		}
	}
}
Game::Game()
{
	inintailize_grid();
	is_winner=false;
	is_lost=false;
	backGround=LoadMusicStream("sound/background.mp3");
	PlayMusicStream(backGround);
	winningSound=LoadSound("sound/winingSound.mp3");
	BombExplosion=LoadSound("sound/exploision.mp3");
	openingTile=LoadSound("actiondone.mp3");
	losing=LoadSound("sound/gameLost.mp3");
}

Game::~Game()
{
	UnloadSound(winningSound);
	UnloadSound(losing);
	UnloadSound(BombExplosion);
	UnloadSound(openingTile);
	UnloadMusicStream(backGround);
	CloseAudioDevice();
}

int main()
{
	InitWindow(1200,670,"mines sweeper");
	SetTargetFPS(60);
	InitAudioDevice();
	Game game{};
	while (!WindowShouldClose())
	{
		BeginDrawing();
		game.DrawGrid();
		if (game.GetIsWinner())
		{
			game.userWon();
			if(IsKeyPressed(KEY_R))
			{
				game.restart();
			}
		}
		else if (game.GetIsLost())
		{
			game.userLost();
			game.showGrid();
			if(IsKeyPressed(KEY_R))
			{
				game.restart();
			}
		}
		else
		{
			UpdateMusicStream(game.backGround);
			game.DrawNormalState();
			if(game.Handle_user_input())
			{
				PlaySound(game.openingTile);
				game.checkwin();
				if (game.GetIsWinner())
				{
					PlaySound(game.winningSound);
				}
				game.checklost();
				if (game.GetIsLost())
				{
					PlaySound(game.BombExplosion);
					PlaySound(game.losing);
				}
			}
			if (IsKeyPressed(KEY_R))
			{
				game.restart();
			}
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}