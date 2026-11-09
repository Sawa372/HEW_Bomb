#include "Ground.h"
#include "DirectXMath.h"
#include "Geometory.h"

Ground::Ground()
	:m_pos{0.0f,0.0f,0.0f}
	,m_scale{1.0f,1.0f}
{
}

Ground::~Ground()
{
}

void Ground::Update()
{
}
// •`‰æˆ—
void Ground::Draw()
{
	// •ÏŠ·s—ñ‚ÌŒvZ
	DirectX::XMMATRIX T = 
		DirectX::XMMatrixTranslation(
			m_pos[0], m_pos[1], m_pos[2]);
	DirectX::XMMATRIX S =
		DirectX::XMMatrixScaling(
			m_scale[0], m_scale[1], m_scale[2]);

	DirectX::XMMATRIX mat = S * T;

	// •ÏŠ·s—ñ‚ğGPU‚É“n‚·‚½‚ß‚Ì‘Oˆ—
	mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fMat;
	DirectX::XMStoreFloat4x4(&fMat, mat);


	// •`‰æˆ—
	Geometory::SetWorld(fMat);
	Geometory::DrawBox();
}

// À•W‚Ìİ’è
void Ground::SetPos(float x, float y, float z)
{
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}

// Šg‘å—¦‚Ìİ’è
void Ground::SetScale(float x, float y, float z)
{
	m_scale[0] = x;
	m_scale[1] = y;
	m_scale[2] = z;
}