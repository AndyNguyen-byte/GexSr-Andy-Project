#include "ScoreScreenState.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include "PlayerControl.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

ScoreScreenState::ScoreScreenState(StateStack& stack, Context context)
	: State(stack, context)
	,displayText()
	,highScoresText()
	,backText()
{
	sf::Font& font = context.fonts->get(FontID::Main);
	sf::Vector2f windowSize(context.window->getSize());

	displayText.setFont(font);
	highScoresText.setFont(font);
	backText.setFont(font);

	displayText.setString("High Scores");
	highScoresText.setString(context.player->getHighScores());
	backText.setString("Press Esc to Go to Menu");

	displayText.setCharacterSize(40);
	highScoresText.setCharacterSize(40);
	backText.setCharacterSize(40);

	displayText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);
	highScoresText.setPosition(0.5f * windowSize.x, 0.6f * windowSize.y);
	backText.setPosition(0.5f * windowSize.x, 0.8f * windowSize.y);

	centerOrigin(displayText);
	centerOrigin(highScoresText);
	centerOrigin(backText);
}

void ScoreScreenState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(50, 50, 50, 15));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(displayText);
	window.draw(highScoresText);
	window.draw(backText);
}

bool ScoreScreenState::update(sf::Time dt)
{
	return false;
}

bool ScoreScreenState::handleEvent(const sf::Event& event)
{
	if (event.key.code == sf::Keyboard::Escape)
	{
		requestStackPop();
		requestStackPush(StateID::Menu);
		return true;
	}

	return true;
}
