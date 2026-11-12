#pragma once
#ifndef __MAP_RENDERER_H__
#define __MAP_RENDERER_H__

#include <DirectXMath.h>

class GridManager;
class Model;

//==================================================
// マップの描画を管理するクラス
// GridManagerからマップ情報を取得し、壁やオブジェクトを3Dモデルで描画する
// モデルの読み込み・解放・種類ごとの切り替えもする
// 後々だれにも触れないようにする予定
//==================================================
class MapRenderer
{
public:
    // コンストラクタ
    MapRenderer();

    // デストラクタ
    // 管理しているモデルを解放する
    ~MapRenderer();

    // 初期化・解放

    // 描画に使用する3Dモデルを読み込む
    // 読み込みに失敗した場合はfalseを返す
    bool Initialize();

    // 読み込んだ3Dモデルをすべて解放する
    // 読み込んだすべてのモデルを解放する
    void Clear()
    {
        delete m_pWallModel;
        delete m_pNetworkServerModel;
        delete m_pGeneratorModel;
        delete m_pComputerModel;
        delete m_pPillarModel;
        delete m_pOilCanModel;
        delete m_pFloorModel;

        m_pWallModel = nullptr;
        m_pNetworkServerModel = nullptr;
        m_pGeneratorModel = nullptr;
        m_pComputerModel = nullptr;
        m_pPillarModel = nullptr;
        m_pOilCanModel = nullptr;
        m_pFloorModel = nullptr;

        // プレイヤーモデルを解放
        delete m_pPlayerModel;
        m_pPlayerModel = nullptr;
    }

    // マップの描画

    // GridManagerからマップ情報を取得し、壁やオブジェクトを描画する
    // view：カメラのビュー行列
    // projection：カメラのプロジェクション行列
    void Draw(
        const GridManager& grid,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection
    );

private:
    // 指定したモデルを指定したワールド座標に描画する
    // pModel：描画するモデル
    // worldPosition：モデルを配置するワールド座標
    void DrawModel(
        Model* pModel,
        const DirectX::XMFLOAT3& worldPosition
    );

private:
    // 描画用モデル

    Model* m_pWallModel;
    Model* m_pNetworkServerModel;
    Model* m_pGeneratorModel;
    Model* m_pComputerModel;
    Model* m_pPillarModel;
    Model* m_pOilCanModel;
    Model* m_pFloorModel;

    // プレイヤーモデル
    Model* m_pPlayerModel;
};

#endif