#pragma once
#ifndef __MAP_RENDERER_H__
#define __MAP_RENDERER_H__

#include <DirectXMath.h>
#include <vector>

#include "GridPosition.h"
#include "BombType.h"
#include "ExplosionRangeRenderer.h"

class GridManager;
class Model;
class Map;
class Bomb;

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
		// オブジェクトモデルを解放
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

		// 爆弾モデルを解放
        delete m_pCubeBombModel;
        delete m_pWideCubeBombModel;
        delete m_pCylinderBombModel;
        delete m_pVShapeBombModel;
        delete m_pCircleBombModel;
        delete m_pCrossBombModel;
        delete m_pInvertedPyramidBombModel;

        m_pCubeBombModel = nullptr;
        m_pWideCubeBombModel = nullptr;
        m_pCylinderBombModel = nullptr;
        m_pVShapeBombModel = nullptr;
        m_pCircleBombModel = nullptr;
        m_pCrossBombModel = nullptr;
        m_pInvertedPyramidBombModel = nullptr;
        // 爆発範囲のキャッシュを削除
        m_explosionRangeRenderer.Clear();
    }

    // マップの描画

    // GridManagerからマップ情報を取得し、壁やオブジェクトを描画する
    // view：カメラのビュー行列
    // projection：カメラのプロジェクション行列
    void Draw(
        const Map& map,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection
    );

    //==================================================
    // 設置前の爆発範囲プレビュー

    // 爆発範囲を設定する
    void SetExplosionPreview(
        BombType type,
        const GridPosition& position,
        const DirectX::XMFLOAT3& rotation
    );

    // プレビューを非表示にする
    void ClearExplosionPreview();


private:
    // 指定したモデルを指定したワールド座標に描画する
    // pModel：描画するモデル
    // worldPosition：モデルを配置するワールド座標
    void DrawModel(
        Model* pModel,
        const DirectX::XMFLOAT3& worldPosition
    );

    // 回転を指定してモデルを描画する
    void DrawModel(
        Model* pModel,
        const DirectX::XMFLOAT3& worldPosition,
        const DirectX::XMFLOAT3& rotation
    );

    // 起爆スポットのワイヤー描画
    void DrawDetonationSpots(
        const Map& map,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection
    );

private:
    // 描画用モデル

    Model* m_pWallModel;
    Model* m_pNetworkServerModel;
    Model* m_pGeneratorModel;
    Model* m_pComputerModel;
    Model* m_pPillarModel;
    Model* m_pOilCanModel;
    Model* m_pUnbreakModel;
    Model* m_pFloorModel;

    // プレイヤーモデル
    Model* m_pPlayerModel;

    //==================================================
    // 爆弾モデル

    Model* m_pCubeBombModel;
    Model* m_pWideCubeBombModel;
    Model* m_pCylinderBombModel;
    Model* m_pVShapeBombModel;
    Model* m_pCircleBombModel;
    Model* m_pCrossBombModel;
    Model* m_pInvertedPyramidBombModel;

    // 爆弾モデルの読み込み
    bool LoadBombModel(Model*& pModel, const char* filePath);


    //==================================================
    // 爆発範囲のキャッシュ

    struct ExplosionRangeCache
    {
        // キャッシュ対象の爆弾
        const Bomb* pBomb;

        // 前回計算した爆弾の情報
        BombType type;
        GridPosition position;
        DirectX::XMFLOAT3 rotation;

        // 計算済みの爆発範囲
        std::vector<GridPosition> affectedPositions;

        // 今回の描画で使用されたか
        bool isUsed;
    };
    // 爆発範囲の描画管理
    ExplosionRangeRenderer m_explosionRangeRenderer;
};

#endif