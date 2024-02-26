#pragma once
#include "gameobject.h"
#include <sgg/graphics.h>

class Explosion : public GameObject
{
	float pos_x;
	float pos_y;
	float size;
	bool active = false;

public:
	Explosion(float p_x, float p_y, float size);
	void update();
	void draw() override;
	void init() override;
	bool isActive() { return active; }
	void setActive(bool a) { active = a; }
};