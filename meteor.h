#pragma once
#include "gameobject.h"
#include "box.h"
#include <sgg/graphics.h>
#include "util.h"


class Meteor : public Box, public GameObject
{
protected:
	float previousGlobalOffsetX, previousGlobalOffsetY;
	float speed;
	float size;
	float rotation;
	graphics::Brush brush;
	graphics::Brush m_meteor_brush_debug;
	bool active = true;
	float damage_factor;
	void debugDraw();
public:
	void update(float dt) override;
	void draw() override;
	void init() override;
	virtual bool isActive() { return active; }
	Meteor();
	~Meteor();
	virtual float getSize() const { return size; } // πολυμορφισμος , χρησιμοποιουνται στο λεβελ
	virtual float getPosX() {return m_pos_x;}
	virtual float getPosY() {return m_pos_y;}
	virtual float getDamageFactor() { return damage_factor; } //Παραδείγματα:

	//Διαφορετικοί τύποι εχθρών μπορεί να έχουν διαφορετικούς παράγοντες ζημιάς.
	//	Ο παράγοντας ζημιάς μπορεί να εξαρτάται από το επίπεδο δυσκολίας.
};