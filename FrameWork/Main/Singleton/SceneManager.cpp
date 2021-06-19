#include "stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	for (map<string, Scene*>::iterator i = mapScene.begin(); i != mapScene.end(); i++)
	{
		if (i->second)
		{
			i->second->release();
			SAFE_DELETE(i->second);
		}
	}
	mapScene.clear();
}

void SceneManager::AddScene(string key, Scene * p)
{
	if (mapScene.find(key) == mapScene.end())
	{
		mapScene[key] = p;
	}
	else
	{
		assert(0 && "Scene Name Overlap");
	}
}

Scene * SceneManager::ChangeScene(string key)
{
	map<string, Scene*>::iterator it;
	it = mapScene.find(key);
	if (it == mapScene.end())
	{
		assert(0 && "none Scene");
	}
	else
	{
		CurrentScene = *it;
	}

	return CurrentScene.second;
}

Scene * SceneManager::GetScene(string key)
{
	if (mapScene.find(key) == mapScene.end())
	{
		assert(0 && "none Scene");
	}
	Scene* temp = mapScene[key];

	return temp;
}

void SceneManager::DeleteScene(string key)
{
	map<string, Scene*>::iterator it;
	it = mapScene.find(key);
	if (it == mapScene.end())
	{
		assert(0 && "none Scene");
	}
	else
	{
		if (it->first != CurrentScene.first)
		{
			mapScene.erase(it);
		}
	}
}