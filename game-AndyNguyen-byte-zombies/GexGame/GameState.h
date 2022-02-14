#pragma once

#include "State.h"
#include "World.h"
#include "PlayerControl.h"

class GameState : public State
{
public:
	GameState(StateStack& stack, Context context);

	void					draw() override;
	bool					update(sf::Time dt) override;
	bool					handleEvent(const sf::Event& event) override;

private:
	World					world;
	PlayerControl&			player;

};
