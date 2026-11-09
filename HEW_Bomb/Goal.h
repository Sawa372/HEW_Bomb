#pragma once

class Goal 
{
private:
	float m_pos[3]; // 座標
	float m_scale[3];	// 拡大率

	float m_time;	// アニメーション経過時間

public:
	Goal();
	~Goal();

	void Update();
	void Draw();

	void SetPos(float x, float y, float z);
	void SetScale(float x, float y, float z);
};