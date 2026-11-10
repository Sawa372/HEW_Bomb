#pragma once
#ifndef __MAP_RENDERER_H__
#define __MAP_RENDERER_H__

#include <DirectXMath.h>

class GridManager;
class Model;

// マップの描画を管理するクラス
class MapRenderer
{
public:
    MapRenderer();
    ~MapRenderer();

    // 初期化
    bool Initialize();

    // 解放
    void Clear();

    // マップの描画
    void Draw(
        const GridManager& grid,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection
    );

private:
    // モデルの描画
    void DrawModel(
        Model* pModel,
        const DirectX::XMFLOAT3& worldPosition
    );

private:
    // 壁用モデル
    Model* m_pWallModel;

    // オブジェクト用モデル
    Model* m_pDefaultModel;
    Model* m_pTreeModel;
    Model* m_pRockModel;
    Model* m_pBoxModel;
    Model* m_pPillarModel;
    Model* m_pOilDrumModel;
};

#endif