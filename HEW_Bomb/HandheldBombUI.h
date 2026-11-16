#pragma once
#include "DirectX.h"


class HandheldBombUI
{
public:
	HandheldBombUI();
	~HandheldBombUI();

	void Update();
	void Draw();

private:
	ID3D11Buffer* m_pBuffer;
	ID3D11ShaderResourceView* m_pTexture;
};

