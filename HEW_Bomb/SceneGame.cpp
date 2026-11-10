#include "SceneGame.h"
#include "Geometory.h"
#include "CameraDebug.h"


#include <ShaderList.h>


SceneGame::SceneGame()
{
	m_pGround = new Ground;
	m_pGoal = new Goal;
	m_pCamera = new CameraDebug;
	m_pModel = new Model();

	if(!m_pModel->Load("Assets/Model/tree_forest.fbx",0.5f))  // 倍率と反転は省略可 
	{
		MessageBox(NULL, "tree_forest.fbx読込失敗", "Error", MB_OK);
	}

	m_pGround->SetPos(0.0f, 0.0f, 0.0f);
	m_pGround->SetScale(3.0f, 0.4f, 3.0f);
	
	m_pGoal->SetPos(3.0f, 1.0f, 0.0f);
	m_pGoal->SetScale(0.5f, 0.5f, 0.5f);

	// マップを初期化
	if (!m_map.Initialize())
	{
		MessageBox(NULL, "マップの初期化に失敗しました", "Error", MB_OK);
	}

	// グリッドの設定
	GridManager& grid = m_map.GetGridManager();

	// 1マスのワールドサイズ
	grid.SetCellSize(1.0f, 1.0f, 1.0f);

	// グリッドの原点
	grid.SetGridOrigin(
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)
	);

	// WorldToGridの動作確認
	GridPosition result;

	if (!grid.WorldToGrid(
		DirectX::XMFLOAT3(-3.2f, 0.0f, -4.1f),
		result))
	{
		MessageBox(NULL, "WorldToGrid失敗", "Error", MB_OK);
	}
	else if (result != GridPosition(-3, 0, -4))
	{
		MessageBox(NULL, "座標変換が正しくありません", "Error", MB_OK);
	}
}

SceneGame::~SceneGame()
{
	if (m_pModel) {
		delete m_pModel;
		m_pModel = nullptr;
	}

	if (m_pCamera) {
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pGoal) {
		delete m_pGoal;
		m_pGoal = nullptr;
	}

	if (m_pGround) {
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

	//m_pGround->Draw();
	//m_pGoal->Draw();

	if (m_pModel)
	{
		ShaderList::SetView(m_pCamera->GetViewMatrix());
		ShaderList::SetProjection(m_pCamera->GetProjectionMatrix());

		GridManager& grid = m_map.GetGridManager();

		std::vector<GridPosition> objectPositions =
			grid.GetObjectPositions();

		for (const GridPosition& position : objectPositions)
		{
			DirectX::XMFLOAT3 worldPosition;

			// グリッド座標をワールド座標へ変換
			if (!grid.GridToWorld(position, worldPosition))
			{
				continue;
			}

			// ワールド行列を作成
			DirectX::XMMATRIX world =
				DirectX::XMMatrixTranslation(
					worldPosition.x,
					worldPosition.y,
					worldPosition.z
				);

			DirectX::XMFLOAT4X4 fWorld;
			DirectX::XMStoreFloat4x4(&fWorld, world);

			// ワールド行列を設定
			ShaderList::SetWorld(fWorld);

			// シェーダーを設定
			m_pModel->SetVertexShader(
				ShaderList::GetVS(ShaderList::VS_WORLD)
			);

			m_pModel->SetPixelShader(
				ShaderList::GetPS(ShaderList::PS_LAMBERT)
			);

			// モデルを描画
			for (int i = 0; i < m_pModel->GetMeshNum(); ++i)
			{
				const Model::Mesh* mesh = m_pModel->GetMesh(i);

				Model::Material material =
					*m_pModel->GetMaterial(mesh->materialID);

				ShaderList::SetMaterial(material);

				m_pModel->Draw(i);
			}
		}

		// 壁の配置を取得
		std::vector<GridPosition> wallPositions =
			grid.GetWallPositions();

		for (const GridPosition& position : wallPositions)
		{
			DirectX::XMFLOAT3 worldPosition;

			if (!grid.GridToWorld(position, worldPosition))
			{
				continue;
			}

			DirectX::XMMATRIX world =
				DirectX::XMMatrixTranslation(
					worldPosition.x,
					worldPosition.y,
					worldPosition.z
				);

			DirectX::XMFLOAT4X4 fWorld;
			DirectX::XMStoreFloat4x4(&fWorld, world);

			ShaderList::SetWorld(fWorld);

			m_pModel->SetVertexShader(
				ShaderList::GetVS(ShaderList::VS_WORLD)
			);

			m_pModel->SetPixelShader(
				ShaderList::GetPS(ShaderList::PS_LAMBERT)
			);

			for (int i = 0; i < m_pModel->GetMeshNum(); ++i)
			{
				const Model::Mesh* mesh = m_pModel->GetMesh(i);

				Model::Material material =
					*m_pModel->GetMaterial(mesh->materialID);

				ShaderList::SetMaterial(material);

				m_pModel->Draw(i);
			}
		}
	}
}
