#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "box.h"
#include "util.h"
#include "meteor.h"
#include "flame.h"

class Player : public Box, public GameObject
{
	// animated player
	std::vector<std::string> m_sprites;

	graphics::Brush m_brush_player;

	const float m_accel_horizontal = 20.0f; //rythmos oy epitaxynei orizontia
	const float m_accel_vertical = 300.1f;//katheta
	const float m_max_velocity = 5.0f;//max taxitita
	const float m_gravity = 10.0f;
	float life = PLAYER_LIFE; //einai sto util.h
	float score = 0.0f;
	Flame* flames[FLAME_NUMBER]; //pinakas apo floges pou ftinei
	int currentFlames = 0; //arithmos energwn flogwn
	bool emptyOfFlames = false; //otan teleiwnoun oi sfaires


public:
	float m_vx = 0.0f; // orizontia sxetikh thesi
	float m_vy = 0.0f; // katheti sxetikh thesi


public:
	void update(float dt) override;
	void draw() override;
	void init() override;
	float getPositionX() const;
	float getPositionY() const;
	Player(std::string name) : GameObject(name) {}
	~Player();
	float getScore() { return score; }
	void setScore(float s) { score = s; }
	float getRemainingLife() const { return life; }
	void drainLife(float amount) { life = std::max<float>(0.0f, life - amount); }
	void addExtraLife(float amount) { life = std::max<float>(0.0f, life + amount); }
	Flame* getFlame(int i) { return flames[i]; }
	void setFlame(int i, Flame* f) { flames[i] = f; }
	void checkFlames();
	void shoot(float dt);
	
protected:
	void debugDraw();
	
	// dynamic motion control
	void movePlayer(float dt);
};


