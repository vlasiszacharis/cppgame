#include "flame.h"


Flame::Flame(float p_x, float p_y)
{
	m_pos_x = p_x  + 0.5;
	m_pos_y = p_y ;
	init();
}

void Flame::update(float player_pos)
{
	m_pos_x += speed * graphics::getDeltaTime();
	if (m_pos_x > player_pos + m_state->getCanvasWidth() + size)
		active = false; //AN EXEI PERASEI TO KARE
}

void Flame::draw()
{
	graphics::Brush brush;
	brush.outline_opacity = 0.0f;
	brush.texture = m_state->getFullAssetPath("fireball.png");
	brush.fill_opacity = 1.0f;
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y +m_state->m_global_offset_y, size, size, brush);
	graphics::resetPose();

	if (m_state->m_debugging)
		debugDraw();
}

void Flame::init()
{
	graphics::playSound(m_state->getFullAssetPath("shoot.mp3"), 0.6f, false);
}

void Flame::debugDraw()
{
	SETCOLOR(debugBrush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debugBrush.outline_color, 1, 0.1f, 0);
	debugBrush.fill_opacity = 0.1f;
	debugBrush.outline_opacity = 1.0f;
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + +m_state->m_global_offset_y, size, size, debugBrush);

	char s[20];
	sprintf_s(s, "(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debugBrush.fill_color, 1, 0, 0);
	debugBrush.fill_opacity = 1.0f;
	graphics::drawText(m_pos_x + m_state->m_global_offset_x - 0.4f, m_pos_y + m_state->m_global_offset_y - 0.6f, 0.15f, s, debugBrush);
}
