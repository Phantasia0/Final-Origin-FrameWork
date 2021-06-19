#pragma once
class Particle : public Object
{
protected:
	UINT particleCount;
	bool isPlay;
	Texture* texture;
	Shader* shader;
public:
	Particle(UINT particleCount =1);
	virtual ~Particle() override;
	virtual HRESULT create() override;
	virtual void destroy() override;
	virtual void update() override;
	virtual void render() override;
	virtual void reset();
	virtual void play(Vector3 position, Vector3 rot, Vector3 scale);
	virtual void stop();

	MAKEGET(bool, isPlay)
	MAKEREFGET(Texture*, texture)
};