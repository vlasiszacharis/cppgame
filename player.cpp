#include "player.h"
#include <cmath>

void Player::update(float dt)
{
	float delta_time = dt / 1000.0f;

	movePlayer(dt);
	shoot(dt);

	// update offset for other game objects
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;
	
	GameObject::update(dt);

}

void Player::draw()
{	
	int sprite;

	if (m_vx >= 0)
	{
		sprite = (int)fmod(100.0f + m_pos_x * 9.0f, m_sprites.size()/2);
	}
	else {
		sprite = (int)fmod(100.0f - m_pos_x * 9.0f, m_sprites.size() / 2) + (m_sprites.size() / 2);
	}

	m_brush_player.texture = m_sprites[sprite];
	graphics::drawRect(m_state->getCanvasWidth()*0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
	
	//draw flames
	for (int i = 0; i < FLAME_NUMBER; i++)
	{
		if (flames[i])
		{
			flames[i]->draw();
		}
	}


	if (m_state->m_debugging)
		debugDraw();
}

void Player::init()
{
	// stage 1
	m_pos_x = 1.0f;
	m_pos_y = 20.0f;
	
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;
	m_brush_player.texture = m_state->getFullAssetPath("3.png");

	m_sprites.push_back(m_state->getFullAssetPath("1.png"));
	m_sprites.push_back(m_state->getFullAssetPath("2.png"));
	m_sprites.push_back(m_state->getFullAssetPath("3.png"));
	m_sprites.push_back(m_state->getFullAssetPath("4.png"));
	m_sprites.push_back(m_state->getFullAssetPath("5.png"));
	m_sprites.push_back(m_state->getFullAssetPath("6.png"));


	for (int i = 0; i < FLAME_NUMBER; i++)
		flames[i] = nullptr;

	// Adjust width for finer collision detection
	m_width = 0.8f;
}

Player::~Player()
{
	for (int i = 0; i < FLAME_NUMBER; i++)
	{
		if (flames[i])
		{
			delete flames[i];
			flames[i] = nullptr;
		}
	}
	delete[] flames;
}

void Player::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_state->getCanvasWidth()*0.5f, m_state->getCanvasHeight() * 0.5f, m_width, m_height, debug_brush);
	
	char s[20];
	sprintf_s(s,"(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 1, 0, 0);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(m_state->getCanvasWidth() * 0.5f - 0.4f, m_state->getCanvasHeight() * 0.5f - 0.6f, 0.15f, s, debug_brush);
}

void Player::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;

	// Stage 2 code: Acceleration-based velocity
	float move = 0.0f;
	if (graphics::getKeyState(graphics::SCANCODE_A))
		move -= 1.0f;
	if (graphics::getKeyState(graphics::SCANCODE_D))
		move = 1.0f;

	m_vx = std::min<float>(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal);  //papa epitaxunei aristera i deksia
	m_vx = std::max<float>(-m_max_velocity, m_vx); //mexri max velocity

	// friction
	m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx)); //papa epivradunsi 

	// apply static friction threshold
	if (fabs(m_vx) < 0.01f)
		m_vx = 0.0f;

	// adjust horizontal position
	m_pos_x += m_vx * delta_time;

	// jump only when not in flight:
	if (m_vy == 0.0f)
		m_vy -= (graphics::getKeyState(graphics::SCANCODE_W) ? m_accel_vertical : 0.0f) * 0.02f;// not delta_time!! Burst 

	// add gravity
	m_vy += delta_time * m_gravity;

	// adjust vertical position
	m_pos_y += m_vy * delta_time;

	
}

void Player::shoot(float dt)
{
	//shoot
	if (graphics::getKeyState(graphics::SCANCODE_SPACE))
	{
		if (currentFlames < FLAME_NUMBER)
		{
			flames[currentFlames] = new Flame(m_pos_x, m_pos_y);
			currentFlames++;
		}
		else
			emptyOfFlames = true; 
	}

	//an adeiasei o gemisthras perimenw na fygoyn oles oi sfaires apo to canvas gia na ksanagemhsw
	if (emptyOfFlames)
	{
		for (int i = 0; i < FLAME_NUMBER; i++)
		{
			if (flames[i] != nullptr)
				break;
			if (i == FLAME_NUMBER - 1)
			{
				currentFlames = 0;
				emptyOfFlames = false;
			}
		}
	}

	//update flames
	for (int i = 0; i < FLAME_NUMBER; i++)
	{
		if (flames[i])
			flames[i]->update(m_pos_x);
	}

	this->checkFlames();
}

//An yparxei Ammo kai den einai Active delete it
void Player::checkFlames()
{
	for (int i = 0; i < FLAME_NUMBER; i++)
	{
		if (flames[i] && !flames[i]->isActive())
		{
			delete flames[i];
			flames[i] = nullptr;
		}
	}
}

float Player::getPositionX() const
{
	return m_pos_x + m_state->m_global_offset_x;
}

float Player::getPositionY() const
{
	return m_pos_y + m_state->m_global_offset_y;
}

