#pragma once

struct FontBrushDesc;
struct FontTextDesc;

class WriteManager : public SingletonBase<WriteManager>
{
private:
	ID2D1Device* device;
	ID2D1DeviceContext* deviceContext = NULL;
	ID2D1Factory1* factory;
	IDWriteFactory* writeFactory = NULL;
	ID2D1Bitmap1* targetBitmap;
	vector<FontBrushDesc> fontBrush;
	vector<FontTextDesc>  fontText;
public:
	void RenderText
	(
		wstring text
		, RECT rect
		, float size = 20.0f
		, wstring font = L"µ¸¿òÃ¼"
		, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1)
		, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL
		, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL
	);
	void Render3DText
	(
		Vector3 wPos,
		float Width,
		wstring text
		, wstring font = L"µ¸¿òÃ¼"
		, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1)
		, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL
		, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL
		, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL
	);
public:
	WriteManager();
	~WriteManager();
	ID2D1DeviceContext* GetDc() { return deviceContext; }
	void ResizeScreen(float width, float height);
	void CreateBackBuffer(float width, float height);
	void DeleteBackBuffer();
};

struct FontBrushDesc
{
	D3DXCOLOR Color;
	ID2D1SolidColorBrush* Brush = NULL;
	bool operator==(const FontBrushDesc& val)
	{
		return Color.r == val.Color.r &&
			Color.g == val.Color.g &&
			Color.b == val.Color.b &&
			Color.a == val.Color.a;
	}
};

struct FontTextDesc
{
	wstring Font;
	float FontSize;
	DWRITE_FONT_WEIGHT Weight;
	DWRITE_FONT_STYLE Style;
	DWRITE_FONT_STRETCH Stretch;
	IDWriteTextFormat* Format = NULL;
	bool operator==(const FontTextDesc& val)
	{
		bool b = true;
		b &= Font == val.Font;
		b &= FontSize == val.FontSize;
		b &= Weight == val.Weight;
		b &= Style == val.Style;
		b &= Stretch == val.Stretch;

		return b;
	}
};