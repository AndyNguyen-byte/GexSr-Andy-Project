#pragma once

#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"
#include "MusicPlayer.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

// forward declaration
namespace sf {
	class RenderWindow;
}

class StateStack;
class PlayerControl;
class SoundPlayer;

class State
{
public:
	using Ptr = std::unique_ptr<State>;

	struct Context
	{
		Context(
			sf::RenderWindow& window, 
			TextureHolder_t& textures, 
			FontHolder_t& fonts, 
			PlayerControl& player,
			MusicPlayer& music,
			SoundPlayer& sound);

		sf::RenderWindow* window;
		TextureHolder_t* textures;
		FontHolder_t* fonts;
		PlayerControl* player;
		MusicPlayer* music;
		SoundPlayer* sound;
	};

public:
	State(StateStack& stack, Context context);
	virtual		~State();

	// abstract member function 
	virtual void	draw() = 0;
	virtual bool	update(sf::Time dt) = 0;
	virtual bool	handleEvent(const sf::Event& event) = 0;


protected:
	void				requestStackPush(StateID stateID);
	void				requestStackPop();
	void				requestStackClear();

	Context				getContext() const;


private:
	StateStack*		stack;
	Context			context;

};

