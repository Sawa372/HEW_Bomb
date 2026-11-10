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
}

void SceneGame::Draw()
{
	// 深度テストの有効化 
	SetDepthTest(DEPTH_ENABLE);

	// 仮置きのボックスにカメラ設定
	Geometory::SetView(m_pCamera->GetViewMatrix());
	Geometory::SetProjection(m_pCamera->GetProjectionMatrix());

	m_pGround->Draw();
	m_pGoal->Draw();

	// マップを描画
	m_mapRenderer.Draw(
		m_map.GetGridManager(),
		m_pCamera->GetViewMatrix(),
		m_pCamera->GetProjectionMatrix()
	);
}
