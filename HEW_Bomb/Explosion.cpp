#include "Explosion.h"
#include "GridManager.h"

#include <cmath>

// コンストラクタ
Explosion::Explosion()
    : m_type(BombType::None)
    , m_anchor(0, 0, 0)
    , m_rotation(0.0f, 0.0f, 0.0f)
    , m_size(1, 1, 1)
{
}

// デストラクタ
Explosion::~Explosion()
{
}

// 爆発の種類
void Explosion::SetType(BombType type)
{
    m_type = type;
}

BombType Explosion::GetType() const
{
    return m_type;
}

// 爆発の基準位置
void Explosion::SetAnchor(const GridPosition& anchor)
{
    m_anchor = anchor;
}

const GridPosition& Explosion::GetAnchor() const
{
    return m_anchor;
}

// 回転角度
void Explosion::SetRotation(
    const DirectX::XMFLOAT3& rotation)
{
    m_rotation = rotation;
}

const DirectX::XMFLOAT3& Explosion::GetRotation() const
{
    return m_rotation;
}

// 爆発範囲のマス数
bool Explosion::SetSize(int x, int y, int z)
{
    if (x <= 0 || y <= 0 || z <= 0)
    {
        return false;
    }

    m_size = DirectX::XMINT3(x, y, z);

    return true;
}

const DirectX::XMINT3& Explosion::GetSize() const
{
    return m_size;
}

// 爆発範囲内か判定する
bool Explosion::IsInsideShape(
    const DirectX::XMFLOAT3& localPosition) const
{
    const float epsilon = 0.0001f;

    switch (m_type)
    {
    case BombType::Cube:
    case BombType::WideCube:

        return
            localPosition.x >= -0.5f - epsilon &&
            localPosition.x <= m_size.x - 0.5f + epsilon &&

            localPosition.y >= -0.5f - epsilon &&
            localPosition.y <= m_size.y - 0.5f + epsilon &&

            localPosition.z >= -0.5f - epsilon &&
            localPosition.z <= m_size.z - 0.5f + epsilon;

    case BombType::Cylinder:
    {
        // 円柱の中心位置
        float centerX = (m_size.x - 1) * 0.5f;
        float centerZ = (m_size.z - 1) * 0.5f;

        // 円柱の半径
        int diameter = (m_size.x < m_size.z)
            ? m_size.x
            : m_size.z;

        float radius = diameter * 0.5f - 0.25f;

        // 円の中心からの距離
        float dx = localPosition.x - centerX;
        float dz = localPosition.z - centerZ;

        float distanceSquared =
            dx * dx + dz * dz;

        // 高さ方向の判定
        bool insideHeight =
            localPosition.y >= -0.5f - epsilon &&
            localPosition.y <= m_size.y - 0.5f + epsilon;

        // 円形断面の判定
        bool insideCircle =
            distanceSquared <= radius * radius + epsilon;

        return insideHeight && insideCircle;
    }

    default:
        return false;
    
    }
}

// 爆発範囲内のグリッド座標を取得する
std::vector<GridPosition> Explosion::GetAffectedPositions(
    const GridManager& grid) const
{
    std::vector<GridPosition> positions;

    const std::vector<GridCell>& cells =
        grid.GetCells();

    // 度数法からラジアンに変換
    float pitch = DirectX::XMConvertToRadians(
        m_rotation.x);

    float yaw = DirectX::XMConvertToRadians(
        m_rotation.y);

    float roll = DirectX::XMConvertToRadians(
        m_rotation.z);

    // 回転行列
    DirectX::XMMATRIX rotationMatrix =
        DirectX::XMMatrixRotationRollPitchYaw(
            pitch, yaw, roll);

    // 逆回転行列
    DirectX::XMMATRIX inverseRotation =
        DirectX::XMMatrixTranspose(rotationMatrix);

    for (const GridCell& cell : cells)
    {
        const GridPosition& position =
            cell.GetPosition();

        // 基準位置からのグリッド座標の差
        float dx = static_cast<float>(
            position.GetX() - m_anchor.GetX());

        float dy = static_cast<float>(
            position.GetY() - m_anchor.GetY());

        float dz = static_cast<float>(
            position.GetZ() - m_anchor.GetZ());

        // ローカル座標に変換
        DirectX::XMVECTOR offset =
            DirectX::XMVectorSet(dx, dy, dz, 0.0f);

        DirectX::XMVECTOR local =
            DirectX::XMVector3TransformNormal(
                offset, inverseRotation);

        DirectX::XMFLOAT3 localPosition;

        DirectX::XMStoreFloat3(
            &localPosition, local);

        // 爆発範囲内なら登録
        if (IsInsideShape(localPosition))
        {
            positions.push_back(position);
        }
    }

    return positions;
}