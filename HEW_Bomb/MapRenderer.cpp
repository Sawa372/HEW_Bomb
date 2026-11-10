#include "MapRenderer.h"
#include "GridManager.h"
#include "Model.h"
#include "GridCell.h"
#include "GridObjectType.h"

#include <ShaderList.h>
#include <Windows.h>

// コンストラクタ
MapRenderer::MapRenderer()
    : m_pWallModel(nullptr)
    , m_pDefaultModel(nullptr)
    , m_pTreeModel(nullptr)
    , m_pRockModel(nullptr)
    , m_pBoxModel(nullptr)
    , m_pPillarModel(nullptr)
    , m_pOilDrumModel(nullptr)
{
}

// デストラクタ
MapRenderer::~MapRenderer()
{
    Clear();
}

// 初期化
bool MapRenderer::Initialize()
{
    // 既存のモデルを解放
    Clear();

    // 壁用モデル
    m_pWallModel = new Model();
    if (!m_pWallModel->Load(
        "Assets/Model/tree_forest.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "壁モデルの読み込みに失敗しました。\n(tree_forest.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    // Defaultモデル
    m_pDefaultModel = new Model();
    if (!m_pDefaultModel->Load(
        "Assets/Model/plantB_forest.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "Defaultモデルの読み込みに失敗しました。\n(plantB_forest.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    // Treeモデル
    m_pTreeModel = new Model();
    if (!m_pTreeModel->Load(
        "Assets/Model/tree_forest.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "Treeモデルの読み込みに失敗しました。\n(tree_forest.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    // Rockモデル
    m_pRockModel = new Model();
    if (!m_pRockModel->Load(
        "Assets/Model/rocksA_desert.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "Rockモデルの読み込みに失敗しました。\n(rocksA_desert.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    // Boxモデル
    m_pBoxModel = new Model();
    if (!m_pBoxModel->Load(
        "Assets/Model/flag_teamBlue.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "Boxモデルの読み込みに失敗しました。\n(flag_teamBlue.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    // Pillarモデル
    m_pPillarModel = new Model();
    if (!m_pPillarModel->Load(
        "Assets/Model/flag_teamRed.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "Pillarモデルの読み込みに失敗しました。\n(flag_teamRed.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    // OilDrumモデル
    m_pOilDrumModel = new Model();
    if (!m_pOilDrumModel->Load(
        "Assets/Model/flag_teamYellow.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "OilDrumモデルの読み込みに失敗しました。\n(flag_teamYellow.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    return true;
}

// モデルの解放
void MapRenderer::Clear()
{
    delete m_pWallModel;
    m_pWallModel = nullptr;

    delete m_pDefaultModel;
    m_pDefaultModel = nullptr;

    delete m_pTreeModel;
    m_pTreeModel = nullptr;

    delete m_pRockModel;
    m_pRockModel = nullptr;

    delete m_pBoxModel;
    m_pBoxModel = nullptr;

    delete m_pPillarModel;
    m_pPillarModel = nullptr;

    delete m_pOilDrumModel;
    m_pOilDrumModel = nullptr;
}

// マップの描画
void MapRenderer::Draw(
    const GridManager& grid,
    const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection)
{
    // カメラの設定
    ShaderList::SetView(view);
    ShaderList::SetProjection(projection);

    // 全セルを取得
    const std::vector<GridCell>& cells = grid.GetCells();

    for (const GridCell& cell : cells)
    {
        // 壁もオブジェクトもない場合はスキップ
        if (!cell.IsWall() && !cell.HasObject())
        {
            continue;
        }

        DirectX::XMFLOAT3 worldPosition;

        // グリッド座標をワールド座標へ変換
        if (!grid.GridToWorld(
            cell.GetPosition(),
            worldPosition))
        {
            continue;
        }

        // 壁を描画
        if (cell.IsWall())
        {
            DrawModel(m_pWallModel, worldPosition);
        }

        // オブジェクトを描画
        if (cell.HasObject())
        {
            Model* pModel = nullptr;

            // オブジェクトの種類からモデルを選択
            switch (cell.GetObjectType())
            {
            case GridObjectType::Default:
                pModel = m_pDefaultModel;
                break;

            case GridObjectType::Tree:
                pModel = m_pTreeModel;
                break;

            case GridObjectType::Box:
                pModel = m_pBoxModel;
                break;

            case GridObjectType::Rock:
                pModel = m_pRockModel;
                break;

            case GridObjectType::Pillar:
                pModel = m_pPillarModel;
                break;

            case GridObjectType::OilDrum:
                pModel = m_pOilDrumModel;
                break;

            default:
                break;
            }

            DrawModel(pModel, worldPosition);
        }
    }
}

// モデルの描画
void MapRenderer::DrawModel(
    Model* pModel,
    const DirectX::XMFLOAT3& worldPosition)
{
    if (pModel == nullptr)
    {
        return;
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
    pModel->SetVertexShader(
        ShaderList::GetVS(ShaderList::VS_WORLD)
    );

    pModel->SetPixelShader(
        ShaderList::GetPS(ShaderList::PS_LAMBERT)
    );

    // モデルのメッシュを描画
    for (int i = 0; i < pModel->GetMeshNum(); ++i)
    {
        const Model::Mesh* mesh = pModel->GetMesh(i);

        Model::Material material =
            *pModel->GetMaterial(mesh->materialID);

        ShaderList::SetMaterial(material);

        pModel->Draw(i);
    }
}