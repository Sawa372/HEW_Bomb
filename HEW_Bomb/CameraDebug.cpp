#include "CameraDebug.h"

CameraDebug::CameraDebug()
	:m_radXZ(DirectX::XMConvertToRadians(45.0f))
	,m_radY(DirectX::XMConvertToRadians(25.0f))
	,m_radius(METER(10.0f))
	,m_minRadY(DirectX::XMConvertToRadians(10.0f))
	,m_maxRadY(DirectX::XMConvertToRadians(50.0f))
	,m_minRadius(METER(5.0f))
	,m_maxRadius(METER(10.0f))
{
	// 注視点：マップ中央
	m_look = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	// カメラ位置：マップ中央から斜め上
	m_pos = DirectX::XMFLOAT3(0.0f, 28.0f, -28.0f);
	// カメラの上方向
	m_up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	// 初期画角
	m_fovy = DirectX::XMConvertToRadians(60.0f);
}

CameraDebug::~CameraDebug()
{
}

// 注視点のY座標設定

void CameraDebug::SetTargetY(float y)
{
	m_look.y = y;
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

	// ズームイン
	if (IsKeyPress('N'))
	{
		m_radius -= 0.1;
	}

	// ズームアウト
	if (IsKeyPress('M'))
	{
		m_radius += 0.1;
	}

	// カメラ位置の計算

	m_pos.x = m_look.x + cos(m_radY) * sin(m_radXZ) * m_radius;
		
	m_pos.y = m_look.y + sin(m_radY)* m_radius;
		
	m_pos.z = m_look.z + cos(m_radY) * -cos(m_radXZ) * m_radius;

	//垂直方向の回転角度を制限
	//下限を制限
	if (m_radY < m_minRadY)
	{
		m_radY = m_minRadY;
	}

	//上限を制限
	else if (m_radY > m_maxRadY)
	{
		m_radY = m_maxRadY;
	}

	// カメラ距離を制限
	// 最短距離を制限
	if (m_radius < m_minRadius)
	{
		m_radius = m_minRadius;
	}

	// 最長距離を制限
	else if (m_radius > m_maxRadius)
	{
		m_radius = m_maxRadius;
	}
}
