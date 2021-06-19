#pragma once

class Scene;
class SceneManager : public SingletonBase<SceneManager>
{
private:
	map<string, Scene*>mapScene;
public:
	pair<string, Scene*>CurrentScene;
	SceneManager();
	~SceneManager();
	void AddScene(string key, Scene* p);
	Scene* ChangeScene(string key);
	Scene* GetScene(string key);
	void DeleteScene(string key);
};