#include "Explosion.h"
#include "GridManager.h"

#include <cmath>

Explosion::Explosion()
    : m_center(0.0f, 0.0f, 0.0f)
    , m_radius(0.0f)
{
}

Explosion::~Explosion()
{
}

void Explosion::SetCenter(
    const DirectX::XMFLOAT3& center)
{
    m_center = center;
}

void Explosion::SetRadius(float radius)
{
    if (radius < 0.0f)
    {
        m_radius = 0.0f;
        return;
    }

    m_radius = radius;
}

bool Explosion::IsInside(
    const DirectX::XMFLOAT3& position) const
{
    float dx = position.x - m_center.x;
    float dy = position.y - m_center.y;
    float dz = position.z - m_center.z;

    float distanceSquared =
        dx * dx +
        dy * dy +
        dz * dz;

    return distanceSquared <=
        m_radius * m_radius;
}

std::vector<GridPosition> Explosion::GetAffectedPositions(
    const GridManager& grid
) const
{
    std::vector<GridPosition> positions;

    // グリッド内の全てのマスを確認
    const std::vector<GridCell>& cells = grid.GetCells();

    for (const GridCell& cell : cells)
    {
        DirectX::XMFLOAT3 worldPosition;

        // グリッド座標をワールド座標に変換
        if (!grid.GridToWorld(
            cell.GetPosition(),
            worldPosition))
        {
            continue;
        }

        // 爆発範囲内か確認
        if (IsInside(worldPosition))
        {
            positions.push_back(cell.GetPosition());
        }
    }

    return positions;
}