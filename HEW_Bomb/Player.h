#pragma once

#include <DirectXMath.h>
#include <vector>
#include "GridPosition.h"
#include "Camera.h"
#include "Bomb.h"
#include "BombRotation.h"


class Map;
class BombInventory;
class GridCollision;
class GridManager;

class Player
{
public:
    Player();
    ~Player();

    // プレイヤーの初期化
    void Initialize(const GridPosition& startPos);

    // プレイヤーの更新
    // 移動、爆弾操作、爆弾選択をまとめて管理する
    void Update(
        Map& map,
        BombInventory& inventory,
        const GridCollision& gridCollision
    );

    // プレイヤーの現在位置を取得
    GridPosition PlayerGetPosition() const;

    // 爆弾の設置予定位置を取得
    // SceneGameの爆発範囲プレビューで使用する
    const GridPosition& GetBombPlacementPosition() const;

    // 爆弾の設置回転を取得
    // SceneGameの爆発範囲プレビューで使用する
    const DirectX::XMFLOAT3& GetBombPlacementRotation() const;

    // 爆弾の設置回転を設定
    // 将来的にはプレイヤーの向きから設定する
    void SetBombPlacementRotation(
        const DirectX::XMFLOAT3& rotation
    );

    void SetCamera(Camera* pCamera);

    // プレイヤー死亡判定
    // 危険範囲にプレイヤーがいるか確認
    bool CheckDeath(
        const std::vector<GridPosition>& dangerousPositions
    ) const;

    // 現在の残機を取得
    int GetLife() const;

private:

    //==================================================
    // プレイヤーの基本情報

    GridPosition m_playerPos; // グリッド上のプレイヤー位置

    int m_nLife;    // プレイヤー残機

    // 移動キーの押しっぱなし防止
    bool m_isKeyReleased;

    GridPosition m_startPos;  // プレイヤーの初期位置


    //==================================================
    // 爆弾の設置情報

    // 爆弾を設置する予定のグリッド座標
    GridPosition m_bombPlacementPosition;

    // 爆弾の回転
    DirectX::XMFLOAT3 m_bombPlacementRotation;


    //==================================================
    // 爆弾操作用のキー入力状態　

    bool m_isPlaceKeyReleased;          // 設置：Space
    bool m_isRetrieveKeyReleased;       // 回収：R
    bool m_isDetonateKeyReleased;       // 起爆：E

    // 爆弾選択
    int m_selectedBombIndex;

    bool m_isBombNextKeyReleased;       // 次の爆弾：1
    bool m_isBombPreviousKeyReleased;   // 前の爆弾：2

    // 爆弾回転
    bool m_isRotateXKeyReleased;        // X軸回転：3
    bool m_isRotateYKeyReleased;        // Y軸回転：4

    // 爆弾の回転管理
    BombRotation m_bombRotation;

    // カメラ情報
    Camera* m_pCamera;

    Bomb* m_pBomb;
    
};