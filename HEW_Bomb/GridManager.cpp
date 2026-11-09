#include <cstdlib>

#include "GridManager.h"

GridManager::GridManager()
    : m_width(0)
    , m_height(0)
    , m_depth(0)
    , m_cellSize(1.0f, 1.0f, 1.0f)
    , m_gridOrigin(0.0f, 0.0f, 0.0f)
{
}

GridManager::~GridManager()
{
    Clear();
}

bool GridManager::Initialize(int width, int height, int depth)
{
    if (width <= 0 || height <= 0 || depth <= 0)
    {
        return false;
    }

    Clear();

    m_width = width;
    m_height = height;
    m_depth = depth;

    m_cells.reserve(width * height * depth);

    for (int y = 0; y < height; ++y)
    {
        for (int z = 0; z < depth; ++z)
        {
            for (int x = 0; x < width; ++x)
            {
                m_cells.emplace_back(GridPosition(x, y, z));
            }
        }
    }

    return true;
}

void GridManager::Clear()
{
    m_cells.clear();

    m_width = 0;
    m_height = 0;
    m_depth = 0;
}

int GridManager::GetWidth() const
{
    return m_width;
}

int GridManager::GetHeight() const
{
    return m_height;
}

int GridManager::GetDepth() const
{
    return m_depth;
}

bool GridManager::IsInside(const GridPosition& position) const
{
    return position.GetX() >= 0
        && position.GetX() < m_width
        && position.GetY() >= 0
        && position.GetY() < m_height
        && position.GetZ() >= 0
        && position.GetZ() < m_depth;
}

GridCell* GridManager::GetCell(const GridPosition& position)
{
    if (!IsInside(position))
    {
        return nullptr;
    }

    return &m_cells[GetIndex(position)];
}

const GridCell* GridManager::GetCell(const GridPosition& position) const
{
    if (!IsInside(position))
    {
        return nullptr;
    }

    return &m_cells[GetIndex(position)];
}

bool GridManager::IsWall(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    if (cell == nullptr)
    {
        return true;
    }

    return cell->IsWall();
}

bool GridManager::HasObject(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    if (cell == nullptr)
    {
        return false;
    }

    return cell->HasObject();
}

bool GridManager::HasBomb(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    if (cell == nullptr)
    {
        return false;
    }

    return cell->HasBomb();
}

bool GridManager::HasPlayer(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    if (cell == nullptr)
    {
        return false;
    }

    return cell->HasPlayer();
}

bool GridManager::CanMove(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    if (cell == nullptr)
    {
        return false;
    }

    // 通行できる状態で、プレイヤーもいないこと
    return cell->IsWalkable() && !cell->HasPlayer();
}

bool GridManager::SetWall(const GridPosition& position)
{
    if (!CanPlaceWall(position))
    {
        return false;
    }

    GridCell* cell = GetCell(position);
    cell->SetWall(true);

    return true;
}

bool GridManager::RemoveWall(const GridPosition& position)
{
    GridCell* cell = GetCell(position);

    if (cell == nullptr || !cell->IsWall())
    {
        return false;
    }

    cell->SetWall(false);

    return true;
}

bool GridManager::SetObject(const GridPosition& position)
{
    if (!CanPlaceObject(position))
    {
        return false;
    }

    GridCell* cell = GetCell(position);
    cell->SetObject(true);

    return true;
}

bool GridManager::RemoveObject(const GridPosition& position)
{
    GridCell* cell = GetCell(position);

    if (cell == nullptr || !cell->HasObject())
    {
        return false;
    }

    cell->SetObject(false);

    return true;
}

std::vector<GridPosition> GridManager::GetObjectPositions() const
{
    std::vector<GridPosition> positions;

    for (const GridCell& cell : m_cells)
    {
        if (cell.HasObject())
        {
            positions.push_back(cell.GetPosition());
        }
    }

    return positions;
}

bool GridManager::SetBomb(const GridPosition& position)
{
    if (!CanPlaceBomb(position))
    {
        return false;
    }

    GridCell* cell = GetCell(position);

    cell->SetBomb(true);

    return true;
}

bool GridManager::RemoveBomb(const GridPosition& position)
{
    GridCell* cell = GetCell(position);

    if (cell == nullptr || !cell->HasBomb())
    {
        return false;
    }

    cell->SetBomb(false);

    return true;
}

bool GridManager::SetPlayer(const GridPosition& position)
{
    if (!CanPlacePlayer(position))
    {
        return false;
    }

    GridCell* cell = GetCell(position);
    cell->SetPlayer(true);

    return true;
}

bool GridManager::RemovePlayer(const GridPosition& position)
{
    GridCell* cell = GetCell(position);

    if (cell == nullptr || !cell->HasPlayer())
    {
        return false;
    }

    cell->SetPlayer(false);

    return true;
}

int GridManager::GetIndex(const GridPosition& position) const
{
    return position.GetY() * m_depth * m_width
        + position.GetZ() * m_width
        + position.GetX();
}

bool GridManager::MovePlayer(
    const GridPosition& currentPosition,
    const GridPosition& nextPosition)
{
    // 現在地から移動先までの距離を計算
    int dx = std::abs(nextPosition.GetX() - currentPosition.GetX());
    int dy = std::abs(nextPosition.GetY() - currentPosition.GetY());
    int dz = std::abs(nextPosition.GetZ() - currentPosition.GetZ());

    // 隣接する1マス以外への移動は禁止
    if (dx + dy + dz != 1)
    {
        return false;
    }

    // 現在地を取得
    GridCell* currentCell = GetCell(currentPosition);

    if (currentCell == nullptr || !currentCell->HasPlayer())
    {
        return false;
    }

    // 移動先が通行可能か確認
    if (!CanMove(nextPosition))
    {
        return false;
    }

    GridCell* nextCell = GetCell(nextPosition);

    if (nextCell == nullptr)
    {
        return false;
    }

    // 現在地からプレイヤーを取り除く
    currentCell->SetPlayer(false);

    // 移動先にプレイヤーを配置
    nextCell->SetPlayer(true);

    return true;
}

bool GridManager::CanPlaceBomb(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    if (cell == nullptr)
    {
        return false;
    }

    // 壁・オブジェクト・爆弾がある場所には設置できない
    // プレイヤーがいる場所には設置できる
    return !cell->IsWall()
        && !cell->HasObject()
        && !cell->HasBomb();
}

// 壁を配置できるか
bool GridManager::CanPlaceWall(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    if (cell == nullptr)
    {
        return false;
    }

    return !cell->IsWall()
        && !cell->HasObject()
        && !cell->HasBomb()
        && !cell->HasPlayer();
}

// オブジェクトを配置できるか
bool GridManager::CanPlaceObject(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    if (cell == nullptr)
    {
        return false;
    }

    return !cell->IsWall()
        && !cell->HasObject()
        && !cell->HasBomb()
        && !cell->HasPlayer();
}

// プレイヤーを配置できるか
bool GridManager::CanPlacePlayer(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    if (cell == nullptr)
    {
        return false;
    }

    return cell->IsWalkable()
        && !cell->HasPlayer();
}

// 1マスのワールドサイズを設定
bool GridManager::SetCellSize(float x, float y, float z)
{
    // 0以下のサイズは設定できない
    if (x <= 0.0f || y <= 0.0f || z <= 0.0f)
    {
        return false;
    }

    m_cellSize = DirectX::XMFLOAT3(x, y, z);

    return true;
}

// グリッドの原点を設定
void GridManager::SetGridOrigin(
    const DirectX::XMFLOAT3& origin)
{
    m_gridOrigin = origin;
}

// グリッド座標をワールド座標に変換
bool GridManager::GridToWorld(
    const GridPosition& position,
    DirectX::XMFLOAT3& worldPosition
) const
{
    // グリッドの範囲外なら変換しない
    if (!IsInside(position))
    {
        return false;
    }

    worldPosition.x =
        m_gridOrigin.x + position.GetX() * m_cellSize.x;

    worldPosition.y =
        m_gridOrigin.y + position.GetY() * m_cellSize.y;

    worldPosition.z =
        m_gridOrigin.z + position.GetZ() * m_cellSize.z;

    return true;
}