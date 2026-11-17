#include "HandheldBombUI.h"
#include "VertexBuffer.h"
#include "DirectXTex/TextureLoad.h"
#include "SpriteDrawer.h"

HandheldBombUI::HandheldBombUI()
	:m_pSlotBuffer(nullptr)
	,m_pLargeBuffer(nullptr)
	,m_pTexture(nullptr)
{
	// 頂点バッファの作成
	float w = 216 * 0.5f;
	float h = 36 * 0.5f;
	Vertex vtx[] = {
		{{-w, -h, 0.0f}, {0.0f, 0.0f}},
		{{-w,  h, 0.0f}, {0.0f, 1.0f}},
		{{ w, -h, 0.0f}, {1.0f, 0.0f}},
		{{ w,  h, 0.0f}, {1.0f, 1.0f}},
	};

	m_pSlotBuffer = CreateVertexBuffer(GetDevice(), vtx, _countof(vtx));
	if (m_pSlotBuffer == nullptr)
	{
		MessageBox(NULL, "HandheldBombUI", "Error", MB_OK);
	}

	HRESULT hr = LoadTextureFromFile(GetDevice(), "HandheldBombUI.png", &m_pTexture);
	if (FAILED(hr)) { MessageBox(NULL, "HandheldBombUI", "Load Error", MB_OK); }
}

HandheldBombUI::~HandheldBombUI()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
	if (m_pSlotBuffer != nullptr)
	{
		m_pSlotBuffer->Release();
		m_pSlotBuffer = nullptr;
	}
}

void HandheldBombUI::Update()
{

}

void HandheldBombUI::Draw()
{
	// 画像表示処理
	SetSpritePos(-500, 250);
	SetSpriteTexture(m_pTexture);
	DrawSprite(m_pSlotBuffer);
}