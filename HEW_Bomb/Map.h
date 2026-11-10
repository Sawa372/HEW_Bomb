#pragma once
#ifndef __MAP_H__
#define __MAP_H__

#include "GridManager.h"

// ステージのマップを管理するクラス
class Map
{
public:
    Map();

    // マップの初期化
    bool Initialize();

    // マップの解放
    void Clear();

    // グリッドマネージャーの取得
    GridManager& GetGridManager();
    const GridManager& GetGridManager() const;

private:
    // ステージの配置を作成
    bool CreateStage();

private:
    GridManager m_gridManager;
};

#endif // __MAP_H__