#include "PauseState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


PauseState::PauseState(StateStack& stack, Context context)
    : State(stack, context)
    , pausedText()
    , instructionText()
{
	//context.music->setPaused(true);

	pausedText.setFont(context.fonts->get(FontID::Main));
	pausedText.setString("Game Paused");
	pausedText.setCharacterSize(80);
	centerOrigin(pausedText);

	instructionText.setFont(context.fonts->get(FontID::Main));
	instructionText.setString("(Press Backspace to return to the main menu)");
	centerOrigin(instructionText);

	sf::Vector2f viewSize = context.window->getView().getSize();
	pausedText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);
	instructionText.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);
}

PauseState::~PauseState()
{
	//getContext().music->setPaused(false);
}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(100, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(pausedText);
	window.draw(instructionText);

}

bool PauseState::update(sf::Time dt)
{
    return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Escape)
		requestStackPop();

	if (event.key.code == sf::Keyboard::BackSpace)
	{
		requestStackClear();
		requestStackPush(StateID::Menu);
	}

	return false;
}
