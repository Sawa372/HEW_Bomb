#pragma once
#ifndef __GRID_OBJECT_H__
#define __GRID_OBJECT_H__

#include "GridPosition.h"
#include "GridObjectType.h"

// グリッド上に配置されるオブジェクトの基底クラス
class GridObject
{
public:
    GridObject(const GridPosition& position);
    virtual ~GridObject();

    // 爆発を受けたときの処理
    virtual void OnExplosion() = 0;

    // オブジェクトの座標を取得
    const GridPosition& GetPosition() const;

    // オブジェクトの種類を取得
    virtual GridObjectType GetType() const = 0;

    // 破壊されたか
    bool IsDestroyed() const;

protected:
    // オブジェクトを破壊済みにする
    void Destroy();

private:
    // グリッド上の座標
    GridPosition m_position;

    // 破壊されたか
    bool m_isDestroyed;
};

#endif // __GRID_OBJECT_H__
