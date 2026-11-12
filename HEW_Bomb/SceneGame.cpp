#include "SceneGame.h"
#include "Geometory.h"
#include "CameraDebug.h"


#include <ShaderList.h>


SceneGame::SceneGame()
{
	m_pGround = new Ground;
	m_pGoal = new Goal;
	m_pCamera = new CameraDebug;

	m_pGround->SetPos(0.0f, 0.0f, 0.0f);
	m_pGround->SetScale(3.0f, 0.4f, 3.0f);

	m_pGoal->SetPos(3.0f, 1.0f, 0.0f);
	m_pGoal->SetScale(0.5f, 0.5f, 0.5f);

	// マップを初期化
	if (!m_map.Initialize())
	{
		MessageBox(NULL, "マップの初期化に失敗しました", "Error", MB_OK);
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
}

SceneGame::~SceneGame()
{
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pGoal)
	{
		delete m_pGoal;
		m_pGoal = nullptr;
	}

	if (m_pGround)
	{
		delete m_pGround;
		m_pGround = nullptr;
	}
}

void SceneGame::Update()
{
	m_pGround->Update();
	m_pGoal->Update();
	m_pCamera->Update();

	// ==========================================
	//  毎フレームの更新でプレイヤーを動かす
	// ==========================================
	GridCollision gridCollision(m_map.GetGridManager());
	//gridCollision.Initialize(m_map.GetGridManager()); // マップの情報を元に当たり判定を準備

	// I, J, K, L キーの入力を受け取って移動処理を実行
	m_player.Update(m_map.GetGridManager(), gridCollision);
}

void SceneGame::Draw()
{
	// 深度テストの有効化 
	SetDepthTest(DEPTH_ENABLE);

	// 仮置きのボックスにカメラ設定
	Geometory::SetView(m_pCamera->GetViewMatrix());
	Geometory::SetProjection(m_pCamera->GetProjectionMatrix());

	//m_pGround->Draw();
	//m_pGoal->Draw();

	// マップを描画
	m_mapRenderer.Draw(
		m_map.GetGridManager(),
		m_pCamera->GetViewMatrix(),
		m_pCamera->GetProjectionMatrix()
	);
}
