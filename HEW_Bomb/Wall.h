#pragma once
#ifndef __WALL_H__
#define __WALL_H__

#include "GridObject.h"

// 壁を表すクラス
class Wall : public GridObject
{
public:
    Wall(const GridPosition& position);
    ~Wall() override;

    // 爆発を受けたときの処理
    void OnExplosion() override;

    // オブジェクトの種類を取得
    GridObjectType GetType() const override;
};

#endif // __WALL_H__