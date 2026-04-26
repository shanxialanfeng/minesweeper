#include<iostream>
#include"Tile.hpp"

Tile::Tile(): around(0), revealed(false), flagged(false) {} //默认构造函数
bool Tile::isRevealed() const { return revealed; }//是否翻开
bool Tile::isFlagged() const { return flagged; }//是否标记
bool Tile::isMine() const //是否地雷
{
    if (around == -1) return true;
    else return false;
}
int Tile::getAround() const { return around; }//周围地雷数量
void Tile::reveal() { revealed = true; }//翻开格子
void Tile::toggleFlag() { flagged = !flagged; }//切换标记状态
char Tile::getDisplayChar() const //获取显示字符
{
    if (!revealed) return flagged ? 'F' : '#';
    if (isMine()) return '*';
    if (getAround() == 0) return ' ';
    return '0' + getAround(); // 将数字转换为字符
}
void Tile::setMine() { around = -1; }//设置地雷
void Tile::incrementAround() { around++; }//周围地雷数量加1