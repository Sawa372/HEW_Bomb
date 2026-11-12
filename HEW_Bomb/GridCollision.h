#pragma once
#ifndef __GRID_COLLISION_H__
#define __GRID_COLLISION_H__

#include <DirectXMath.h>
#include "GridPosition.h"
#include "GridManager.h"
#include "GridCell.h"
#include "GridObjectType.h"

// グリッドベースの当たり判定を管理するクラス
class GridCollision
{
public:
    // コンストラクタで GridManager の参照を紐付ける
    explicit GridCollision(GridManager& gridManager);
    ~GridCollision();

    // ==============================================
    // 1. 移動・通行判定 (bool形による直感的な判定系)
    // ==============================================
    // 指定のワールド座標が移動可能か（範囲内かつ歩行可能、プレイヤー不在など）
    bool CanMove(const DirectX::XMFLOAT3& worldPos) const;
    bool CanMove(const GridPosition& gridPos) const;

    // ==========================================
    // 2. セルの状態確認 
    // ==========================================
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

    bool HasFloor(const DirectX::XMFLOAT3& worldPos) const;
    bool HasFloor(const GridPosition& gridPos) const;

    // ==========================================
    // 3. 配置可能判定 
    // ==========================================
    // 爆弾を設置できるか
    bool CanPlaceBomb(const DirectX::XMFLOAT3& worldPos) const;
    bool CanPlaceBomb(const GridPosition& gridPos) const;

    // 壁を配置できるか
    bool CanPlaceWall(const DirectX::XMFLOAT3& worldPos) const;
    bool CanPlaceWall(const GridPosition& gridPos) const;

    // オブジェクトを配置できるか
    bool CanPlaceObject(const DirectX::XMFLOAT3& worldPos) const;
    bool CanPlaceObject(const GridPosition& gridPos) const;

    // プレイヤーを配置できるか
    bool CanPlacePlayer(const DirectX::XMFLOAT3& worldPos) const;
    bool CanPlacePlayer(const GridPosition& gridPos) const;

    // ==========================================
    // 4. 補助および演算子オーバーロード
    // ==========================================
    bool IsInside(const DirectX::XMFLOAT3& worldPos) const;
    bool IsInside(const GridPosition& gridPos) const;

    // 演算子 `()` で `if (collision(pos))` のように bool 判定できるようにする
   // 例: if (gridCollision(pos)) { ... }
    bool operator()(const DirectX::XMFLOAT3& worldPos) const;
    bool operator()(const GridPosition& gridPos) const;

private:
    GridManager& m_gridManager; // 既存のGridManagerへの参照
};

#endif // __GRID_COLLISION_H__