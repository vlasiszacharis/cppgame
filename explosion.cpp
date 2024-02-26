#include "explosion.h"

Explosion::Explosion(float p_x, float p_y, float m_size)
{
	pos_x = p_x + m_state->m_global_offset_x;
	pos_y = p_y + m_state->m_global_offset_y;
	size = m_size;
	init();
}

void Explosion::update()
{
	//se sygkekrimenes stigmes to kanei inactive
	if (fmod(graphics::getGlobalTime(), 1000) > 800 && fmod(graphics::getGlobalTime(), 1000) < 900)
	{
		active = false;
	}
}

void Explosion::draw()
{
	graphics::Brush br;
	br.texture = m_state->getFullAssetPath("kaboom.png");
	br.fill_opacity = 1.0f;
	br.outline_opacity = 0.0f;
	graphics::drawRect(pos_x, pos_y, size * 1.5f, size, br);
	graphics::resetPose();
}

void Explosion::init()
{
	graphics::playSound(m_state->getFullAssetPath("explosion.mp3"), 0.4f, false);
}
