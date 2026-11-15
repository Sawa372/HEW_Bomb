#include "SceneGame.h"

#include "Geometory.h"
#include "CameraDebug.h"
#include "controller.h"
#include "GridCollision.h"
#include "GridPosition.h"
#include "BombType.h"

#include <ShaderList.h>
#include <SDL3/SDL.h>
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

	// マップ描画クラスを初期化
	if (!m_mapRenderer.Initialize())
	{
		MessageBox(
			NULL,
			"MapRendererの初期化に失敗しました",
			"Error",
			MB_OK
		);
	}

	// ==========================================
	//  プレイヤーの初期化と配置
	// ==========================================
	m_player.Initialize(GridPosition(0, 0, 0)); // 初期位置を (0,0,0) に設定
	m_map.GetGridManager().SetPlayer(m_player.GetPosition()); // マップ側にプレイヤーをセット
	Controller::Initialize();
    // 最初の爆弾を選択
    m_bombInventory.SelectBomb(
        GetBombTypeByIndex(0)
    );


    //==================================================
    // プレイヤーの初期化

    m_player.Initialize(GridPosition(0, 0, 0));

    // マップ側にプレイヤーの位置を登録
    m_map.GetGridManager().SetPlayer(
        m_player.GetPosition()
    );


    //==================================================
    // 爆発範囲プレビューの初期化

    UpdateBombPreview();
}


//==================================================
// デストラクタ

SceneGame::~SceneGame()
{
	Controller::Finalize();

	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}


//==================================================
// 更新処理

void SceneGame::Update()
{
    //==================================================
    // カメラの更新

    m_pCamera->Update();


    //==================================================
    // プレイヤーの更新

    // マップの情報を使って当たり判定を行う
    GridCollision gridCollision(m_map.GetGridManager());

    /*m_player.Update(
        m_map,
		m_bombInventory,/////////////////////////////Player.cppを書き換えたら動くはず！(多分！！！)
        gridCollision
    );*/


    //==================================================
    // 爆発範囲プレビューの更新

    UpdateBombPreview();
	// I, J, K, L キーの入力を受け取って移動処理を実行
	m_player.Update(m_map.GetGridManager(), gridCollision);

	// コントローラーの状態を更新
	Controller::Update();

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			break;
		}

		Controller::ProcessEvent(event);
	}

	// SDLイベントを取得
	SDL_PumpEvents();
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
   /* m_mapRenderer.SetExplosionPreview(
        m_bombInventory.GetSelectedType(),
        m_player.GetBombPlacementPosition(),/////////////////////////////これもPlayer.cppを書き換えたら動くはず！
        m_player.GetBombPlacementRotation()
    );*/
}