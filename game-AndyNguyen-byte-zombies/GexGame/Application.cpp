#include "Application.h"
#include "State.h"

#include "TitleState.h"
#include "GameState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "GameOverState.h"

const sf::Time	Application::TIME_PER_FRAME = sf::seconds((1.f / 60.f));

Application::Application()
	: window(sf::VideoMode(578.f, 1024.f), "Flappy Turtle GEX", sf::Style::Close)
	, textures()
 	, fonts()
	, player()
	, stateStack(State::Context(window, textures, fonts, player, music, sound))
	, statisticsText()
	, statisticsUpdateTime()
	, statisticsNumFrames(0)
{ 

	window.setKeyRepeatEnabled(false);

	fonts.load(FontID::Main, "../Media/Sansation.ttf");
	textures.load(TextureID::TitleScreen, "../Media/Textures/FroggerTitle.png");

	statisticsText.setFont(fonts.get(FontID::Main));
	statisticsText.setPosition(15.0f, 15.0f);
	statisticsText.setCharacterSize(15);
	

	registerStates();
	stateStack.pushState(StateID::Title);

}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (window.isOpen()) {

		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate > TIME_PER_FRAME) {
			timeSinceLastUpdate -= TIME_PER_FRAME;

			processInput();
			update(TIME_PER_FRAME);

			// Check inside this loop, 
			// because stack might be empty 
			// before update() call
			if (stateStack.isEmpty())
				window.close();

		}

		updateStatistics(elapsedTime);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		stateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			window.close();
	}
}

void Application::update(sf::Time dt)
{
	stateStack.update(dt);
}

void Application::render()
{
	window.clear();
	stateStack.draw();

	window.setView(window.getDefaultView());
	window.draw(statisticsText);

	window.display();
}

void Application::updateStatistics(sf::Time dt)
{
	statisticsUpdateTime += dt;
	statisticsNumFrames += 1;
	if (statisticsUpdateTime >= sf::seconds(1.0f))
	{
		statisticsText.setString("FPS: " + std::to_string(statisticsNumFrames));

		statisticsUpdateTime -= sf::seconds(1.0f);
		statisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	stateStack.registerState<TitleState>(StateID::Title);
	stateStack.registerState<MenuState>(StateID::Menu);
	stateStack.registerState<GameState>(StateID::Game);
	stateStack.registerState<PauseState>(StateID::Pause);
	stateStack.registerState<GameOverState>(StateID::GameOver);
}
