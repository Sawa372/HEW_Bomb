#include "Explosion.h"
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