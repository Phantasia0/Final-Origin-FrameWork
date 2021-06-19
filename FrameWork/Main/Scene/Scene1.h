#pragma once

class Scene1 : public Scene
{
private:
	QuadTree* Quad;
	TerrainEditor* Editor;
public:
	Scene1();
	~Scene1() override;
	HRESULT init() override;
	void release() override;
	void update() override;
	void PreRender() override;
	void render() override;
};