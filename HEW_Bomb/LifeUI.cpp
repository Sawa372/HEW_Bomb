#include "LifeUI.h"

LifeUI::LifeUI()
	:m_life(3) // 初期スコア3
{
}

LifeUI::~LifeUI()
{
}

void LifeUI::Update()
{
}
void LifeUI::Draw()
{
}


void LifeUI::DeleteLife() // プレイヤーが死んだときに呼ぶ
{
	m_life -= 1;

	if (m_life < 0)
	{
		m_life = 0;
	}
}
