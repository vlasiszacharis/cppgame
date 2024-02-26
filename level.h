#pragma once

#include "gameobject.h"
#include <vector>
#include <list>
#include <string>
#include <sgg/graphics.h>
#include "player.h"
#include "meteor.h"
#include "explosion.h"
#include "util.h"
#include "gate.h"

class Level : public GameObject
{
	graphics::Brush m_brush_background;

	std::vector<GameObject*> m_static_objects; //Antikeimena pou tha exei mesa,pou einai statika,den allazoun,OXI UPDATE
	std::list<GameObject*> m_dynamic_objects; //Lista pou tha apothikeuei ta dunamika ,pou allazoun ,kalei me polumorfiki oti allazei
	
	// add some collidable blocks
	std::vector<Box> m_blocks; //papa , pou einai kai ti diastaseis exoun
	std::vector<std::string> m_block_names;
	const float m_block_size = 1.0f;
	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug; // oria collision

	// add some collidable interactive objects
	Meteor* m_meteors[METEOR_NUMBER]; // define sto util 15 meteorites
	Explosion* exp = nullptr;

	graphics::Brush m_meteor_brush;
	graphics::Brush m_meteor_brush_debug;

	Gate* gate = 0;
	bool activateGate = false;

	float m_center_x = 5.0f; //Define x,y
	float m_center_y = 5.0f;

	typedef enum { STATUS_START, STATUS_LEVEL1, STATUS_OVER, STATUS_COMPLETED } gameStatus;
	gameStatus status = STATUS_START;

	// dedicated method to draw a block
	void drawBlock(int i);

	// detect collisions
	void checkCollisions(float dt); //papap

	// detect collisions with blocks
	void checkBlockCollisions();

	// detect collisions with meteorites
	void checkMeteorCollisions(float dt);

	// check for inactive Meteorites and delete them
	void checkMeteorite(Meteor*& meteorite);

	void kaboom(Meteor*& meteorite);

	void checkHitTarget(Meteor*& meteorite);




public:
	void update(float dt) override; //polumorfismos apo gameobject
	void checkExplosions();
	void checkFinish();
	void draw() override;
	void init() override;

	Level(const std::string & name = "Level0");
	~Level() override; //destructor 
};