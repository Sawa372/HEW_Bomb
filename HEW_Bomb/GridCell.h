#pragma once
#ifndef __GRID_CELL_H__
#define __GRID_CELL_H__

#include "GridPosition.h"
#include "GridObjectType.h"

// グリッド上の1マスを表すクラス
class GridCell
{
public:
    GridCell();
    GridCell(const GridPosition& position);

    // マスの座標
    const GridPosition& GetPosition() const;

    // 壁かどうか
    bool IsWall() const;

    // オブジェクトがあるか
    bool HasObject() const;

    // オブジェクトの種類を取得
    GridObjectType GetObjectType() const;

    // オブジェクトの種類を設定
    void SetObjectType(GridObjectType type);

    // 爆弾があるか
    bool HasBomb() const;

    // プレイヤーがいるか
    bool HasPlayer() const;

    // 壁の設定
    void SetWall(bool wall);

    // オブジェクトの設定
    void SetObject(bool object);

    // 爆弾の設定
    void SetBomb(bool bomb);

    // プレイヤーの設定
    void SetPlayer(bool player);

    // 移動可能か
    bool IsWalkable() const;

private:
    GridPosition m_position;

    bool m_isWall;
    GridObjectType m_objectType;    
    bool m_hasBomb;
    bool m_hasPlayer;
};

#endif // __GRID_CELL_H__