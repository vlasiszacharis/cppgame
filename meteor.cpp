#include "meteor.h"
#include "gamestate.h"


void Meteor::update(float dt)
{
	m_pos_x -= speed * dt ;
	rotation += 0.05f* dt;
	rotation = fmodf(rotation, 360);

	if (m_pos_x < -size)
		active = false;
}

void Meteor::draw()
{
	graphics::setOrientation(rotation);
	brush.outline_opacity = 0.0f;
	brush.texture = m_state->getFullAssetPath("meteor_shadow.png");
	brush.fill_opacity = 0.5f;
	brush.texture = m_state->getFullAssetPath("meteor.png");
	brush.fill_opacity = 1.0f;
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y + +m_state->m_global_offset_y, size, size, brush);
	graphics::resetPose();
		
	if (m_state->m_debugging)
		debugDraw();
}

void Meteor::init()
{
	speed = 0.002f;
	size = 0.5f + 1 * rand0to1();

	m_pos_x = m_state->getCanvasWidth() - m_state->m_global_offset_x + size; 
	m_pos_y = rand0to1() * (m_state->getCanvasHeight() - m_state->m_global_offset_y); 
	m_width = size;
	m_height = size;

	rotation = 360 * rand0to1();
	damage_factor = 1.5f;
}


Meteor::Meteor()
{
	init();
}

Meteor::~Meteor()
{
}

void Meteor::debugDraw()
{
	SETCOLOR(m_meteor_brush_debug.fill_color, 1, 0.3f, 0);
	SETCOLOR(m_meteor_brush_debug.outline_color, 1, 0.1f, 0);
	m_meteor_brush_debug.fill_opacity = 0.1f;
	m_meteor_brush_debug.outline_opacity = 1.0f;
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x, m_pos_y+ +m_state->m_global_offset_y, m_width, m_height, m_meteor_brush_debug);

	char s[20];
	sprintf_s(s, "(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(m_meteor_brush_debug.fill_color, 1, 0, 0);
	m_meteor_brush_debug.fill_opacity = 1.0f;
	graphics::drawText(m_pos_x + m_state->m_global_offset_x - 0.4f, m_pos_y + m_state->m_global_offset_y - 0.6f, 0.15f, s, m_meteor_brush_debug);
}