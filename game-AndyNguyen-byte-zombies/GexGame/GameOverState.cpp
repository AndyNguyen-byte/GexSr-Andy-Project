#include "GameOverState.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include "GameState.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


GameOverState::GameOverState(StateStack& stack, Context context)
	: State(stack, context)
	, gameOverText()
	, elapsedTime(sf::Time::Zero)
{
	sf::Font& font = context.fonts->get(FontID::Main);
	sf::Vector2f windowSize(context.window->getSize());

	gameOverText.setFont(font);

	if (context.player->getMissionStatus() == PlayerControl::MissionStatus::MissionFailure)
		gameOverText.setString("Mission Failure");
	else
		gameOverText.setString("Mission Success");

	gameOverText.setCharacterSize(70);
	centerOrigin(gameOverText);
	gameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(50, 50, 50, 15));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(gameOverText);
}

bool GameOverState::update(sf::Time dt)
{
	elapsedTime += dt;
	if (elapsedTime > sf::seconds(3))
	{
		requestStackClear();
		requestStackPush(StateID::Menu);
	}
	return false;
}

bool GameOverState::handleEvent(const sf::Event& event)
{
	return false;
}
