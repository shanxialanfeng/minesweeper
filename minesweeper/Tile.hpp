#ifndef TILE_HPP
#define TILE_HPP

class Tile
{
        private:
    int around; //-1表示地雷，0-8表示周围地雷数量
    bool revealed;
    bool flagged;

    public:
    Tile();
    
    bool isRevealed() const;
    bool isFlagged() const;
    bool isMine() const;
    int getAround() const;
    void reveal();
    void toggleFlag();
    char getDisplayChar() const;
    void setMine();
    void incrementAround();
};

#endif