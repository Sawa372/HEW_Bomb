#pragma once
#ifndef __MAP_H__
#define __MAP_H__

#include "GridManager.h"

//==================================================
// ステージのマップを管理するクラス
// グリッドの初期化やステージ内の壁・オブジェクトの配置を行う
// マップ作成用のヘッダのつもり
// 改善していく予定
//==================================================
class Map
{
public:
    // コンストラクタ
    Map();

    // デストラクタ
    ~Map();

    // マップを初期化し、ステージの配置を作成する
    bool Initialize();

    // マップのグリッド情報を解放する
    void Clear();

    // 管理しているGridManagerを取得する
    // グリッドの情報を変更する場合に使用する
    GridManager& GetGridManager();

    // 管理しているGridManagerを取得する（読み取り専用）
    // グリッドの情報を参照する場合に使用する
    const GridManager& GetGridManager() const;

private:
    // ステージ内の壁やオブジェクトの配置を作成する
    // Initialize()から呼び出される
    bool CreateStage();

private:
    // ステージのグリッド情報を管理する
    GridManager m_gridManager;
};

#endif // __MAP_H__