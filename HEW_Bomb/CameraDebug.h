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

public:
	// コンストラクタ
	CameraDebug();

	// デストラクタ
	~CameraDebug();

	// 更新処理
	void Update() final;
};