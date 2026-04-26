#ifndef BOARD_HPP
#define BOARD_HPP

#include"Tile.hpp"
#include<vector>

class Board
{
    private:
    int height, width, mines;
    int TotalSafe;
    std::vector<std::vector<Tile>> board;
    int RevealedSafe;
    int FlaggedMines;

    public:
    Board(int h, int w, int m);
    void PlaceMinesAvoiding(int firstX, int firstY);
    bool Reveal(int x, int y, bool isRoot = true);
    bool ToggleFlag(int x, int y);
    void DisplayBoard();
    void RevealAll();
    void Win();
    int getRevealedSafe() const;
    int getFlaggedMines() const;
    int getHeight() const;
    int getWidth() const;
    int getMines() const;
    int getTotalSafe() const;
};

#endif