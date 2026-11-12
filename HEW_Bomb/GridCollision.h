//★★固定サイスの配列を使ったbool判定ベースのグリッド当たり判定の書き方★★
//#pragma once
//#ifndef __GRID_COLLISION_H__
//#define __GRID_COLLISION_H__
//
//#include <DirectXMath.h>
//#include "GridPosition.h"
//
//// 固定サイズの最大幅と高さ（必要に応じて変更）
//const int GRID_WIDTH = 10;
//const int GRID_DEPTH = 10;
//
//class GridCollision
//{
//public:
//    GridCollision();
//    ~GridCollision();
//
//    // 範囲内かどうかを bool で返す
//    bool IsInsideBounds(const GridPosition& pos) const;
//
//    // 指定座標に移動できるか（歩行可能か）を bool で返す
//    bool CanMoveTo(const GridPosition& pos) const;
//
//    // 壁の状態を設定する
//    void SetWall(const GridPosition& pos, bool isWall);
//
//    // 壁かどうかを bool で取得する
//    bool IsWall(const GridPosition& pos) const;
//
//    // 座標変換
//    GridPosition WorldToGrid(const DirectX::XMFLOAT3& worldPos, float cellSize = 1.0f) const;
//    DirectX::XMFLOAT3 GridToWorld(const GridPosition& gridPos, float cellSize = 1.0f) const;
//
//private:
//    // 二次元配列（または一次元配列）で壁の有無を管理
//    // true: 壁（移動不可）, false: 通行可能
//    bool m_isWall[GRID_DEPTH][GRID_WIDTH];
//};
//
//#endif // __GRID_COLLISION_H__

#pragma once
#ifndef __GRID_COLLISION_H__
#define __GRID_COLLISION_H__

#include <DirectXMath.h>
#include "GridPosition.h"
#include "GridManager.h"

// グリッドベースの当たり判定を管理するクラス
class GridCollision
{
public:
    // コンストラクタで GridManager の参照を紐付ける
    explicit GridCollision(GridManager& gridManager);
    ~GridCollision();

    // --- 1. bool 形による直感的な判定系 ---

    // 指定のワールド座標が移動可能か（範囲内かつ歩行可能、プレイヤー不在など）
    bool CanMove(const DirectX::XMFLOAT3& worldPos) const;
    bool CanMove(const GridPosition& gridPos) const;

    // 指定のワールド座標に壁があるか
    bool IsWall(const DirectX::XMFLOAT3& worldPos) const;
    bool IsWall(const GridPosition& gridPos) const;

    // 指定のワールド座標にオブジェクトがあるか
    bool HasObject(const DirectX::XMFLOAT3& worldPos) const;
    bool HasObject(const GridPosition& gridPos) const;

    // 指定のワールド座標に爆弾があるか
    bool HasBomb(const DirectX::XMFLOAT3& worldPos) const;
    bool HasBomb(const GridPosition& gridPos) const;

    // 指定のワールド座標にプレイヤーがいるか
    bool HasPlayer(const DirectX::XMFLOAT3& worldPos) const;
    bool HasPlayer(const GridPosition& gridPos) const;

    // --- 2. 設置・アクション判定系 ---

    // 爆弾を設置できるか
    bool CanPlaceBomb(const DirectX::XMFLOAT3& worldPos) const;
    bool CanPlaceBomb(const GridPosition& gridPos) const;

    // 壁を配置できるか
    bool CanPlaceWall(const DirectX::XMFLOAT3& worldPos) const;
    bool CanPlaceWall(const GridPosition& gridPos) const;

    // オブジェクトを配置できるか
    bool CanPlaceObject(const DirectX::XMFLOAT3& worldPos) const;
    bool CanPlaceObject(const GridPosition& gridPos) const;

    // --- 3. 演算子オーバーロード（bool として直接評価できるようにする） ---
    // 例: if (gridCollision(pos)) { ... }
    bool operator()(const GridPosition& gridPos) const;
    bool operator()(const DirectX::XMFLOAT3& worldPos) const;

private:
    GridManager& m_gridManager; // 既存のGridManagerへの参照
};

#endif // __GRID_COLLISION_H__