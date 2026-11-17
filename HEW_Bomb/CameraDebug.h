#pragma once
#include "Camera.h"
#include "Input.h"
#include "Defines.h"


// デバッグ用のカメラ
class CameraDebug : public Camera
{
private:
	float m_radXZ; // カメラの横移動
	float m_radY; // カメラの縦移動
	float m_radius; // カメラと注視点の距離
	float m_zoomSpeed;  // ズーム速度
	float m_minFovy;    // 最小画角
	float m_maxFovy;    // 最大画角
	float m_minRadY; // 縦回転の下限
	float m_maxRadY; // 縦回転の上限

public:
	// コンストラクタ
	CameraDebug();

	// デストラクタ
	~CameraDebug();

	// 更新処理
	void Update() final;
};