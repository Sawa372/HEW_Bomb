#include "BombRotation.h"

#include <cmath>

// コンストラクタ
BombRotation::BombRotation()
{
    Reset();
}

// デストラクタ
BombRotation::~BombRotation()
{
}

// 回転を初期状態に戻す
void BombRotation::Reset()
{
    DirectX::XMStoreFloat4x4(
        &m_rotationMatrix,
        DirectX::XMMatrixIdentity()
    );

    m_eulerAngles =
        DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

// ワールドX軸を基準に90度回転
void BombRotation::RotateWorldX90()
{
    DirectX::XMMATRIX current =
        DirectX::XMLoadFloat4x4(&m_rotationMatrix);

    DirectX::XMMATRIX rotation =
        DirectX::XMMatrixRotationX(
            DirectX::XMConvertToRadians(90.0f)
        );

    DirectX::XMStoreFloat4x4(
        &m_rotationMatrix,
        DirectX::XMMatrixMultiply(current, rotation)
    );

    UpdateEulerAngles();
}

// ワールドY軸を基準に90度回転
void BombRotation::RotateWorldY90()
{
    DirectX::XMMATRIX current =
        DirectX::XMLoadFloat4x4(&m_rotationMatrix);

    DirectX::XMMATRIX rotation =
        DirectX::XMMatrixRotationY(
            DirectX::XMConvertToRadians(90.0f)
        );

    DirectX::XMStoreFloat4x4(
        &m_rotationMatrix,
        DirectX::XMMatrixMultiply(current, rotation)
    );

    UpdateEulerAngles();
}

// 回転行列から角度を更新する
void BombRotation::UpdateEulerAngles()
{
    auto IsNear = [](float a, float b)
        {
            return std::fabs(a - b) < 0.001f;
        };

    const DirectX::XMFLOAT4X4& target =
        m_rotationMatrix;

    // X・Y・Zそれぞれ90度刻みで候補を調べる
    for (int x = 0; x < 360; x += 90)
    {
        for (int y = 0; y < 360; y += 90)
        {
            for (int z = 0; z < 360; z += 90)
            {
                DirectX::XMMATRIX candidateMatrix =
                    DirectX::XMMatrixRotationRollPitchYaw(
                        DirectX::XMConvertToRadians(
                            static_cast<float>(x)
                        ),
                        DirectX::XMConvertToRadians(
                            static_cast<float>(y)
                        ),
                        DirectX::XMConvertToRadians(
                            static_cast<float>(z)
                        )
                    );

                DirectX::XMFLOAT4X4 candidate;

                DirectX::XMStoreFloat4x4(
                    &candidate,
                    candidateMatrix
                );

                bool isSame =
                    IsNear(candidate._11, target._11) &&
                    IsNear(candidate._12, target._12) &&
                    IsNear(candidate._13, target._13) &&

                    IsNear(candidate._21, target._21) &&
                    IsNear(candidate._22, target._22) &&
                    IsNear(candidate._23, target._23) &&

                    IsNear(candidate._31, target._31) &&
                    IsNear(candidate._32, target._32) &&
                    IsNear(candidate._33, target._33);

                if (isSame)
                {
                    m_eulerAngles =
                        DirectX::XMFLOAT3(
                            static_cast<float>(x),
                            static_cast<float>(y),
                            static_cast<float>(z)
                        );

                    return;
                }
            }
        }
    }
}

// 回転角度を取得する
const DirectX::XMFLOAT3& BombRotation::GetEulerAngles() const
{
    return m_eulerAngles;
}

// 回転行列を取得する
const DirectX::XMFLOAT4X4& BombRotation::GetMatrix() const
{
    return m_rotationMatrix;
}