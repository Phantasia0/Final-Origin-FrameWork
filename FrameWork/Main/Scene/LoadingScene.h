#pragma once
class LoadingScene : public Scene
{
private:
	Ui* Loading;
	thread* t1;
public:
	LoadingScene();
	~LoadingScene() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void PreRender() override;
	void render() override;

	static void LoadScene();
};