#include "GridCollision.h"

GridCollision::GridCollision(GridManager& gridManager)
    : m_gridManager(gridManager)
{

}

GridCollision::~GridCollision()
{

}

// ==========================================
// 1. 移動・通行判定
// =========================================

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

// ==========================================
// 2. セルの状態確認
// ==========================================

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

bool GridCollision::HasFloor(const DirectX::XMFLOAT3& worldPos) const
{
    GridPosition gridPos;
    if (!m_gridManager.WorldToGrid(worldPos, gridPos))
    {
        return false;
    }
    return HasFloor(gridPos);
}

bool GridCollision::HasFloor(const GridPosition& gridPos) const
{
    return m_gridManager.HasFloor(gridPos);
}

// ==========================================
// 3. 配置可能判定
// ==========================================

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

bool GridCollision::CanPlacePlayer(const DirectX::XMFLOAT3& worldPos) const
{
    GridPosition gridPos;
    if (!m_gridManager.WorldToGrid(worldPos, gridPos))
    {
        return false;
    }
    return CanPlacePlayer(gridPos);
}

bool GridCollision::CanPlacePlayer(const GridPosition& gridPos) const
{
    return m_gridManager.CanPlacePlayer(gridPos);
}

// ==========================================
// 4. 範囲内判定
// ==========================================
bool GridCollision::IsInside(const DirectX::XMFLOAT3& worldPos) const
{
    GridPosition gridPos;
    return m_gridManager.WorldToGrid(worldPos, gridPos);
}
bool GridCollision::IsInside(const GridPosition& gridPos) const
{
    return m_gridManager.IsInside(gridPos);
}

// ==========================================
// 5. 演算子オーバーロードによる bool 判定
// ==========================================

bool GridCollision::operator()(const DirectX::XMFLOAT3& worldPos) const
{
    return CanMove(worldPos);
}

bool GridCollision::operator()(const GridPosition& gridPos) const
{
    return CanMove(gridPos);
}

