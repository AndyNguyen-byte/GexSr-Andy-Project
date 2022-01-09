//
//
#pragma once

#include "State.h"

#include <SFML/Graphics/Text.hpp>

class GameOverState : public State
{
public:

	GameOverState( StateStack& stack, Context context);


	void					draw() override;
	bool					update(sf::Time dt) override;
	bool					handleEvent(const sf::Event& event) override;

private:

	sf::Text				gameOverText;
	sf::Time				elapsedTime;

};

