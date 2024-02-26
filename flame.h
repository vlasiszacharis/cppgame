#pragma once
#include "gameobject.h"
#include "box.h"
#include "sgg/graphics.h"
#include "util.h"

class Flame : public GameObject, public Box
{
	float speed = 0.003f;
	float size = 0.2f;
	bool active = true;//
	graphics::Brush debugBrush;

public:
	Flame(float p_x, float p_y);
	void update(float dt) override;
	void draw() override;
	void init() override;
	bool isActive() { return active; }
	void setActive(bool a) { active = a; }
	void debugDraw();
};