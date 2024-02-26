#include "level.h"
#include <sgg/graphics.h>
#include "player.h"
#include "util.h"
#include <iostream>


void Level::drawBlock(int i)
{
	Box& box = m_blocks[i];
	
	float x = box.m_pos_x + m_state->m_global_offset_x; //papa
	float y = box.m_pos_y + m_state->m_global_offset_y;  //papa

	m_block_brush.texture = m_state->getFullAssetPath("block.png"); //idio se ola

	graphics::drawRect(x, y, 1.3f * m_block_size, 1.3f * m_block_size, m_block_brush); //sxediasmos blocks kai dokimi block size

	if (m_state->m_debugging) //debug prasuno
		graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);

}

//an o Meteor yparxei kai den einai energos, diegrapse ton 
void Level::checkMeteorite(Meteor*& meteorite)
{
	if (meteorite && !meteorite->isActive())
	{
		delete meteorite;
		meteorite = nullptr;
	}
}

// Dhmiourgei Explosion kai diagrafei ton Meteor
void Level::kaboom(Meteor*& meteorite)
{
	exp = new Explosion(meteorite->getPosX(), meteorite->getPosY(), meteorite->getSize());
	exp->setActive(true);
	delete meteorite;
	meteorite = nullptr;
}


void Level::checkMeteorCollisions(float dt)
{
	//Dhmiourgw Meteor an xreiazetai kai elegxw tous Meteor gia collision me Player
	for (int i = 0; i < METEOR_NUMBER; i++)
	{
		checkMeteorite(m_meteors[i]);

		//gia na mh kanoun spawn oi Meteor tautoxrona
		if (fmod(graphics::getGlobalTime(), 1000.0f) > 100.0f + (i % 4) * 100.0f && fmod(graphics::getGlobalTime(), 1000.0f) < 200.0f + (i % 4) * 200.0f)
		{
			if (!m_meteors[i])
				m_meteors[i] = new Meteor;
		}

		if (m_meteors[i])
			m_meteors[i]->update(dt);

		if (m_state->getPlayer() && m_meteors[i])
		{
			if (m_state->getPlayer()->intersect(*m_meteors[i]))
			{
				if (m_state->getPlayer()->intersectDown(*m_meteors[i]) < 0.0f)
				{
					
					m_state->getPlayer()->setScore(m_state->getPlayer()->getScore() + m_meteors[i]->getDamageFactor() * m_meteors[i]->getSize());
				}
				else
				{
					//xanei zwh
					m_state->getPlayer()->drainLife(0.1f * m_meteors[i]->getDamageFactor() * m_meteors[i]->getSize());

					//Xanei monades an Sygkroystei. to score to theloyme >=0
					m_state->getPlayer()->setScore(((m_state->getPlayer()->getScore() - m_meteors[i]->getDamageFactor() * m_meteors[i]->getSize()) >= 0) ? (m_state->getPlayer()->getScore() - m_meteors[i]->getDamageFactor() * m_meteors[i]->getSize()) : 0.0f);
				}
					

				//new explosion , delete meteor
				kaboom(m_meteors[i]);

				//checkaroume otan teleiwsei h zvh meta apo kapoio collision gia na kanoume restart
				if (m_state->getPlayer()->getRemainingLife() == 0.0f)
				{
					status = STATUS_OVER;
				}


			}

		}
		checkHitTarget(m_meteors[i]);
	}
}

//An kapoia sfaira xtyphsei Meteor, diegrapse thn, aukshse to Score kai kalese thn Kaboom gia ton Meteor
void Level::checkHitTarget(Meteor*& meteorite)
{
	if (!m_state->getPlayer() || !meteorite)
	{
		return;
	}
	if (meteorite && meteorite->isActive())
	{
		for (int i = 0; i < FLAME_NUMBER; i++)
		{
			if (m_state->getPlayer()->getFlame(i))
			{

				if (m_state->getPlayer()->getFlame(i)->intersect(*meteorite))
				{
					delete m_state->getPlayer()->getFlame(i);
					m_state->getPlayer()->setFlame(i, nullptr);
					m_state->getPlayer()->setScore(m_state->getPlayer()->getScore() + meteorite->getDamageFactor() * meteorite->getSize());
					kaboom(meteorite);
					return;
				}
			}
		}
	}
}

void Level::checkBlockCollisions()
{
	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDown(block))
		{
			m_state->getPlayer()->m_pos_y += offset;

			// add sound event
			if (m_state->getPlayer()->m_vy > 1.0f)
				graphics::playSound(m_state->getFullAssetPath("bump.mp3"), 6.0f, false);

			m_state->getPlayer()->m_vy = 0.0f;

			break;
		}
	}

	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectSideways(block))
		{
			m_state->getPlayer()->m_pos_x += offset;

			m_state->getPlayer()->m_vx = 0.0f;
			break;
		}
	}
} //OK OK

void Level::checkCollisions(float dt) //ok ok
{
	checkBlockCollisions();
	checkMeteorCollisions(dt);
}

void Level::update(float dt)
{
	if (status == STATUS_START)
	{
		if (graphics::getKeyState(graphics::SCANCODE_RETURN))
			status = STATUS_LEVEL1;
		return;
	}

	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);

	checkCollisions(dt);

	checkExplosions();

	checkFinish();


	GameObject::update(dt);
}

void Level::checkExplosions()
{
	//Explosion Handling
	if (exp)
	{
		exp->update();
		if (!exp->isActive())
		{
			delete exp;
			exp = nullptr;
		}
	}
}

void Level::checkFinish() /// an to score einai megalutero apo 5 energopoiise tin puli , an gate  = true , orise status complete
{
	if (m_state->getPlayer()->getScore() > 5.0f && !activateGate)
	{
		gate = new Gate(7, 12.5f, 1, 1);
		activateGate = true;
	}

	if (gate)
	{
		if (m_state->getPlayer()->intersect(*gate))
		{
			status = STATUS_COMPLETED;
		}
	}
}
//OK
void Level::draw()
{
	if (status == STATUS_START)
	{
		graphics::Brush br;
		char info[40];
		sprintf_s(info, "Press ENTER to start");
		graphics::drawText(m_state->getCanvasWidth() / 4.5f, m_state->getCanvasHeight() / 2, 0.3f, info, br);
		return;
	}
	else if (status == STATUS_OVER)
	{
		graphics::Brush br;
		char info[40];
		sprintf_s(info, "GAME OVER");
		graphics::drawText(m_state->getCanvasWidth() / 4.0f, m_state->getCanvasHeight() / 2, 0.5f, info, br);
		return;
	}
	else if (status == STATUS_COMPLETED)
	{
		graphics::Brush br;
		char info[40];
		sprintf_s(info, "GAME COMPLETED");
		graphics::drawText(m_state->getCanvasWidth() / 7.5f, m_state->getCanvasHeight() / 2, 0.5f, info, br);
		return;
	}
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();
	
	float offset_x = m_state->m_global_offset_x / 2.0f + w/2.0f; // 
	float offset_y = m_state->m_global_offset_y / 2.0f + h/2.0f;

	//draw background ,  ta vazo me tin seira sxediasis,prota to background
	graphics::drawRect(offset_x, offset_y, 2.0f*w, 4.0f*w, m_brush_background);

	// draw player
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();

	// draw blocks
	for (int i = 0; i < m_blocks.size(); i++)
	{
		drawBlock(i);
	}

	if (gate)
		gate->draw();

	//draw Meteors
	for (int i = 0; i < METEOR_NUMBER; i++)
	{
		if (m_meteors[i])
		{
			//std::cout << "meteor" << i << "exists" << std::endl;
			m_meteors[i]->draw();
		}
	}

	//draw Explosions
	if (exp)
	{
		if (exp->isActive())
			exp->draw();
	}

	if (gate) gate->draw();

	graphics::Brush br;
	//draw player score
	if (m_state->getPlayer())
	{
		br.outline_opacity = 0.0f;
		char infoo[40];
		sprintf_s(infoo, "[ Score = %5.2f ]", m_state->getPlayer()->getScore());
		graphics::drawText(m_state->getCanvasWidth() - 5.5f, m_state->getCanvasHeight() / 10 + 0.1f, 0.2f, infoo, br);
	}

	//draw plaisio zwhs
	float player_life = m_state->getPlayer() ? m_state->getPlayer()->getRemainingLife() : 0.0f;

	//inner rectangle
	br.outline_opacity = 0.0f;
	br.texture = "";
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.0f;
	br.fill_secondary_color[0] = 1.0f * (1.0f - player_life);
	br.fill_secondary_color[1] = 1.0f * player_life;
	br.fill_secondary_color[2] = 0.0f;
	br.gradient = true;
	br.gradient_dir_u = 1.0f;
	br.gradient_dir_v = 0.0f;

	graphics::drawRect(m_state->getCanvasWidth() - 1.0f - ((1.0f - player_life)/ 2), m_state->getCanvasHeight()/10, player_life, 0.2f, br);

	//outer rectangle
	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	graphics::drawRect(m_state->getCanvasWidth() - 1.0f, m_state->getCanvasHeight() / 10, 1.0f, 0.2f, br);
}

void Level::init()
{
	// Stage 1
	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->init();
	
	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->init();

	for (int i = 0; i < METEOR_NUMBER; i++)
	{
		m_meteors[i] = nullptr;
	}

	//blocks initialization
	for (float y = 0; y < 23; y += 1.0f) {
		// aristeros toixos
		m_blocks.push_back(Box(0, y, 1, 1));
		// deksis toixos
		m_blocks.push_back(Box(14, y, 1, 1));
	}

	for (float x = 1; x < 14; x += 1.0f) {
		// panw toixos
		m_blocks.push_back(Box(x, 0, 1, 1));
		// katw toixos
		m_blocks.push_back(Box(x, 22, 1, 1));
	}

	for (int i = 7; i < 11; i++) 
	{
		m_blocks.push_back(Box(i, 20, 1, 1));
	}

	m_blocks.push_back(Box(11, 19, 1, 1));
	m_blocks.push_back(Box(13, 18, 1, 1));

	for (int i = 0; i < 3; i++) 
	{
		m_blocks.push_back(Box(3 + i, 18, 1, 1));
	}

	m_blocks.push_back(Box(3, 16, 1, 1));
	m_blocks.push_back(Box(5, 15, 1, 1));
	m_blocks.push_back(Box(7, 14, 1, 1)); //TOIXOS  


	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.1f, 1.0f, 0.1f); //prasino kouti debug
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);
} ///OK  // OK
//OK
Level::Level(const std::string & name)
	: GameObject(name)
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("background.png");
}

Level::~Level()
{
	for (auto p_go : m_static_objects)
		delete p_go;
	for (auto p_go : m_dynamic_objects)
		delete p_go;
	for (int i = 0; i < METEOR_NUMBER; i++) {
		if (m_meteors[i]) {
			delete m_meteors[i];
			m_meteors[i] = nullptr;
		}
	}
	if (gate) delete gate;
}
