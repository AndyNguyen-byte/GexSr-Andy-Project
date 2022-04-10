#pragma once

#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Animation2.h"
#include "ResourceIdentifiers.h"

#include <map>

#include <SFML/Graphics/Sprite.hpp>
#include "TextNode.h"

class Turtle : public Entity
{
public:
	enum class State {
		Alive,
		Die,
		Still,
		Falling,
		Flying,
		Count
	};

	enum class Direction
	{
		Up
	};


	enum class Type
	{
		NormalTurtle,
		ExtraTurtle,
		ExtremeTurle
	};


public:
	Turtle(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts);

	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual unsigned int	getCategory() const override;

	void					updateScore(int points);
	int						getScore();
	void					setDeathStatus(bool s);
	void					reverseGravity(bool b);
	virtual sf::FloatRect	getBoundingRect() const override;


	virtual bool			isMarkedForRemoval() const override;
	virtual void			remove() override;

	void					hop(Direction hopDirection);

	void					playLocalSound(CommandQueue& commands, SoundEffectID effect);

private:

	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands) override;
	void					updateTexts();

	void					updateFlyAnimation();

private:
	Type										type;
	State										state;
	Direction									direction;
	sf::Sprite									sprite;
	sf::Clock									_movementClock;

	bool										isDead = false;
	bool										reverse = false;
	bool										isHopping = false;
	bool										atStart = true;
	bool										hasPlayedHopSound = false;
	bool										hasPlayedDeathSound = false;

	int											score = 0;
	TextNode*									pointsDisplay;
};

