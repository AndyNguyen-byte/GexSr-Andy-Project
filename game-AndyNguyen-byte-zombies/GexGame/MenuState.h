#pragma once


#include "State.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class MenuState : public State
{
public:
	MenuState(StateStack& stack, Context context);

	virtual void			draw() override;
	virtual bool			update(sf::Time dt) override;
	virtual bool			handleEvent(const sf::Event& event) override;

	void					updateOptionText();


private:
	enum OptionNames
	{
		Play,
		Exit,
	};


private:
	sf::Sprite				backgroundSprite;

	std::vector<sf::Text>	options;
	std::size_t				optionsIndex;
};