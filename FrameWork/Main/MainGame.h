#pragma once

class MainGame : public Scene
{
public:
	MainGame();
	~MainGame() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void PreRender()override;
	void render() override;
};