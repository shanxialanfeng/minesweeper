#include<iostream>
#include<string>
#include"Board.hpp"
using namespace std;

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
    int h, w, m;
    cout << "请输入棋盘的高度、宽度和地雷数量 (h w m): " << endl;
    cout << "请注意：地雷数量必须小于 (高度 * 宽度 - 9)，以确保第一步点击的安全。" << endl;
    cin >> h >> w >> m;
    Board board(h, w, m);
    bool gameStarted = false;
    while(true)
    {
        board.DisplayBoard();
        if(!gameStarted){ 
            cout<< "本关共有" << board.getMines() << "个地雷。" << endl;
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
            if(sx < 0 || sx >= board.getWidth() || sy < 0 || sy >= board.getHeight()){
                cout <<"无效坐标，请重新输入！" << endl;
                continue;
            }
            board.PlaceMinesAvoiding(sx ,sy); // 在玩家第一次点击的格子周围放置地雷，确保第一次点击安全
            gameStarted = true;
            board.Reveal(sx, sy); // 翻开玩家第一次点击的格子
            continue;
        }
        // 标记模式
        if (command == "f") {
            int fx, fy;
            cout << "标记模式：请输入要切换旗子的坐标(x y): ";
            cin >> fx >> fy;
            if (handleInputFailure("无效坐标！")) continue;
            board.ToggleFlag(fx, fy);
            if(board.getFlaggedMines() == board.getMines()) {
                board.Win();
                break;
            }
            continue;  // 回到循环开头，不进行翻开逻辑
        }
        else{
            int mx, my;
            mx = stoi(command); // 将第一个输入的命令转换为整数坐标
            cin >> my;
            if (handleInputFailure("无效坐标！")) continue;

            if(!board.Reveal(mx ,my)){
                cout << "Game Over!" << endl;
                board.RevealAll();
                board.DisplayBoard();
                break;
            }
            
            if(board.getRevealedSafe() == board.getTotalSafe()){
                board.Win();
                break;
            }
        }
    }
}