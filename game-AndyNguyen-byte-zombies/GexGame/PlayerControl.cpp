#include "PlayerControl.h"

#include "Aircraft.h"
#include "Actor.h"
#include "Command.h"
#include "CommandQueue.h"

#include <iostream> 
#include "Frog.h"

PlayerControl::PlayerControl()
	: currentMissionStatus(MissionStatus::MissionRunning)
{
	initializeKeys();
	initializeActions();
}

void PlayerControl::initializeKeys()
{
	// Set initial key bindings

	// arrows
	keyBindings[sf::Keyboard::Left]		= Action::MoveLeft;
	keyBindings[sf::Keyboard::Right]	= Action::MoveRight;
	keyBindings[sf::Keyboard::Up]		= Action::MoveUp;
	keyBindings[sf::Keyboard::Down]		= Action::MoveDown;

	// AWSD
	keyBindings[sf::Keyboard::A] = Action::MoveLeft;
	keyBindings[sf::Keyboard::D] = Action::MoveRight;
	keyBindings[sf::Keyboard::W] = Action::MoveUp;
	keyBindings[sf::Keyboard::S] = Action::MoveDown;

	keyBindings[sf::Keyboard::J] = Action::Jump;
	keyBindings[sf::Keyboard::Space] = Action::Attack;
	keyBindings[sf::Keyboard::F] = Action::ForceField;

	 
}


void PlayerControl::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		auto found = keyBindings.find(event.key.code);
		
		if ( found != keyBindings.end() && !isRealTimeAction(found->second) )
			commands.push(actionBindings[found->second]);
		
	}
}

void PlayerControl::handleRealTimeInput(CommandQueue& commands)
{

	for (auto p : keyBindings)
	{
		if (sf::Keyboard::isKeyPressed(p.first) && isRealTimeAction(p.second))
			commands.push(actionBindings[p.second]);
	}

}


void PlayerControl::assignKey(Action action, sf::Keyboard::Key key)
{
	// remove all keys that already map to action
	for (auto itr = keyBindings.begin(); itr != keyBindings.end();)
	{
		if (itr->second == action)
			keyBindings.erase(itr++);
		else
			++itr;
	}

	// insert new binding
	keyBindings[key] = action;
}

sf::Keyboard::Key PlayerControl::getAssignedKey(Action action) const
{
	for (auto p : keyBindings)
	{
		if (p.second == action)
			return p.first;
	}
	return sf::Keyboard::Unknown;
}

void PlayerControl::setMissionStatus(MissionStatus status)
{
	currentMissionStatus = status;
}

PlayerControl::MissionStatus PlayerControl::getMissionStatus() const
{
	return currentMissionStatus;
}

void PlayerControl::initializeActions()
{

	actionBindings[Action::MoveLeft].action = derivedAction<Frog>([=](Frog& n, sf::Time dt) { n.hop(Frog::Direction::Left); });
	actionBindings[Action::MoveLeft].category = Category::PlayerFrog;
	
	actionBindings[Action::MoveRight].action = derivedAction<Frog>([=](Frog& n, sf::Time dt) { n.hop(Frog::Direction::Right); });
	actionBindings[Action::MoveRight].category = Category::PlayerFrog;
	
	actionBindings[Action::MoveUp].action = derivedAction<Frog>([=](Frog& n, sf::Time dt) { n.hop(Frog::Direction::Up); });
	actionBindings[Action::MoveUp].category = Category::PlayerFrog;
	
	actionBindings[Action::MoveDown].action = derivedAction<Frog>([=](Frog& n, sf::Time dt) { n.hop(Frog::Direction::Down); });
	actionBindings[Action::MoveDown].category = Category::PlayerFrog;
}

bool PlayerControl::isRealTimeAction(Action action)
{
	switch (action)
	{
	case Action::Fire:
		return true;

	default:
		return false;
	}
}
