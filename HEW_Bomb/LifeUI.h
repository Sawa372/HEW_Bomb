#pragma once

class LifeUI
{
public:
	LifeUI();
	~LifeUI();

	void Update();
	void Draw();
	void DeleteLife();

private:
	int m_life;
};