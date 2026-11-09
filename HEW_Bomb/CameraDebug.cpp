#include "CameraDebug.h"

CameraDebug::CameraDebug()
	: m_radXZ(0.0f)
	,m_radY(0.0f)
	,m_radius(5.0f)
{
}

CameraDebug::~CameraDebug()
{
}

void CameraDebug::Update()
{
	// 直接数字で移動速度を設定するより
	// 1秒間の移動距離から１フレームの速度が計算できると良い
	// 
	const float speed = METER(1.5f) / fFPS;
	// キー入力に合わせた注視点の移動処理
	if (IsKeyPress(VK_UP))		{ m_look.z += speed; }
	if (IsKeyPress(VK_DOWN))	{ m_look.z -= speed; }
	if (IsKeyPress(VK_LEFT))	{ m_look.x -= speed; }
	if (IsKeyPress(VK_RIGHT))	{ m_look.x += speed; }
	if (IsKeyPress(VK_SHIFT))	{ m_look.y += speed; }
	if (IsKeyPress(VK_CONTROL)) { m_look.y -= speed; }

	// カメラ位置の移動
	// 回り込みの処理
	if (IsKeyPress('A')) { m_radXZ += speed; }
	if (IsKeyPress('D')) { m_radXZ -= speed; }
	if (IsKeyPress('W')) { m_radY  += speed; }
	if (IsKeyPress('S')) { m_radY  -= speed; }

	// カメラ位置の計算

	m_pos.x = cos(m_radY) * sin(m_radXZ) * m_radius;
		
	m_pos.y = sin(m_radY)* m_radius;
		
	m_pos.z = cos(m_radY) * -cos(m_radXZ) * m_radius;
}
