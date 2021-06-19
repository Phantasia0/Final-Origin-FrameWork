#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

Shader * ShaderManager::AddShader(_tstring key, string input, string vs, string ps)
{
	Iter iter = ShaderMap.find(key);

	if (iter == ShaderMap.end())
	{
		Shader* re = new Shader(key, vs,ps);
		ShaderMap.insert(make_pair(key, re));
		if (input == "PC")
		{
			re->CreateInputLayout(VERTEX_PC::LayoutDesc, 2);
		}
		else if (input == "PT")
		{
			re->CreateInputLayout(VERTEX_PT::LayoutDesc, 2);
		}
		else if (input == "PS")
		{
			re->CreateInputLayout(VERTEX_PS::LayoutDesc, 2);
		}
		else if (input == "PSV")
		{
			re->CreateInputLayout(VERTEX_PSV::LayoutDesc, 3);
		}
		else if (input == "PCN")
		{
			re->CreateInputLayout(VERTEX_PCN::LayoutDesc, 3);
		}
		else if (input == "PTN")
		{
			re->CreateInputLayout(VERTEX_PTN::LayoutDesc, 3);
		}
		else if (input == "MODEL")
		{
			re->CreateInputLayout(VERTEX_MODEL::LayoutDesc, 6);
		}
		else
		{
			assert(!"inputError");
		}

		return re;
	}
	else
	{
		return iter->second;
	}

	return nullptr;
}

void ShaderManager::DeleteAll()
{
	Iter iter = ShaderMap.begin();
	for (iter; iter != ShaderMap.end(); iter++)
	{
		if (iter->second != nullptr)
		{
			SAFE_DELETE(iter->second);
		}
	}
	ShaderMap.clear();
}