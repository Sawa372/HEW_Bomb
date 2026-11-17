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

// 爆発の種類と爆発範囲を設定する
void Explosion::SetType(BombType type)
{
    m_type = type;

    // 種類ごとの爆発範囲を設定
    switch (m_type)
    {
    case BombType::Cube:
        // 3 × 1 × 3 = 9マス
        m_size = DirectX::XMINT3(3, 1, 3);
        break;

    case BombType::WideCube:
        // 4 × 1 × 2 = 8マス
        m_size = DirectX::XMINT3(4, 1, 2);
        break;

    case BombType::Cylinder:
        // 円柱状
        m_size = DirectX::XMINT3(5, 3, 5);
        break;

    case BombType::VShape:
        // V字型
        m_size = DirectX::XMINT3(3, 1, 2);
        break;

    case BombType::Circle:
        // 円形（平面）
        m_size = DirectX::XMINT3(5, 1, 5);
        break;

    case BombType::Cross:
        // 固定された5マスの十字形状
        m_size = DirectX::XMINT3(3, 3, 1);
        break;

    case BombType::InvertedPyramid:
        // 逆ピラミッド型
        m_size = DirectX::XMINT3(5, 3, 5);
        break;

    default:
        // Noneなど
        m_size = DirectX::XMINT3(1, 1, 1);
        break;
    }
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
    case BombType::Circle:
    {
        // 円柱・円型の中心位置
        float centerX = (m_size.x - 1) * 0.5f;
        float centerZ = (m_size.z - 1) * 0.5f;

        // 円の直径
        int diameter = (m_size.x < m_size.z)
            ? m_size.x
            : m_size.z;

        // 円の半径
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
    case BombType::VShape:
    {
        // V字は左右対称にするため、幅は奇数にする
        if (m_size.x < 3 ||
            m_size.x % 2 == 0 ||
            m_size.z < 2)
        {
            return false;
        }

        // 高さの判定
        bool insideHeight =
            localPosition.y >= -0.5f - epsilon &&
            localPosition.y <= m_size.y - 0.5f + epsilon;

        if (!insideHeight)
        {
            return false;
        }

        // V字の各段を調べる
        int maxOffset = (m_size.x - 1) / 2;

        for (int row = 0; row < m_size.z; ++row)
        {
            // 奥に進むほど左右に広がる
            int offset = static_cast<int>(
                (static_cast<float>(row) * maxOffset /
                    (m_size.z - 1)) + 0.5f
                );

            // 現在の段のZ座標に近いか
            if (std::fabs(
                localPosition.z - row) > 0.5f + epsilon)
            {
                continue;
            }

            // 先端部分は中央の1マス
            if (row == 0)
            {
                if (std::fabs(localPosition.x) <=
                    0.5f + epsilon)
                {
                    return true;
                }
            }
            // それ以外は左右の2マス
            else
            {
                bool left =
                    std::fabs(localPosition.x + offset)
                    <= 0.5f + epsilon;

                bool right =
                    std::fabs(localPosition.x - offset)
                    <= 0.5f + epsilon;

                if (left || right)
                {
                    return true;
                }
            }
        }
        return false;
    }

    case BombType::InvertedPyramid:
    {
        // 正方形の底面にするための条件確認
        // X・Zの幅は同じ、奇数、高さは2以上
        if (m_size.x < 3 ||
            m_size.x != m_size.z ||
            m_size.x % 2 == 0 ||
            m_size.y < 2)
        {
            return false;
        }

        // 現在の高さの段を取得
        int layer = static_cast<int>(
            std::floor(localPosition.y + 0.5f)
            );

        // 高さの範囲外なら対象外
        if (layer < 0 || layer >= m_size.y)
        {
            return false;
        }

        // 現在の段の高さ範囲
        bool insideHeight =
            localPosition.y >= layer - 0.5f - epsilon &&
            localPosition.y <= layer + 0.5f + epsilon;

        if (!insideHeight)
        {
            return false;
        }

        // 最上段の最大半径
        float maxRadius =
            static_cast<float>((m_size.x - 1) / 2);

        // 高さに応じて断面を大きくする
        float radius =
            maxRadius * static_cast<float>(layer) /
            static_cast<float>(m_size.y - 1);

        // マス単位の大きさに丸める
        radius = std::floor(radius + 0.5f);

        // 現在の高さの四角形に入っているか
        bool insideX =
            std::fabs(localPosition.x) <=
            radius + 0.5f + epsilon;

        bool insideZ =
            std::fabs(localPosition.z) <=
            radius + 0.5f + epsilon;

        return insideX && insideZ;
    }

    case BombType::Cross:
    {
        // 爆弾の設置位置を基準にしたXY平面の十字

        // 爆弾の位置
        bool bombPosition =
            std::fabs(localPosition.x) <= 0.5f + epsilon &&
            std::fabs(localPosition.y) <= 0.5f + epsilon &&
            std::fabs(localPosition.z) <= 0.5f + epsilon;

        // Y=1の中央
        bool center =
            std::fabs(localPosition.x) <= 0.5f + epsilon &&
            std::fabs(localPosition.y - 1.0f) <= 0.5f + epsilon &&
            std::fabs(localPosition.z) <= 0.5f + epsilon;

        // Y=1のX方向
        bool left =
            std::fabs(localPosition.x + 1.0f) <= 0.5f + epsilon &&
            std::fabs(localPosition.y - 1.0f) <= 0.5f + epsilon &&
            std::fabs(localPosition.z) <= 0.5f + epsilon;

        bool right =
            std::fabs(localPosition.x - 1.0f) <= 0.5f + epsilon &&
            std::fabs(localPosition.y - 1.0f) <= 0.5f + epsilon &&
            std::fabs(localPosition.z) <= 0.5f + epsilon;

        // Y=2
        bool top =
            std::fabs(localPosition.x) <= 0.5f + epsilon &&
            std::fabs(localPosition.y - 2.0f) <= 0.5f + epsilon &&
            std::fabs(localPosition.z) <= 0.5f + epsilon;

        return
            bombPosition ||
            center ||
            left ||
            right ||
            top;
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