#pragma once
#ifndef __OIL_CAN_H__
#define __OIL_CAN_H__

#include "GridObject.h"

class OilCan : public GridObject
{
public:
    OilCan(const GridPosition& position);
    ~OilCan() override;

    // 爆発を受けたときの処理
    void OnExplosion() override;

    // オブジェクトの種類を取得
    GridObjectType GetType() const override;

    // オブジェクトの状態をコピーする
    std::unique_ptr<GridObject> Clone() const override;
};

#endif