#pragma once
#include "DirectX.h"

//enum class BombType
//{
//	TypeA = 0,
//	TypeB,
//	TypeC,
//	Max
//};

class HandheldBombUI
{
public:
	HandheldBombUI();
	~HandheldBombUI();

	void Update();
	void Draw();

private:
	ID3D11Buffer* m_pSlotBuffer;
	ID3D11Buffer* m_pLargeBuffer;
	ID3D11ShaderResourceView* m_pTexture;
};

