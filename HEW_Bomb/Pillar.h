#pragma once
#ifndef __PILLAR_H__
#define __PILLAR_H__

#include "GridObject.h"

class Pillar : public GridObject
{
public:
    Pillar(const GridPosition& position);
    ~Pillar() override;

    // 爆発を受けたときの処理
    void OnExplosion() override;

    // オブジェクトの種類を取得
    GridObjectType GetType() const override;

    // オブジェクトの状態をコピーする
    std::unique_ptr<GridObject> Clone() const override;
};

#endif
