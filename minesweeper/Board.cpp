#include<iostream>
#include<random>
#include<vector>
#include<chrono> // 用于生成随机数的时间种子
#include"Board.hpp"
using namespace std;

Board::Board(int h, int w, int m): height(h), width(w), mines(m), TotalSafe(h * w - m), RevealedSafe(0), FlaggedMines(0) {
    board.resize(width, vector<Tile>(height)); // 初始化棋盘
}

void Board::PlaceMinesAvoiding(int firstX, int firstY)
{
    // 1. 重置棋盘（清除上一局的所有雷和数字）
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            board[x][y] = Tile();

    // 2. 随机布雷，但避开 firstX, firstY 及其周围8格
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    int count = 0;
    while (count < mines) {
        std::uniform_int_distribution<int> distX(0, width - 1);
        std::uniform_int_distribution<int> distY(0, height - 1);
        int x = distX(rng);
        int y = distY(rng);
        
        // 检查是否在禁止区域（第一步点击的格子及其周围8格）
        bool isForbidden = false;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (x == firstX + dx && y == firstY + dy) {
                    isForbidden = true;
                    break;
                }
            }
            if (isForbidden) break;
        }
        if (isForbidden || board[x][y].isMine()) continue;

        board[x][y].setMine();
        count++;

        // 更新周围8格子的 around 值
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    if (!board[nx][ny].isMine())
                        board[nx][ny].incrementAround();
                }
            }
        }
    }
}

//翻开(x,y)位置的格子, isRoot = true表示这是玩家翻开的格子，false表示递归翻开的格子; Reveal返回false表示踩雷
bool Board::Reveal(int x, int y, bool isRoot)
{
    if (x < 0 || x >= width || y < 0 || y >= height) {
        if (isRoot) cout << "坐标越界，请重新输入！" << endl;
        return true;  // 越界不视为踩雷
    }

    Tile& tile = board[x][y];

    if (tile.isRevealed()) {
        if (isRoot) cout << "该格子已经翻开了！" << endl;
        return true;
    }

    if (tile.isFlagged()) {
        if (isRoot) cout << "该格子已被标记为雷，请先取消标记！" << endl;
        return true;
    }

    if(tile.isMine()){
        if(isRoot) return false;
        else return true;
    }

    tile.reveal();
    RevealedSafe++;

    if(tile.getAround() == 0)//  空白格递归翻开周围的格子
    {
        for(int dy = -1; dy <= 1; dy++)
        {
            for(int dx = -1; dx <= 1; dx++)
            {
                if(dx == 0 && dy == 0) continue;
                Reveal(x + dx, y + dy, false);
            }
        }    
    }
    return true;
}

bool Board::ToggleFlag(int x, int y)
{
    if(x<0 || x>= width || y<0 ||y>= height) return false;//越界检查
    Tile &tile = board[x][y];
    if (tile.isRevealed()) {
        cout << "已翻开的格子不能插旗！" << endl;
        return false;
    }
    tile.toggleFlag();
    if (tile.isFlagged()){
        if(tile.isMine()) FlaggedMines++;
        cout << "已标记 (" << x << "," << y << ") 为雷区" << endl;
    }else{
        if(tile.isMine()) FlaggedMines--;
        cout << "已取消标记 (" << x << "," << y << ")" << endl;
    }
    cout<< "当前已标记的雷数: " << FlaggedMines << "/" << mines << endl;
    return true;
}

void Board::DisplayBoard()//棋盘可视化
{
    cout << " ";
    for (int x = 0; x < width; x++) {
        cout << " " << x;
    }
    cout << endl;

    for(int y = 0; y < height; y++)
    {
        cout << y <<" ";
        for(int x = 0; x < width; x++)
        {
            Tile &tile = board[x][y];
            cout<< tile.getDisplayChar() << " ";  
        }
        cout << endl;
    }
    cout << endl;
}

void Board::RevealAll(){
    for(int x = 0; x < width; x++)
        for(int y = 0; y < height; y++)
            board[x][y].reveal();//翻开所有格子显示地雷位置
}

void Board::Win(){
    cout << "你赢了！" << endl;
    RevealAll();
    DisplayBoard();
}

int Board::getRevealedSafe() const { return RevealedSafe; }
int Board::getFlaggedMines() const { return FlaggedMines; }
int Board::getHeight() const { return height; }
int Board::getWidth() const { return width; }
int Board::getMines() const { return mines; }
int Board::getTotalSafe() const { return TotalSafe; }