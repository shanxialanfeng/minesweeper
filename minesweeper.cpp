#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
#include<vector>
using namespace std;

const int WIDTH = 9;
const int HEIGHT = 9;
const int MINES = 10;
const int TotalSafe = WIDTH * HEIGHT - MINES;
int RevealedSafe = 0;
int FlaggedMines = 0;

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
    
    bool isRevealed() const { return revealed; }//是否翻开
    bool isFlagged() const { return flagged; }//是否标记
    bool isMine() const //是否地雷
    {
        if (around == -1) return true;
        else return false;
    }
    int getAround() const { return around; }//周围地雷数量
    void reveal() { revealed = true; }//翻开格子
    void toggleFlag() { flagged = !flagged; }//切换标记状态
    char getDisplayChar() const //获取显示字符
    {
        if (!revealed) return flagged ? 'F' : '#';
        if (isMine()) return '*';
        if (getAround() == 0) return ' ';
        return '0' + getAround(); // 将数字转换为字符
    }
    void setMine() { around = -1; }//设置地雷
    void incrementAround() { around++; }//周围地雷数量加1
};

Tile board[WIDTH][HEIGHT];

void PlaceMinesAvoiding(int firstX, int firstY)
{
    // 1. 重置棋盘（清除上一局的所有雷和数字）
    for (int x = 0; x < WIDTH; x++)
        for (int y = 0; y < HEIGHT; y++)
            board[x][y] = Tile();

    // 2. 随机布雷，但避开 firstX, firstY 及其周围8格
    int count = 0;
    while (count < MINES) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;

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
                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                    if (!board[nx][ny].isMine())
                        board[nx][ny].incrementAround();
                }
            }
        }
    }
}

//翻开(x,y)位置的格子, isRoot = true表示这是玩家翻开的格子，false表示递归翻开的格子; Reveal返回false表示踩雷
bool Reveal(int x, int y, bool isRoot = true) 
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        if (isRoot) cout << "坐标越界，请重新输入！" << endl;
        return true;  // 越界不视为踩雷
    }

    Tile &tile = board[x][y];

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

bool ToggleFlag(int x, int y)
{
    if(x<0 || x>= WIDTH || y<0 ||y>= HEIGHT) return false;//越界检查
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
    cout<< "当前已标记的雷数: " << FlaggedMines << "/" << MINES << endl;
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

void Win()
{
    cout << "你赢了！" << endl;
    for(int x = 0; x < WIDTH; x++)
        for(int y = 0; y < HEIGHT; y++)
            board[x][y].reveal();//翻开所有格子显示地雷位置
    DisplayBoard();
    return;
}

int main()
{
    srand(time(0));
    bool gameStarted = false;
    while(true)
    {
        DisplayBoard();
        if(!gameStarted){ 
            cout<< "本关共有" << MINES << "个地雷。" << endl;
            cout << "欢迎来到扫雷游戏！请输入坐标x y 以开始游戏：";
        } else { 
            cout << "输入格式: (坐标 x y 翻开) 或 (f x y 插旗/取消旗子)：";
        }

        string command;
        cin >> command;
        if (handleInputFailure()) continue;
        if(!gameStarted){
            int sx, sy;
            sx = stoi(command);
            cin >> sy;
            if (handleInputFailure("无效坐标！")) continue;
            if(sx < 0 || sx >= WIDTH || sy < 0 || sy >= HEIGHT){
                cout <<"无效坐标，请重新输入！" << endl;
                continue;
            }
            PlaceMinesAvoiding(sx ,sy); // 在玩家第一次点击的格子周围放置地雷，确保第一次点击安全
            gameStarted = true;
            Reveal(sx, sy); // 翻开玩家第一次点击的格子
            continue;
        }
        // 标记模式
        if (command == "f") {
            int fx, fy;
            cout << "标记模式：请输入要切换旗子的坐标(x y): ";
            cin >> fx >> fy;
            if (handleInputFailure("无效坐标！")) continue;
            ToggleFlag(fx, fy);
            if(FlaggedMines == MINES) {
                Win();
                break;
            }
            continue;  // 回到循环开头，不进行翻开逻辑
        }
        else{
            int mx, my;
            mx = stoi(command); // 将第一个输入的命令转换为整数坐标
            cin >> my;
            if (handleInputFailure("无效坐标！")) continue;

            if(!Reveal(mx ,my)){
                cout << "Game Over!" << endl;
                for(int x = 0; x < WIDTH; x++)
                    for(int y = 0; y < HEIGHT; y++)
                        board[x][y].reveal();//翻开所有格子显示地雷位置
                DisplayBoard();
                break;
            }

            if(RevealedSafe == TotalSafe)
            {
                Win();
                break;
            }
        }
    }
}