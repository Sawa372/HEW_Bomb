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

	m_pGround->Draw();
	m_pGoal->Draw();

	if (m_pModel)
	{
		// モデルを表示するためのワールド行列を計算
		DirectX::XMFLOAT4X4 fWorld;
		DirectX::XMMATRIX world;

		//// 変換行列の計算
		//DirectX::XMMATRIX T =
		//	DirectX::XMMatrixTranslation(
		//		0.0f, 0.0f, 0.0f);
		//DirectX::XMMATRIX S =
		//	DirectX::XMMatrixScaling(
		//		1.0f, 1.0f, 1.0f);

		//world = S * T;

		// 変換行列をGPUに渡すための前処理
		world = DirectX::XMMatrixIdentity();
		DirectX::XMStoreFloat4x4(&fWorld, world);

		// シェーダーに必要なデータを設定 
		ShaderList::SetWorld(fWorld);
		ShaderList::SetView(m_pCamera->GetViewMatrix());
		ShaderList::SetProjection(m_pCamera->GetProjectionMatrix());

		// モデルに使用する各種シェーダーを設定 
		m_pModel->SetVertexShader(ShaderList::GetVS(ShaderList::VS_WORLD));
		m_pModel->SetPixelShader(ShaderList::GetPS(ShaderList::PS_LAMBERT));

		// メッシュ別にモデルを表示 
		for (int i = 0; i < m_pModel->GetMeshNum(); ++i)
		{

			// モデルのメッシュを取得 
			const Model::Mesh* mesh = m_pModel->GetMesh(i);

			// メッシュに割り当てられているマテリアルを取得 
			Model::Material material = *m_pModel->GetMaterial(mesh->materialID);

			// シェーダーへマテリアルを設定 
			ShaderList::SetMaterial(material);

			// モデルの描画(内部的に一括で行っていた描画処理を、メッシュごとの描画処理に変更) 
			m_pModel->Draw(i);
		}
	}
}
