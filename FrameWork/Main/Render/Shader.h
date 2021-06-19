#pragma once

class Shader
{
private:
	ID3D11InputLayout* VertexLayout;

	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11GeometryShader* GeometryShader = nullptr;
	ID3D11ComputeShader* ComputeShader = nullptr;

	ID3D10Blob* VsBlob;
	ID3D10Blob* PsBlob;
	ID3D10Blob* GsBlob;
	ID3D10Blob* CsBlob;
public:
	Shader() {};
	Shader(_tstring file, string vs = "VS", string ps = "PS");
	~Shader();
	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT count);
	void CreateGeometryShader(_tstring file, string gsName = "GS");
	void CreateComputeShader(_tstring file, string csName = "CS");
	void Render();
	void CSSet();
	MAKEREFGET(ID3D11GeometryShader*, GeometryShader)
	MAKEREFGET(ID3D11ComputeShader*, ComputeShader)
};