#include "stdafx.h"
#include "Particle.h"

Particle::Particle(UINT particleCount)
{
	texture = new Texture();
	this->particleCount = particleCount;
}

Particle::~Particle()
{
	SAFE_DELETE(texture);
}

HRESULT Particle::create()
{
	Object::create();
	return S_OK;
}

void Particle::destroy()
{
	Object::destroy();
}

void Particle::update()
{
	Object::update();
}

void Particle::render()
{
	Object::render();
}

void Particle::reset()
{
}

void Particle::play(Vector3 position, Vector3 rot, Vector3 scale)
{
	isPlay = true;
	Pos = position;
	Rot = rot;
	Scale = scale;
}

void Particle::stop()
{
	isPlay = false;
}