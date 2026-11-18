#pragma once
#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "GridObject.h"

class Generator : public GridObject
{
public:
    Generator(const GridPosition& position);
    ~Generator() override;

    // 爆発を受けたときの処理
    void OnExplosion() override;

    // オブジェクトの種類を取得
    GridObjectType GetType() const override;

    // オブジェクトの状態をコピーする
    std::unique_ptr<GridObject> Clone() const override;

private:
    int m_explosionCount;
};

#endif
