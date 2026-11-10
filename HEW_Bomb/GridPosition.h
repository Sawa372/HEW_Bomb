#pragma once

#ifndef __GRID_POSITION_H__
#define __GRID_POSITION_H__

//==================================================
// グリッド上の座標を管理するクラス
// X・Y・Zの3つの座標を保持し、座標の取得・設定や
// 座標同士の計算、比較を行う
// 移動管理ツールとしての利用しかしない
//==================================================
class GridPosition
{
public:
    // コンストラクタ
    // 各座標を初期値に設定する
    GridPosition();

    // デストラクタ
    ~GridPosition();

    // 指定されたX・Y・Z座標で初期化する
    GridPosition(int x, int y, int z);

    // X座標を取得する
    int GetX() const;

    // Y座標を取得する
    int GetY() const;

    // Z座標を取得する
    int GetZ() const;

    // X・Y・Z座標をまとめて設定する
    void Set(int x, int y, int z);

    // 2つの座標を加算した新しい座標を返す
    GridPosition operator+(const GridPosition& other) const;

    // 2つの座標を減算した新しい座標を返す
    GridPosition operator-(const GridPosition& other) const;

    // 現在の座標に指定された座標を加算する
    // 計算結果は現在の座標に反映される
    GridPosition& operator+=(const GridPosition& other);

    // 現在の座標から指定された座標を減算する
    // 計算結果は現在の座標に反映される
    GridPosition& operator-=(const GridPosition& other);

    // X・Y・Zのすべての座標が一致しているか確認する
    bool operator==(const GridPosition& other) const;

    // X・Y・Zのいずれかの座標が異なっているか確認する
    bool operator!=(const GridPosition& other) const;

private:
    // X座標
    int m_x;

    // Y座標
    int m_y;

    // Z座標
    int m_z;
};

#endif // __GRID_POSITION_H__