#include <cstdlib>
#include <cmath>
#include <climits>

#include "GridManager.h"

GridManager::GridManager()
    : m_minPosition(0, 0, 0)
    , m_width(0)
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

// グリッドの範囲を取得
bool GridManager::GetBounds(
    GridPosition& minPosition,
    GridPosition& maxPosition
) const
{
    // グリッドが初期化されていない場合
    if (m_cells.empty())
    {
        return false;
    }

    // 最小座標
    minPosition = m_minPosition;

    // 最大座標
    maxPosition.Set(
        m_minPosition.GetX() + m_width - 1,
        m_minPosition.GetY() + m_height - 1,
        m_minPosition.GetZ() + m_depth - 1
    );

    return true;
}

bool GridManager::Initialize(int width, int height, int depth)
{
    if (width <= 0 || height <= 0 || depth <= 0)
    {
        return false;
    }

    return Initialize(
        GridPosition(0, 0, 0),
        GridPosition(width - 1, height - 1, depth - 1)
    );
}

bool GridManager::Initialize(
    const GridPosition& minPosition,
    const GridPosition& maxPosition
)
{
    // 最小座標と最大座標の大小関係を確認
    if (minPosition.GetX() > maxPosition.GetX() ||
        minPosition.GetY() > maxPosition.GetY() ||
        minPosition.GetZ() > maxPosition.GetZ())
    {
        return false;
    }

    // 各方向のマス数を計算
    long long width =
        static_cast<long long>(maxPosition.GetX()) -
        minPosition.GetX() + 1;

    long long height =
        static_cast<long long>(maxPosition.GetY()) -
        minPosition.GetY() + 1;

    long long depth =
        static_cast<long long>(maxPosition.GetZ()) -
        minPosition.GetZ() + 1;

    // インデックスで扱える範囲か確認
    if (width > INT_MAX || height > INT_MAX || depth > INT_MAX)
    {
        return false;
    }

    if (width * height > INT_MAX ||
        width * height * depth > INT_MAX)
    {
        return false;
    }

    // 古いグリッドを解放
    Clear();

    // 最小座標を保存
    m_minPosition = minPosition;

    // グリッドのサイズを保存
    m_width = static_cast<int>(width);
    m_height = static_cast<int>(height);
    m_depth = static_cast<int>(depth);

    // 必要なマス数を確保
    m_cells.reserve(
        static_cast<size_t>(m_width) *
        m_height *
        m_depth
    );

    // グリッドを作成
    for (int y = 0; y < m_height; ++y)
    {
        for (int z = 0; z < m_depth; ++z)
        {
            for (int x = 0; x < m_width; ++x)
            {
                m_cells.emplace_back(
                    GridPosition(
                        minPosition.GetX() + x,
                        minPosition.GetY() + y,
                        minPosition.GetZ() + z
                    )
                );
            }
        }
    }

    return true;
}


void GridManager::Clear()
{
    m_cells.clear();

    m_minPosition.Set(0, 0, 0);

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
    long long x = position.GetX();
    long long y = position.GetY();
    long long z = position.GetZ();

    long long minX = m_minPosition.GetX();
    long long minY = m_minPosition.GetY();
    long long minZ = m_minPosition.GetZ();

    return
        x >= minX && x < minX + m_width &&
        y >= minY && y < minY + m_height &&
        z >= minZ && z < minZ + m_depth;
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

// 全てのマスを取得
const std::vector<GridCell>& GridManager::GetCells() const
{
    return m_cells;
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

std::vector<GridPosition> GridManager::GetWallPositions() const
{
    std::vector<GridPosition> positions;
    for (const GridCell& cell : m_cells)
    {
        if (cell.IsWall())
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
    int x = position.GetX() - m_minPosition.GetX();
    int y = position.GetY() - m_minPosition.GetY();
    int z = position.GetZ() - m_minPosition.GetZ();

    return y * m_depth * m_width
        + z * m_width
        + x;
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

// ワールド座標をグリッド座標に変換
bool GridManager::WorldToGrid(
    const DirectX::XMFLOAT3& worldPosition,
    GridPosition& gridPosition
) const
{
    // グリッドが初期化されていない場合
    if (m_cells.empty())
    {
        return false;
    }

    // ワールド座標からグリッド座標を計算
    double x =
        (static_cast<double>(worldPosition.x) - m_gridOrigin.x)
        / m_cellSize.x;

    double y =
        (static_cast<double>(worldPosition.y) - m_gridOrigin.y)
        / m_cellSize.y;

    double z =
        (static_cast<double>(worldPosition.z) - m_gridOrigin.z)
        / m_cellSize.z;

    // 計算結果が正常か確認
    if (!std::isfinite(x) ||
        !std::isfinite(y) ||
        !std::isfinite(z))
    {
        return false;
    }

    // 最も近いグリッド座標に丸める
    x = std::round(x);
    y = std::round(y);
    z = std::round(z);

    // intで扱える範囲か確認
    if (x < INT_MIN || x > INT_MAX ||
        y < INT_MIN || y > INT_MAX ||
        z < INT_MIN || z > INT_MAX)
    {
        return false;
    }

    // グリッド座標に変換
    GridPosition result(
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(z)
    );

    // グリッドの範囲内か確認
    if (!IsInside(result))
    {
        return false;
    }

    gridPosition = result;

    return true;
}