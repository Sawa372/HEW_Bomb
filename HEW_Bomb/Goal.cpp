#include "Goal.h"
#include "Defines.h"
#include "DirectXMath.h"
#include "Geometory.h"
Goal::Goal()
	:m_time(0)
{
}

Goal::~Goal()
{
}

void Goal::Update()
{
	m_time += 1.0f/fFPS;
}

void Goal::Draw()
{
	// 変換行列の計算
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos[0], m_pos[1], m_pos[2]);
	DirectX::XMMATRIX Rx =DirectX::XMMatrixRotationX(m_time);
	DirectX::XMMATRIX Ry = DirectX::XMMatrixRotationY(m_time);
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_time);
	DirectX::XMMATRIX R = Rx * Ry * Rz;
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_scale[0], m_scale[1], m_scale[2]);

	DirectX::XMMATRIX mat = S * R * T;

	// 変換行列をGPUに渡すための前処理
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);
	

	// 描画処理
	Geometory::SetWorld(fMat);
	Geometory::DrawBox();
}

// 座標設定
void Goal::SetPos(float x, float y, float z)
{
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}

// 拡大率の設定
void Goal::SetScale(float x, float y, float z)
{
	m_scale[0] = x;
	m_scale[1] = y;
	m_scale[2] = z;
}
