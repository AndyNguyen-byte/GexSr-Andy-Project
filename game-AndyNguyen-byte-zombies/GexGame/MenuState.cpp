#include "MenuState.h"
#include "State.h"
#include "Ultility.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "MusicPlayer.h"

#include <SFML/Graphics/RenderWindow.hpp>

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, optionsIndex(0)
{
	context.music->play(MusicID::MenuTheme);

	sf::Texture& texture = context.textures->get(TextureID::TitleScreen);

	backgroundSprite.setTexture(texture);

	// set up menu

	// play option 
	sf::Text playOption;
	playOption.setFont(context.fonts->get(FontID::Main));
	playOption.setString("Play");
	centerOrigin(playOption);
	playOption.setPosition(context.window->getView().getSize() / 2.f);

	options.push_back(playOption);

	// exit option 
	sf::Text exitOption;
	exitOption.setFont(context.fonts->get(FontID::Main));
	exitOption.setString("Exit");
	centerOrigin(exitOption);
	exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
	options.push_back(exitOption);

	updateOptionText();
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
	window.draw(backgroundSprite);

	for (const sf::Text& text : options)
		window.draw(text);
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
		return true;

	if (event.key.code == sf::Keyboard::Return)
	{
		if (optionsIndex == Play)
		{
			requestStackPop();
			requestStackPush(StateID::Game);
		}
		else if (optionsIndex == Exit)
		{
			requestStackPop();
		}
	}
	else if ((event.key.code == sf::Keyboard::Up))
	{
		if (optionsIndex > 0)
			optionsIndex -= 1;
		else
			optionsIndex = options.size() - 1;

		updateOptionText();
	}
	else if ((event.key.code == sf::Keyboard::Down))
	{
		if (optionsIndex < options.size() - 1)
			optionsIndex += 1;
		else
			optionsIndex = 0;

		updateOptionText();
	}

	return true;
}

void MenuState::updateOptionText()
{
	if (options.empty())
		return;

	for (sf::Text& text : options)
	{
		text.setFillColor(sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
	}

	options[optionsIndex].setFillColor(sf::Color::Magenta);
}
