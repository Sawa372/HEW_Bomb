#pragma once

#include <DirectXMath.h>

class BombRotation
{
public:
    BombRotation();
    ~BombRotation();

    // 回転を初期状態に戻す
    void Reset();

    // ワールド座標基準で90度回転
    void RotateWorldX90();
    void RotateWorldY90();

    // 回転角度を取得する（度数法）
    const DirectX::XMFLOAT3& GetEulerAngles() const;

    // 回転行列を取得する
    const DirectX::XMFLOAT4X4& GetMatrix() const;

private:
    // 回転行列
    DirectX::XMFLOAT4X4 m_rotationMatrix;

    // 既存のBombやExplosionで使用する回転角度
    DirectX::XMFLOAT3 m_eulerAngles;

    // 回転行列から角度を更新する
    void UpdateEulerAngles();
};