#pragma once
#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"
#include "Camera.h"
#include "Map.h"
#include "MapRenderer.h"
#include "Player.h"
#include "BombInventory.h"
#include "HandheldBombUI.h"


class SceneGame : public Scene
{
public:
    SceneGame();
    ~SceneGame();

    void Update() final;
    void Draw() final;


private:

    //==================================================
    // カメラ
    Camera* m_pCamera;

    //==================================================
    // UI
    HandheldBombUI* m_pHandheldBombUI;

    //==================================================
    // ゲーム管理

    Map m_map;
    MapRenderer m_mapRenderer;
    Player m_player;

    // 爆弾の所持数・選択状態
    // ステージごとの初期設定を行う
    BombInventory m_bombInventory;


    //==================================================
    // 爆発範囲プレビュー

    // Playerから設置予定位置・回転・選択中の爆弾を取得して更新
    void UpdateBombPreview();
};

#endif