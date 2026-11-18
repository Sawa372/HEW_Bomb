#pragma once
#ifndef __MAP_H__
#define __MAP_H__

#include "GridManager.h"
#include "GridObjectManager.h"
#include "BombManager.h"
#include "DetonationSpot.h"

#include <DirectXMath.h>

#include <vector>

//==================================================
// ステージのマップを管理するクラス
// グリッドの初期化やステージ内の壁・オブジェクトの配置を行う
// マップ作成用のヘッダのつもり
// 改善していく予定
//==================================================


class BombInventory;

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

    // 爆発範囲内のオブジェクトに爆発を伝える
    void ProcessExplosion(
        const std::vector<GridPosition>& positions
    );



    //==================================================
    // 爆弾の管理

   //// 爆弾を設置する
   // bool AddBomb(
   //     const GridPosition& position,
   //     BombInventory& inventory
   // );

    // 回転を指定して爆弾を設置する
    bool AddBomb(
        const GridPosition& position,
        const DirectX::XMFLOAT3& rotation,
        BombInventory& inventory
    );

    // 爆弾を回収する
    bool RemoveBombAt(
        const GridPosition& position,
        BombInventory& inventory
    );

    // 指定位置の爆弾を取得する
    Bomb* GetBombAt(const GridPosition& position);
    const Bomb* GetBombAt(const GridPosition& position) const;

    // 指定位置に爆弾があるか
    bool HasBombAt(const GridPosition& position) const;

    // 設置済み爆弾の数
    int GetBombCount() const;

    // 指定位置の爆弾を起爆する
    // 爆発したすべてのマスを outAffectedPositions に記録する
    bool DetonateBombAt(
        const GridPosition& position,
        std::vector<GridPosition>* outAffectedPositions = nullptr
    );


    // 起爆スポットを登録
    bool AddDetonationSpot(const GridPosition& position);

    // 登録されている起爆スポット一覧を取得
    const std::vector<DetonationSpot>& GetDetonationSpots() const;

    //==================================================
    // Undo処理

    // マップの状態を保存
    bool SaveUndoState();

    // 保存した状態に戻す
    bool RestoreUndoState();

    // 保存データを破棄
    void DiscardUndoState();


private:
    // ステージ内の壁やオブジェクトの配置を作成する
    // Initialize()から呼び出される
    bool CreateStage();

    // ステージにオブジェクトを登録
    bool RegisterStageObject(
        GridObjectType type,
        const GridPosition& position
    );


private:
    // ステージのグリッド情報を管理する
    GridManager m_gridManager;

    GridObjectManager m_gridObjectManager;

    // 設置済み爆弾を管理する
    BombManager m_bombManager;

    int n_FroorDistance;

    // 起爆スポット
    std::vector<DetonationSpot> m_detonationSpots;
};

#endif // __MAP_H__