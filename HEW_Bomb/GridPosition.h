#pragma once

#ifndef __GRID_POSITION_H__
#define __GRID_POSITION_H__

// グリッド上の座標を表すクラス
class GridPosition
{
public:
    GridPosition();
    GridPosition(int x, int y, int z);

    // 座標取得
    int GetX() const;
    int GetY() const;
    int GetZ() const;

    // 座標設定
    void Set(int x, int y, int z);

    // 座標同士の加算
    GridPosition operator+(const GridPosition& other) const;

    // 座標同士の減算
    GridPosition operator-(const GridPosition& other) const;

    // 加算代入
    GridPosition& operator+=(const GridPosition& other);

    // 減算代入
    GridPosition& operator-=(const GridPosition& other);

    // 座標が同じか
    bool operator==(const GridPosition& other) const;

    // 座標が異なるか
    bool operator!=(const GridPosition& other) const;

private:
    int m_x;
    int m_y;
    int m_z;
};

#endif // __GRID_POSITION_H__