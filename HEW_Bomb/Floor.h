#pragma once
#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "GridObject.h"

class Floor : public GridObject
{
public:
    Floor(const GridPosition& position);
    ~Floor() override;

    // 爆発を受けたときの処理
    void OnExplosion() override;

    // オブジェクトの種類を取得
    GridObjectType GetType() const override;

    // オブジェクトの状態をコピーする
    std::unique_ptr<GridObject> Clone() const override;
};

#endif