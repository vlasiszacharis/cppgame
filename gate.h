#pragma once
#include "gameobject.h"
#include "box.h"
#include <sgg/graphics.h>

class Gate : public GameObject, public Box
{
public:
	void draw() override;
	Gate(float x, float y, float w, float h) : Box(x, y, w, h) {};
};