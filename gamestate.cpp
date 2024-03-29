 #include "gamestate.h"
#include "level.h"
#include "player.h"
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

GameState::GameState()
{
}

GameState::~GameState()
{
	if (m_current_level)
		delete m_current_level;
}

GameState* GameState::getInstance()
{
	if (!m_unique_instance)
	{
		m_unique_instance = new GameState();
	}
	return m_unique_instance;
}

bool GameState::init()
{
	m_current_level = new Level("1.lvl"); // papa
	m_current_level->init(); // papa
	
	m_player = new Player("Player"); // papa
	m_player->init(); // papa

	graphics::preloadBitmaps(getAssetDir()); // papa
	graphics::setFont(m_asset_path + "font.otf"); // papa
	graphics::playMusic(m_asset_path + "music.mp3", 0.1f, true, 4000);

	return true;
}

void GameState::draw()
{
	if (!m_current_level) // papa
		return;

	m_current_level->draw();	 // papa  , deixnei sto current leven, stin dikia mas periptosi exei ena level
}


void GameState::update(float dt)  
{
	// Skip an update if a long delay is detected 
	// to avoid messing up the collision simulation
	if (dt > 500) // ms// papa
		return;
	
	// Avoid too quick updates
	float sleep_time = std::max(17.0f - dt, 0.0f);
	if (sleep_time > 0.0f)
	{
		std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));
	}

	if (!m_current_level)// papa
		return;

	m_current_level->update(dt);// papa SXEDOAZEO TP TREXPM EPIPEDO

	m_debugging = graphics::getKeyState(graphics::SCANCODE_0); //state olo to paixnidiou Αυτή η γραμμή επιτρέπει στο παιχνίδι 
	//σας να ενεργοποιήσει ή να απενεργοποιήσει τη λειτουργία debugging πατώντας το πλήκτρο '0'.
	

}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}

std::string GameState::getAssetDir()
{
	return m_asset_path;
}

GameState* GameState::m_unique_instance = nullptr;