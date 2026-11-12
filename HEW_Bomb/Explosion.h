#pragma once
#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

#include <DirectXMath.h>

// 爆発の範囲を管理するクラス
class Explosion
{
public:
    Explosion();
    ~Explosion();

    // 爆発の中心位置を設定
    void SetCenter(const DirectX::XMFLOAT3& center);

    // 爆発の範囲を設定
    void SetRadius(float radius);

    // 指定した座標が爆発範囲内か
    bool IsInside(const DirectX::XMFLOAT3& position) const;

private:
    // 爆発の中心位置
    DirectX::XMFLOAT3 m_center;

    // 爆発の半径
    float m_radius;
};

#endif // __EXPLOSION_H__