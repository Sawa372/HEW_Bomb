#include "SceneGame.h"

#include "Geometory.h"
#include "CameraDebug.h"
#include "GridCollision.h"
#include "GridPosition.h"
#include "BombType.h"

#include <ShaderList.h>
#include <Windows.h>


//==================================================
// コンストラクタ

SceneGame::SceneGame()
    : m_pCamera(nullptr)
{
    //==================================================
    // カメラの生成

    m_pCamera = new CameraDebug;


    //==================================================
    // マップの初期化

    if (!m_map.Initialize())
    {
        MessageBoxA(
            nullptr,
            "マップの初期化に失敗しました",
            "Error",
            MB_OK
        );
    }


    //==================================================
    // マップ描画クラスの初期化

    if (!m_mapRenderer.Initialize())
    {
        MessageBoxA(
            nullptr,
            "MapRendererの初期化に失敗しました",
            "Error",
            MB_OK
        );
    }


    //==================================================
    // 爆弾の所持数設定（現在はテスト用）

    // 全種類の爆弾を5個ずつ所持
    // 将来的にはステージデータから設定する

    for (int i = 0; i < GetBombTypeCount(); i++)
    {
        BombType type = GetBombTypeByIndex(i);

        m_bombInventory.SetCount(type, 5);
    }

    // 最初の爆弾を選択
    m_bombInventory.SelectBomb(
        GetBombTypeByIndex(0)
    );


    //==================================================
    // プレイヤーの初期化

    m_player.Initialize(GridPosition(0, 0, 0));

    // マップ側にプレイヤーの位置を登録
    m_map.GetGridManager().SetPlayer(
        m_player.PlayerGetPosition()
    );


    //==================================================
    // 爆発範囲プレビューの初期化

    UpdateBombPreview();
}


//==================================================
// デストラクタ

SceneGame::~SceneGame()
{
    delete m_pCamera;
    m_pCamera = nullptr;
}


//==================================================
// 更新処理

void SceneGame::Update()
{
    //==================================================
    // プレイヤーの更新

    GridCollision gridCollision(m_map.GetGridManager());

    m_player.Update(
        m_map,
        m_bombInventory,
        gridCollision
    );

    if(m_player.IsDeathPlayer(m_map.GetGridManager()))
    {
        m_player.Initialize(GridPosition(0, 0, 0));
    }


    //==================================================
    // プレイヤーのワールド座標を取得

    DirectX::XMFLOAT3 playerWorldPos;

    if (m_map.GetGridManager().GridToWorld(
        m_player.PlayerGetPosition(),
        playerWorldPos))
    {
        CameraDebug* pCamera =
            static_cast<CameraDebug*>(m_pCamera);

        pCamera->SetTargetY(playerWorldPos.y);
    }


    //==================================================
    // カメラの更新

    m_pCamera->Update();


    //==================================================
    // 爆発範囲プレビューの更新

    UpdateBombPreview();
}


//==================================================
// 描画処理

void SceneGame::Draw()
{
    // 深度テストの有効化
    SetDepthTest(DEPTH_ENABLE);


    //==================================================
    // カメラの設定

    Geometory::SetView(
        m_pCamera->GetViewMatrix()
    );

    Geometory::SetProjection(
        m_pCamera->GetProjectionMatrix()
    );


    //==================================================
    // マップの描画

    m_mapRenderer.Draw(
        m_map,
        m_pCamera->GetViewMatrix(),
        m_pCamera->GetProjectionMatrix()
    );
}


//==================================================
// 爆発範囲プレビューの更新

void SceneGame::UpdateBombPreview()
{
    m_mapRenderer.SetExplosionPreview(
        m_bombInventory.GetSelectedType(),
        m_player.GetBombPlacementPosition(),/////////////////////////////これもPlayer.cppを書き換えたら動くはず！
        m_player.GetBombPlacementRotation()
    );
}