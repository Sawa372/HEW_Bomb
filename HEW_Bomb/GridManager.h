#pragma once
#ifndef __GRID_MANAGER_H__
#define __GRID_MANAGER_H__

#include "GridPosition.h"
#include "GridCell.h"
#include <vector>
#include <DirectXMath.h>

// グリッド全体を管理するクラス
class GridManager
{
public:
    GridManager();
    ~GridManager();

    // グリッドを作成
    bool Initialize(int width, int height, int depth);

    // グリッドを解放
    void Clear();

    // グリッドのサイズ取得
    int GetWidth() const;
    int GetHeight() const;
    int GetDepth() const;

    // 指定した座標がグリッド内か
    bool IsInside(const GridPosition& position) const;

    // マスを取得
    GridCell* GetCell(const GridPosition& position);
    const GridCell* GetCell(const GridPosition& position) const;

    // 壁判定
    bool IsWall(const GridPosition& position) const;

    // オブジェクト判定
    bool HasObject(const GridPosition& position) const;

    // 爆弾判定
    bool HasBomb(const GridPosition& position) const;

    // プレイヤー判定
    bool HasPlayer(const GridPosition& position) const;

    // 移動可能か
    bool CanMove(const GridPosition& position) const;
   
    // 壁を設定
    bool SetWall(const GridPosition& position);

    // 壁を解除
    bool RemoveWall(const GridPosition& position);

    // オブジェクトを設定
    bool SetObject(const GridPosition& position);

    // オブジェクトを解除
    bool RemoveObject(const GridPosition& position);

    // オブジェクトが配置されている座標を取得
    std::vector<GridPosition> GetObjectPositions() const;

    // 爆弾を設定
    bool SetBomb(const GridPosition& position);

    // 爆弾を解除
    bool RemoveBomb(const GridPosition& position);

    // プレイヤーを設定
    bool SetPlayer(const GridPosition& position);

    // プレイヤーを解除
    bool RemovePlayer(const GridPosition& position);

    // プレイヤーを別のマスへ移動
    bool MovePlayer(
        const GridPosition& currentPosition,
        const GridPosition& nextPosition
    );

    // 爆弾を設置できるか
    bool CanPlaceBomb(const GridPosition& position) const;

    // 壁を配置できるか
    bool CanPlaceWall(const GridPosition& position) const;

    // オブジェクトを配置できるか
    bool CanPlaceObject(const GridPosition& position) const;

    // プレイヤーを配置できるか
    bool CanPlacePlayer(const GridPosition& position) const;

    // 1マスのワールドサイズを設定
    bool SetCellSize(float x, float y, float z);

    // グリッドの原点を設定
    void SetGridOrigin(const DirectX::XMFLOAT3& origin);

    // グリッド座標をワールド座標に変換
    bool GridToWorld(
        const GridPosition& position,
        DirectX::XMFLOAT3& worldPosition
    ) const;

    

private:
    // 座標を配列のインデックスに変換
    int GetIndex(const GridPosition& position) const;

private:
    int m_width;
    int m_height;
    int m_depth;

    std::vector<GridCell> m_cells;

    // 1マスのワールドサイズ
    DirectX::XMFLOAT3 m_cellSize;

    // グリッドの原点
    DirectX::XMFLOAT3 m_gridOrigin;
};

#endif // __GRID_MANAGER_H__