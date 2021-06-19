#pragma once

class ShaderManager : public SingletonBase<ShaderManager>
{
private:
	map<_tstring, Shader*> ShaderMap;
	typedef map<_tstring, Shader*>::iterator Iter;
public:
	ShaderManager();
	~ShaderManager();
	Shader* AddShader(_tstring key, string input, string vs = "VS", string ps = "PS");
	void DeleteAll();
};