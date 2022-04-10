#include "GameState.h"
#include "PlayerControl.h"
#include <SFML/Graphics/RenderWindow.hpp>


GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, world(*context.window, *context.fonts, *context.sound)
	, player(*context.player)
	, scoreText()
{
	context.music->play(MusicID::GameTheme);
	sf::Font& font = context.fonts->get(FontID::Main);
	sf::Vector2f windowSize(context.window->getSize());

	scoreText.setString(std::to_string(world.getScore()));
	scoreText.setFont(font);
	scoreText.setCharacterSize(70);
	centerOrigin(scoreText);
	scoreText.setPosition(0.5f * windowSize.x, 0.2f * windowSize.y);
}

void GameState::draw()
{
	world.draw();
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(scoreText);
}

bool GameState::update(sf::Time dt)
{
	world.update(dt);

	scoreText.setString(std::to_string(world.getScore()));

	if (!world.hasAlivePlayer())
	{
		player.setMissionStatus(PlayerControl::MissionStatus::MissionFailure);
		player.setScore(world.getScore());
		player.addScoreToList(world.getScore());
		requestStackPush(StateID::GameOver);
	}
	else if (world.hasPlayerReachedEnd())
	{
		player.setMissionStatus(PlayerControl::MissionStatus::MissionSuccess);
		requestStackPush(StateID::GameOver);
	}


	CommandQueue& commands = world.getCommandQueue();
	player.handleRealTimeInput(commands);

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	// Game input handling
	CommandQueue& commands = world.getCommandQueue();
	player.handleEvent(event, commands);

	// Escape pressed, trigger the pause screen
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(StateID::Pause);

	return true;
}
