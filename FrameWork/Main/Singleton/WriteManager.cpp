#include "stdafx.h"
#include "WriteManager.h"


void WriteManager::RenderText(wstring text, RECT rect, float size, wstring font, D3DXCOLOR color, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch)
{
    FontBrushDesc brushDesc;
    brushDesc.Color = color;

    FontBrushDesc* findBrush = NULL;
    //�ݺ���  fontBrush ������ �����ŭ �ݺ�
    //�ݺ��� desc
    for(FontBrushDesc& desc : fontBrush)
    {
        //������ ������ ==  ȣ��
        if(desc == brushDesc)
        {
            findBrush = &desc;
            break;
        }
    }
    //���;ȿ� �ش� �귯�ð� ���ٸ�
    if(findBrush == NULL)
    {
        D2D1::ColorF colorF = D2D1::ColorF(color.r, color.g, color.b);
        deviceContext->CreateSolidColorBrush(colorF, &brushDesc.Brush);

        //�ش� �귯�ø� ���Ϳ� �߰�(������ �������� �ʱ⶧����)
        fontBrush.push_back(brushDesc);
        findBrush = &brushDesc;
    }

    //�Ű����� ��Ʈ�� ����
    FontTextDesc textDesc;
    textDesc.Font = font;
    textDesc.FontSize = size;
    textDesc.Stretch = stretch;
    textDesc.Style = style;
    textDesc.Weight = weight;

    FontTextDesc* findText = NULL;
    //������ �����ŭ �ݺ�

    //for (FontTextDesc& desc : fontText)
    for(UINT i = 0; i < fontText.size(); i++)
    {
        //���� ���� �ؽ�Ʈ�� ���� �Ҷ�
        //if (desc == textDesc)
        if(fontText[i] == textDesc)
        {
            //findText = &desc;
            findText = &fontText[i];
            break;
        }
    }
    //���ٸ� �߰�
    if(findText == NULL)
    {
        writeFactory->CreateTextFormat
        (
            textDesc.Font.c_str(), NULL, textDesc.Weight, textDesc.Style, textDesc.Stretch, textDesc.FontSize,
            L"ko", &textDesc.Format
        );

        fontText.push_back(textDesc);
        findText = &textDesc;
    }

    D2D1_RECT_F rectF;
    rectF.left = (float)rect.left;
    rectF.right = (float)rect.right;
    rectF.top = (float)rect.top;
    rectF.bottom = (float)rect.bottom;


#ifdef UNICODE
    deviceContext->DrawTextW
    (
        text.c_str(), text.size(), findText->Format, rectF, findBrush->Brush
    );
#else
    deviceContext->DrawTextA
    (
        text.c_str(), text.size(), findText->Format, rectF, findBrush->Brush
    );
#endif


}

void WriteManager::Render3DText(Vector3 wPos, float Width, wstring text, wstring font, D3DXCOLOR color, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch)
{

    Matrix V, P;
    CAMMANAGER->vec_pCam[0]->GetRefProj()->GetMatrix(&P);
    CAMMANAGER->vec_pCam[0]->GetRefV()->GetMatrix(&V);

    Vector4 ScreenText[2];
    //W��ǥ������ �ʱ�ȭ
    ScreenText[0] = Vector4(wPos.x - Width * 0.5f,
        wPos.y,
        wPos.z, 1.0f);
    ScreenText[1] = Vector4(wPos.x + Width * 0.5f,
        wPos.y,
        wPos.z, 1.0f);

    /*float tempx = CAMMANAGER->vec_pCam[0]->GetRefRot().x;
    Matrix Rx;
    D3DXMatrixRotationX(&Rx, -tempx);*/
    //D3DXMatrixInverse(&Rx, NULL, &Rx);


    Matrix R = V;
    R._41 =0.0f;
    R._42 =0.0f;
    R._43 =0.0f;
    D3DXMatrixInverse(&R,NULL,&R);

    Matrix RT =  R * V ;

    /*ScreenText[0] += Vector4(V._41, V._42, V._43, 0.0f);
    ScreenText[1] += Vector4(V._41, V._42, V._43, 0.0f);*/


    
    //D3DXMatrixRotationX(&R, -CAMMANAGER->vec_pCam[0]->GetRefRot().x);
    //D3DXMatrixInverse(&R,NULL,&R);
    D3DXVec4Transform(&ScreenText[0], &ScreenText[0], &RT);
    D3DXVec4Transform(&ScreenText[1], &ScreenText[1], &RT);

    
    //Screen     =  *P / w * VP
    D3DXVec4Transform(&ScreenText[0], &ScreenText[0], &P);
    D3DXVec4Transform(&ScreenText[1], &ScreenText[1], &P);

    ScreenText[1] /= ScreenText[1].w;
    ScreenText[0] /= ScreenText[0].w;
    // *VP
    ScreenText[1].x =
        (ScreenText[1].x + 1.0f) *
        CAMMANAGER->vec_pCam[0]->GetRefVP()->GetWidth() / 2.0f;
    ScreenText[1].y =
        (ScreenText[1].y - 1.0f) *
        CAMMANAGER->vec_pCam[0]->GetRefVP()->GetHeight() / -2.0f;
    ScreenText[0].x =
        (ScreenText[0].x + 1.0f) *
        CAMMANAGER->vec_pCam[0]->GetRefVP()->GetWidth() / 2.0f;
    ScreenText[0].y =
        (ScreenText[0].y - 1.0f) *
        CAMMANAGER->vec_pCam[0]->GetRefVP()->GetHeight() / -2.0f;

    float Size = (ScreenText[1].x - ScreenText[0].x) / (text.size()+ 1);
    //           l  t  r    b ��ũ����ǥ
    RECT rect = { ScreenText[0].x,ScreenText[0].y- Size,
       ScreenText[1].x,ScreenText[0].y+ Size };

    
    WRITEMANAGER->RenderText(text, rect, Size);

}

WriteManager::WriteManager()
{
    HRESULT hr;

    hr = DWriteCreateFactory
    (
        DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown **)&writeFactory
    );
    assert(SUCCEEDED(hr));


    D2D1_FACTORY_OPTIONS option;
    option.debugLevel = D2D1_DEBUG_LEVEL_NONE;
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory);
    assert(SUCCEEDED(hr));

    IDXGIDevice* dxgiDevice;
    hr = g_Device->QueryInterface(&dxgiDevice);
    assert(SUCCEEDED(hr));

    hr = factory->CreateDevice(dxgiDevice, &device);
    assert(SUCCEEDED(hr));

    hr = device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &deviceContext);
    assert(SUCCEEDED(hr));


    IDXGISurface* dxgiSurface;
    hr = g_SwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void **)&dxgiSurface);
    assert(SUCCEEDED(hr));


    D2D1_BITMAP_PROPERTIES1 bp;
    bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
    bp.dpiX = 96;
    bp.dpiY = 96;
    bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
    bp.colorContext = NULL;

    hr = deviceContext->CreateBitmapFromDxgiSurface(dxgiSurface, &bp, &targetBitmap);
    assert(SUCCEEDED(hr));
    SAFE_RELEASE(dxgiDevice);
    SAFE_RELEASE(dxgiSurface);
    deviceContext->SetTarget(targetBitmap);
}


WriteManager::~WriteManager()
{
    //���� ������ ��ŭ ����
    for(FontBrushDesc desc : fontBrush)
        SAFE_RELEASE(desc.Brush);

    for(FontTextDesc desc : fontText)
        SAFE_RELEASE(desc.Format);

    //�� ��ü ����
    SAFE_RELEASE(targetBitmap);

    SAFE_RELEASE(factory);
    SAFE_RELEASE(writeFactory);

    SAFE_RELEASE(deviceContext);
    SAFE_RELEASE(device);
}

void WriteManager::ResizeScreen(float width, float height)
{
    if(width < 1 || height < 1)
        return;

    DeleteBackBuffer();

    CreateBackBuffer(width, height);
}

void WriteManager::CreateBackBuffer(float width, float height)
{
    HRESULT hr;

    hr = device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &deviceContext);
    assert(SUCCEEDED(hr));

    IDXGISurface* dxgiSurface;
    hr = g_SwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void **)&dxgiSurface);
    assert(SUCCEEDED(hr));


    D2D1_BITMAP_PROPERTIES1 bp;
    bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
    bp.dpiX = 96;
    bp.dpiY = 96;
    bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
    bp.colorContext = NULL;

    hr = deviceContext->CreateBitmapFromDxgiSurface(dxgiSurface, &bp, &targetBitmap);
    assert(SUCCEEDED(hr));
    SAFE_RELEASE(dxgiSurface);

    deviceContext->SetTarget(targetBitmap);
}

void WriteManager::DeleteBackBuffer()
{
    SAFE_RELEASE(targetBitmap);

    SAFE_RELEASE(deviceContext);
}
