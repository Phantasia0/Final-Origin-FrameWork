#include "stdafx.h"
#include "Shader.h"

Shader::Shader(_tstring file, string vs, string ps)
{
	{
		UINT flag = 
			D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_ENABLE_STRICTNESS
			| D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_DEBUG;

		HRESULT hr;

		hr = D3DX11CompileFromFile(file.c_str(), 0, 0, vs.c_str(), "vs_5_0", flag, 0, 0, &VsBlob, 0, 0);
		assert(SUCCEEDED(hr));

		hr = D3DX11CompileFromFile(file.c_str(), 0, 0, ps.c_str(), "ps_5_0", 0, 0, 0, &PsBlob, 0, 0);
		assert(SUCCEEDED(hr));

		hr = g_Device->CreateVertexShader(VsBlob->GetBufferPointer(), VsBlob->GetBufferSize(), NULL, &VertexShader);
		assert(SUCCEEDED(hr));

		hr = g_Device->CreatePixelShader(PsBlob->GetBufferPointer(), PsBlob->GetBufferSize(), NULL, &PixelShader);
		assert(SUCCEEDED(hr));
	}
}

Shader::~Shader()
{
	SAFE_RELEASE(VertexShader);
	SAFE_RELEASE(PixelShader);
	SAFE_RELEASE(VertexLayout);
	SAFE_RELEASE(VsBlob);
	SAFE_RELEASE(PsBlob);
	SAFE_RELEASE(CsBlob);
	SAFE_RELEASE(GsBlob);
	SAFE_RELEASE(GeometryShader);
	SAFE_RELEASE(ComputeShader);
}

void Shader::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * desc, UINT count)
{
	HRESULT hr;
	hr = g_Device->CreateInputLayout(desc, count, VsBlob->GetBufferPointer(), VsBlob->GetBufferSize(), &VertexLayout);
	assert(SUCCEEDED(hr));
}

void Shader::CreateGeometryShader(_tstring file, string gsName)
{
	UINT flags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_ENABLE_STRICTNESS
		| D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_DEBUG;

	D3DX11CompileFromFile(file.c_str(), 0, 0, gsName.c_str(), "gs_5_0", 0, 0, 0, &GsBlob, 0, 0);

	HRESULT hr = g_Device->CreateGeometryShader(GsBlob->GetBufferPointer(), GsBlob->GetBufferSize(), nullptr, &GeometryShader);
	CHECK(hr);
}

void Shader::CreateComputeShader(_tstring file, string csName)
{
	UINT flags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_ENABLE_STRICTNESS
		| D3DCOMPILE_PREFER_FLOW_CONTROL | D3DCOMPILE_DEBUG;

	D3DX11CompileFromFile(file.c_str(), 0, 0, csName.c_str(), "cs_5_0", 0, 0, 0, &CsBlob, 0, 0);

	HRESULT hr = g_Device->CreateComputeShader(CsBlob->GetBufferPointer(), CsBlob->GetBufferSize(), nullptr, &ComputeShader);
	CHECK(hr);
}

void Shader::Render()
{
	g_DeviceContext->VSSetShader(VertexShader, 0, 0);
	g_DeviceContext->PSSetShader(PixelShader, 0, 0);
	g_DeviceContext->IASetInputLayout(VertexLayout);
}

void Shader::CSSet()
{
	g_DeviceContext->CSSetShader(ComputeShader, 0, 0);
}