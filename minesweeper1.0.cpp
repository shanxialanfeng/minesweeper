#include<iostream>
#include<cstdlib>
#include<ctime>

using namespace std;

const int WIDTH = 9;
const int HEIGHT = 9;
const int MINES = 10;
const int TotalSafe = WIDTH * HEIGHT - MINES;
int RevealedSafe = 0;

struct Tile
{
    int around; //-1表示地雷，0-8表示周围地雷数量
    bool revealed;
    bool flagged;
};

Tile board[WIDTH][HEIGHT];

void BoardInit()//初始化棋盘
{
    //1、清零
    for (int x = 0; x < WIDTH; x++)
        for (int y = 0; y < HEIGHT; y++)
            board[x][y] = {0, false, false}; //聚合初始化

    //2、随机放置地雷，并更新周围格子的数字
    int count = 0;
    while(count < MINES){
        int x = rand() % WIDTH; // 生成 0 到 WIDTH-1 之间的随机数
        int y = rand() % HEIGHT;
        if(board[x][y].around != -1)
        {
            board[x][y].around = -1;
            count++;
            for(int dx = -1; dx <= 1; dx++)
            {
                for(int dy = -1; dy <= 1; dy++)
                {
                    int nx = x + dx, ny = y + dy;
                    if(dx == 0 && dy == 0) continue;
                    if(nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT)
                    {
                        if(board[nx][ny].around != -1)
                            board[nx][ny].around++;
                    }
                }
            }
        }
    }
}

void Reveal(int x, int y) //翻开(x,y)位置的格子
{
    if(x<0 || x>= WIDTH || y<0 ||y>= HEIGHT) return;//越界检查
    Tile &tile = board[x][y];
    if(tile.revealed || tile.flagged) return;//已经翻开或标记过的格子不处理
    tile.revealed = true;
    if(tile.around == -1) return;//地雷不继续翻开
    RevealedSafe++;
    if(tile.around == 0)
    {
        for(int dy = -1; dy <= 1; dy++)
        {
            for(int dx = -1; dx <= 1; dx++)
            {
                if(dx == 0 && dy == 0) continue;
                Reveal(x + dx, y + dy);
            }
        }    
    }
}

void PutFlag(int x, int y)
{
    if(x<0 || x>= WIDTH || y<0 ||y>= HEIGHT) return;//越界检查
    Tile &tile = board[x][y];
    if (tile.revealed) {
        cout << "已翻开的格子不能插旗！" << endl;
        return;
    }
    tile.flagged = !tile.flagged;
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
            if(!tile.revealed)
            {
                if(tile.flagged) cout <<"F ";//标记的格子显示F
                else cout << "# ";//未翻开的格子显示#
            }           
            else{
                if(tile.around == -1) cout <<"* ";//地雷显示*
                else if(tile.around == 0) cout <<"  ";//周围没有地雷显示空格
                else cout << tile.around <<" ";//显示周围地雷数量
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    srand(time(0));
    BoardInit();

    while(true)
    {
        DisplayBoard();
        int mx, my;
        cout<<"请输入坐标(x y), 或输入(-1 -1)切换标记模式：";
        cin >> mx >> my;
        if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "请输入数字！" << endl;
        continue;
        }

        // 标记模式
        if (mx == -1 && my == -1) {
            int fx, fy;
            cout << "标记模式：请输入要切换旗子的坐标(x y): ";
            cin >> fx >> fy;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "无效坐标！" << endl;
                continue;
            }
            if(fx < 0 || fx >= WIDTH || fy < 0 || fy >= HEIGHT){
            cout <<"无效坐标，请重新输入！" << endl;
            continue;
        }
    
            PutFlag(fx, fy);
            continue;  // 回到循环开头，不进行翻开逻辑
        }

        if(mx < 0 || mx >= WIDTH || my < 0 || my >= HEIGHT){
            cout <<"无效坐标，请重新输入！" << endl;
            continue;
        }

        Tile& tile = board[mx][my];

        if(tile.revealed){
            cout << "已经翻开了，请重新输入！" << endl;
            continue;
        }

        if(tile.flagged){
            cout << "这是标记的雷，请取消标记后再翻开。" << endl;
            continue;
        }

        if(tile.around == -1){
            cout << "Game Over!" << endl;
            for(int x = 0; x < WIDTH; x++)
               for(int y = 0; y < HEIGHT; y++)
                   board[x][y].revealed = true;//翻开所有格子显示地雷位置
            DisplayBoard();
            break;
        }

        Reveal(mx, my);

        if(RevealedSafe == TotalSafe)
        {
            cout << "你赢了！" << endl;
            for(int x = 0; x < WIDTH; x++)
               for(int y = 0; y < HEIGHT; y++)
                   board[x][y].revealed = true;//翻开所有格子显示地雷位置
            DisplayBoard();
            break;
        }
    }
}