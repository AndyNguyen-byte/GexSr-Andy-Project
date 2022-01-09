#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include "PlayerControl.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "StateStack.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"

class Application
{
public:
	Application();
	Application(Application&) = delete;

	void						run();

private:
	void						processInput();
	void						update(sf::Time dt);
	void						render();

	void						updateStatistics(sf::Time dt);
	void						registerStates();

private:
	static const sf::Time		TIME_PER_FRAME;

	//context
	sf::RenderWindow			window;
	TextureHolder_t				textures;
	FontHolder_t				fonts;
	PlayerControl				player;
	MusicPlayer					music;
	SoundPlayer					sound;

	//statestack
	StateStack					stateStack;

	//stats
	sf::Text					statisticsText;
	sf::Time					statisticsUpdateTime;
	unsigned int				statisticNumFrames;

};

