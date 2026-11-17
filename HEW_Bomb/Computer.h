#pragma once
#ifndef __COMPUTER_H__
#define __COMPUTER_H__

#include "GridObject.h"

class Computer : public GridObject
{
public:
    Computer(const GridPosition& position);
    ~Computer() override;

    // 爆発を受けたときの処理
    void OnExplosion() override;

    // オブジェクトの種類を取得
    GridObjectType GetType() const override;

    // オブジェクトの状態をコピーする
    std::unique_ptr<GridObject> Clone() const override;
};

#endif