#include "gate.h"

void Gate::draw()
{
	graphics::Brush br;
	br.texture = m_state->getFullAssetPath("fireball.png");
	br.fill_opacity = 1.0f;
	br.outline_opacity = 0.0f;
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + m_state->m_global_offset_y, m_width, m_height, br);
	graphics::resetPose();
}
