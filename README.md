# 扫雷游戏 (Minesweeper)

一个用 C++ 实现的控制台扫雷游戏，支持动态棋盘大小，采用模块化设计（Tile、Board 分离），并使用现代 C++ 特性（`std::vector`、`<random>` 等）。

## 版本 v4.1 更新内容
- 将代码拆分为多文件模块：`Tile.hpp/cpp`、`Board.hpp/cpp`、`main.cpp`
- 使用 `std::mt19937` 替代 `rand()`，随机数质量更高
- 改进了文件组织，所有源文件位于 `minesweeper/` 子目录
- 更新 README 文档

## 游戏特性
- 支持自定义棋盘宽度、高度和地雷数量
- **第一次点击安全**：保证第一次翻开不是雷，且周围 8 格无雷（避免第一步踩雷）
- **旗子标记**：输入 `f x y` 切换旗子，正确标记所有雷即可获胜
- **双重胜利条件**：翻开所有安全格子 或 标记完所有地雷
- 实时显示已标记的雷数

## 编译运行

### 使用 g++
```bash
g++ -std=c++17 minesweeper/*.cpp -o minesweeper.exe
./minesweeper.exe