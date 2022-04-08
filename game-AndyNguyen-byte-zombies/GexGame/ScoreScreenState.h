#pragma once
#include "State.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Utility.h"

class ScoreScreenState : public State
{
public:
	ScoreScreenState(StateStack& stack, Context context);

	void		draw() override;
	bool		update(sf::Time dt) override;
	bool		handleEvent(const sf::Event& event) override;


private:
	sf::Text			displayText;
	sf::Text			highScoresText;
	sf::Text			backText;
};

