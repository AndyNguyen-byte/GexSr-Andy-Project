#include "PlayerControl.h"

#include "Aircraft.h"
#include "Actor.h"
#include "Command.h"
#include "CommandQueue.h"

#include <iostream> 
#include "Frog.h"
#include "Turtle.h"

PlayerControl::PlayerControl()
	: currentMissionStatus(MissionStatus::MissionRunning)
{
	initializeKeys();
	initializeActions();
}

void PlayerControl::initializeKeys()
{
	// Set initial key bindings


	keyBindings[sf::Keyboard::J] = Action::Jump;
	keyBindings[sf::Keyboard::Space] = Action::Jump;

	 
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
	actionBindings[Action::Jump].action = derivedAction<Turtle>([=](Turtle& n, sf::Time dt) { n.hop(Turtle::Direction::Up); });
	actionBindings[Action::Jump].category = Category::Turtle;
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
