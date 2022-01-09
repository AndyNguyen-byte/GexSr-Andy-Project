#pragma once
#include <SFML/Graphics.hpp>
#include "Ultility.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "PlayerControl.h"

#include "World.h"

class Game
{
public:
	Game(const Game&) = delete;
	Game();


	void				run();

private:
	void				processInput();
	void				update(sf::Time dt);
	void				render();

private:
	static const sf::Time		TIME_PER_FRAME;

	sf::RenderWindow	window;//must be before world
	World				world;
	PlayerControl		player;
	
};

