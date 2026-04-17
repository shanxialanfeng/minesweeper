#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>

using namespace std;

const int WIDTH = 9;
const int HEIGHT = 9;
const int MINES = 10;
const int TotalSafe = WIDTH * HEIGHT - MINES;
int RevealedSafe = 0;

class Tile
{
    private:
    int around; //-1表示地雷，0-8表示周围地雷数量
    bool revealed;
    bool flagged;

    public:
    Tile() //构造函数初始化
    {
        around = 0;
        revealed = false;
        flagged = false;
    }
    
    bool isRevealed() const { return revealed; }
    bool isFlagged() const { return flagged; }
    bool isMine() const
    {
        if (around == -1) return true;
        else return false;
    }
    int getAround() const { return around; }
    void reveal() { revealed = true; }
    void toggleFlag() { flagged = !flagged; }
    char getDisplayChar() const
    {
        if (!revealed) return flagged ? 'F' : '#';
        if (isMine()) return '*';
        if (getAround() == 0) return ' ';
        return '0' + getAround(); // 将数字转换为字符
    }
    void setMine() { around = -1; }
    void incrementAround() { around++; }
};

Tile board[WIDTH][HEIGHT];

void BoardInit()//初始化棋盘
{
    //1、随机放置地雷，并更新周围格子的数字
    int count = 0;
    while(count < MINES){
        int x = rand() % WIDTH; // 生成 0 到 WIDTH-1 之间的随机数
        int y = rand() % HEIGHT;
        if(!board[x][y].isMine())
        {
            board[x][y].setMine();
            count++;
            for(int dx = -1; dx <= 1; dx++)
            {
                for(int dy = -1; dy <= 1; dy++)
                {
                    int nx = x + dx, ny = y + dy;
                    if(dx == 0 && dy == 0) continue;
                    if(nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT)
                    {
                        if(!board[nx][ny].isMine())
                            board[nx][ny].incrementAround();
                    }
                }
            }
        }
    }
}

//翻开(x,y)位置的格子, isRoot = true表示这是玩家翻开的格子，false表示递归翻开的格子; Reveal返回false表示踩雷
bool Reveal(int x, int y, bool isRoot = true) 
{
    if(x<0 || x>= WIDTH || y<0 ||y>= HEIGHT) return true;//越界检查
    Tile &tile = board[x][y];
    if(tile.isRevealed() || tile.isFlagged()) return true;//已经翻开或标记过的格子不处理

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

bool ToggleFlag(int x, int y)
{
    if(x<0 || x>= WIDTH || y<0 ||y>= HEIGHT) return false;//越界检查
    Tile &tile = board[x][y];
    if (tile.isRevealed()) {
        cout << "已翻开的格子不能插旗！" << endl;
        return false;
    }
    tile.toggleFlag();
    if (tile.isFlagged())
        cout << "已标记 (" << x << "," << y << ") 为雷区" << endl;
    else
        cout << "已取消标记 (" << x << "," << y << ")" << endl;
    return true;
}

void DisplayBoard()//棋盘可视化
{
    cout << " ";
    for (int x = 0; x < WIDTH; x++) {
        cout << " " << x;
    }
    cout << endl;

    for(int y = 0; y < HEIGHT; y++)
    {
        cout << y <<" ";
        for(int x = 0; x < WIDTH; x++)
        {
            Tile &tile = board[x][y];
            cout<< tile.getDisplayChar() << " ";  
        }
        cout << endl;
    }
    cout << endl;
}

// 处理输入失败：清除错误标志、忽略剩余输入、输出错误信息，返回 true 表示失败
bool handleInputFailure(const string& errorMsg = "无效输入！") {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << errorMsg << endl;
        return true;
    }
    return false;
}

int main()
{
    srand(time(0));
    BoardInit();

    while(true)
    {
        DisplayBoard();
        cout<<"请输入命令: (坐标 x y 翻开) 或 (f x y 插旗/取消旗子): ";
        string command;
        cin >> command;
        if (handleInputFailure()) continue;

        // 标记模式
        if (command == "f") {
            int fx, fy;
            cout << "标记模式：请输入要切换旗子的坐标(x y): ";
            cin >> fx >> fy;
            if (handleInputFailure("无效坐标！")) continue;
            ToggleFlag(fx, fy);
            continue;  // 回到循环开头，不进行翻开逻辑
        }
        else{
            int mx, my;
            mx = stoi(command); // 将第一个输入的命令转换为整数坐标
            cin >> my;
            if (handleInputFailure("无效坐标！")) continue;

            if(mx < 0 || mx >= WIDTH || my < 0 || my >= HEIGHT){
                cout <<"无效坐标，请重新输入！" << endl;
                continue;
            }

            bool hitMine = !Reveal(mx, my);
            if(hitMine){
                cout << "Game Over!" << endl;
                for(int x = 0; x < WIDTH; x++)
                    for(int y = 0; y < HEIGHT; y++)
                        board[x][y].reveal();//翻开所有格子显示地雷位置
                DisplayBoard();
                break;
            }

            if(RevealedSafe == TotalSafe)
            {
                cout << "你赢了！" << endl;
                for(int x = 0; x < WIDTH; x++)
                    for(int y = 0; y < HEIGHT; y++)
                        board[x][y].reveal();//翻开所有格子显示地雷位置
                DisplayBoard();
                break;
            }
        }
    }
}