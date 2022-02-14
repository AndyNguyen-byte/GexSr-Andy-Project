#include "StateStack.h"
#include <cassert>

StateStack::StateStack(State::Context context)
	: stack()
	, pendingList()
	, context(context)
	, factories()
{}

void StateStack::update(sf::Time dt)
{
	// top of stack down
	for (auto itr = stack.rbegin(); itr != stack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw()
{
	// painter algorithm draw from the bottom up
	for (State::Ptr& state : stack)
		state->draw();
}

void StateStack::handleEvent(const sf::Event& event)
{
	for (auto itr = stack.rbegin(); itr != stack.rend(); ++itr)
	{
		if ((*itr)->handleEvent(event) == false)
			break;
	}

	applyPendingChanges();
}

void StateStack::pushState(StateID stateID)
{
	pendingList.push_back(PendingChange(Action::Push, stateID));

}

void StateStack::popState()
{
	pendingList.push_back(PendingChange(Action::Pop));

}

void StateStack::clearStates()
{
	pendingList.push_back(PendingChange(Action::Clear));
}

bool StateStack::isEmpty() const
{
    return stack.empty();
}

State::Ptr StateStack::createState(StateID stateID)
{
	auto found = factories.find(stateID);
	assert(found != factories.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (PendingChange change : pendingList)
	{
		switch (change.action)
		{
		case Action::Push:
			stack.push_back(createState(change.stateID));
			break;

		case Action::Pop:
			stack.pop_back();
			break;

		case Action::Clear:
			stack.clear();
			break;
		}
	}

	pendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, StateID stateID)
	: action(action)
	, stateID(stateID)
{}
