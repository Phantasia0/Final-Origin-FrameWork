#include "stdafx.h"
#include "RenderState.h"

RenderState::RenderState()
    :RasterizerState(nullptr), DepthStencilState(nullptr)
{
    BlendState[0] = nullptr;
    BlendState[1] = nullptr;

    //������ ������ �⺻��
    ZeroMemory(&RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
    RasterizerDesc.FillMode = D3D11_FILL_SOLID; // D3D11_FILL_WIREFRAME�� �׵θ�������
    RasterizerDesc.CullMode = D3D11_CULL_BACK; //�ø��Ҷ� �ո�,�޸� �߷����� �Ұ���
    RasterizerDesc.DepthBias = 0;
    RasterizerDesc.FrontCounterClockwise = false; //cw ccw �� ���ϱ�
    RasterizerDesc.DepthBiasClamp = 0;
    RasterizerDesc.SlopeScaledDepthBias = 0;
    RasterizerDesc.DepthClipEnable = true; //����Ŭ���� 
    RasterizerDesc.ScissorEnable = false; //�����׽�Ʈ ���� ����
    RasterizerDesc.MultisampleEnable = false; //��Ƽ ���ø� ��������
    RasterizerDesc.AntialiasedLineEnable = false; //���ξ�Ƽ�ٸ���� ����


    //���� ���ٽ� �⺻��(������ �ʾ�����)
    ZeroMemory(&DepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    DepthStencilDesc.DepthEnable = true;//���̰��� ����Ѵ�.
    DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    DepthStencilDesc.StencilEnable = false;
    DepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;;
    DepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;;
    DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    DepthStencilDesc.BackFace = DepthStencilDesc.FrontFace;

    ZeroMemory(&BlendDesc[0], sizeof(D3D11_BLEND_DESC));
    ZeroMemory(&BlendDesc[1], sizeof(D3D11_BLEND_DESC));

    //0���� ���İ��� �����ʴ�
    BlendDesc[0].AlphaToCoverageEnable = false;
    BlendDesc[0].RenderTarget[0].BlendEnable = false;
    BlendDesc[0].RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    //1���� ���İ��� ����
    BlendDesc[1].AlphaToCoverageEnable = true;
    BlendDesc[1].RenderTarget[0].BlendEnable = true;
	BlendDesc[1].RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	BlendDesc[1].RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc[1].RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc[1].RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc[1].RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc[1].RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc[1].RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
}

RenderState::~RenderState()
{
    if (RasterizerState)
    {
        SAFE_RELEASE(RasterizerState);
    }
    if (DepthStencilState)
    {
        SAFE_RELEASE(DepthStencilState);
    }
    if (BlendState[0])
    {
        SAFE_RELEASE(BlendState[0]);
    }
    if (BlendState[1])
    {
        SAFE_RELEASE(BlendState[1]);
    }
}

void RenderState::CreateRasterizerState()
{
    if (RasterizerState)
    {
        SAFE_RELEASE(RasterizerState);
    }
    HRESULT hr;
    hr = g_Device->CreateRasterizerState(&RasterizerDesc, &RasterizerState);
    CHECK(hr);
}

void RenderState::CreateBlendState()
{
    if (BlendState[0])
    {
        SAFE_RELEASE(BlendState[0]);
    }
    HRESULT hr;
    hr = g_Device->CreateBlendState(&BlendDesc[0], &BlendState[0]);
    CHECK(hr);
    if (BlendState[1])
    {
        SAFE_RELEASE(BlendState[1]);
    }
    hr = g_Device->CreateBlendState(&BlendDesc[1], &BlendState[1]);
    CHECK(hr);
}

void RenderState::CreateDepthStencil()
{
    if (DepthStencilState)
    {
        SAFE_RELEASE(DepthStencilState);
    }
    HRESULT hr;
    hr = g_Device->CreateDepthStencilState(&DepthStencilDesc, &DepthStencilState);
    CHECK(hr);
}

void RenderState::BeforeRender()
{
    if (DepthStencilState)
    {
        g_DeviceContext->OMSetDepthStencilState(DepthStencilState, 0xFF);
    }
    if (RasterizerState)
    {
        g_DeviceContext->RSSetState(RasterizerState);
    }
    if (BlendState[1])
    {
        g_DeviceContext->OMSetBlendState(BlendState[1], NULL, 0xFF);
    }
}

void RenderState::AfterRender()
{
    if (BlendState[0])
    {
        g_DeviceContext->OMSetBlendState(BlendState[0], NULL, 0xFF);
    }
}