#include "GridCollision.h"

GridCollision::GridCollision(GridManager& gridManager)
    : m_gridManager(gridManager)
{
}

GridCollision::~GridCollision()
{
}

// --- ワールド座標ベースのオーバーロード ---

bool GridCollision::CanMove(const DirectX::XMFLOAT3& worldPos) const
{
    GridPosition gridPos;
    if (!m_gridManager.WorldToGrid(worldPos, gridPos))
    {
        return false; // 変換失敗（範囲外など）
    }
    return CanMove(gridPos);
}

bool GridCollision::CanMove(const GridPosition& gridPos) const
{
    return m_gridManager.CanMove(gridPos);
}

bool GridCollision::IsWall(const DirectX::XMFLOAT3& worldPos) const
{
    GridPosition gridPos;
    if (!m_gridManager.WorldToGrid(worldPos, gridPos))
    {
        return true; // 範囲外は壁とみなす
    }
    return IsWall(gridPos);
}

bool GridCollision::IsWall(const GridPosition& gridPos) const
{
    return m_gridManager.IsWall(gridPos);
}

bool GridCollision::HasObject(const DirectX::XMFLOAT3& worldPos) const
{
    GridPosition gridPos;
    if (!m_gridManager.WorldToGrid(worldPos, gridPos))
    {
        return false;
    }
    return HasObject(gridPos);
}

bool GridCollision::HasObject(const GridPosition& gridPos) const
{
    return m_gridManager.HasObject(gridPos);
}

bool GridCollision::HasBomb(const DirectX::XMFLOAT3& worldPos) const
{
    GridPosition gridPos;
    if (!m_gridManager.WorldToGrid(worldPos, gridPos))
    {
        return false;
    }
    return HasBomb(gridPos);
}

bool GridCollision::HasBomb(const GridPosition& gridPos) const
{
    return m_gridManager.HasBomb(gridPos);
}

bool GridCollision::HasPlayer(const DirectX::XMFLOAT3& worldPos) const
{
    GridPosition gridPos;
    if (!m_gridManager.WorldToGrid(worldPos, gridPos))
    {
        return false;
    }
    return HasPlayer(gridPos);
}

bool GridCollision::HasPlayer(const GridPosition& gridPos) const
{
    return m_gridManager.HasPlayer(gridPos);
}

// --- 設置判定 ---

bool GridCollision::CanPlaceBomb(const DirectX::XMFLOAT3& worldPos) const
{
    GridPosition gridPos;
    if (!m_gridManager.WorldToGrid(worldPos, gridPos))
    {
        return false;
    }
    return CanPlaceBomb(gridPos);
}

bool GridCollision::CanPlaceBomb(const GridPosition& gridPos) const
{
    return m_gridManager.CanPlaceBomb(gridPos);
}

bool GridCollision::CanPlaceWall(const DirectX::XMFLOAT3& worldPos) const
{
    GridPosition gridPos;
    if (!m_gridManager.WorldToGrid(worldPos, gridPos))
    {
        return false;
    }
    return CanPlaceWall(gridPos);
}

bool GridCollision::CanPlaceWall(const GridPosition& gridPos) const
{
    return m_gridManager.CanPlaceWall(gridPos);
}

bool GridCollision::CanPlaceObject(const DirectX::XMFLOAT3& worldPos) const
{
    GridPosition gridPos;
    if (!m_gridManager.WorldToGrid(worldPos, gridPos))
    {
        return false;
    }
    return CanPlaceObject(gridPos);
}

bool GridCollision::CanPlaceObject(const GridPosition& gridPos) const
{
    return m_gridManager.CanPlaceObject(gridPos);
}

// --- 演算子オーバーロードによる bool 判定 ---

bool GridCollision::operator()(const GridPosition& gridPos) const
{
    return CanMove(gridPos);
}

bool GridCollision::operator()(const DirectX::XMFLOAT3& worldPos) const
{
    return CanMove(worldPos);
}