#pragma once
#include "stdafx.h"

class Scene
{
public:
	virtual HRESULT init() = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void PreRender() = 0;
	virtual void render() = 0;
	Scene() {};
	virtual ~Scene() {};
};