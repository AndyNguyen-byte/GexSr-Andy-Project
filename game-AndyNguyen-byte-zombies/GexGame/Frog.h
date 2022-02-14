#pragma once


#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Animation2.h"
#include "ResourceIdentifiers.h"

#include <map>

#include <SFML/Graphics/Sprite.hpp>

class TextNode;

class Frog : public Entity
{
public:
	enum class State {
		HopLeft,
		HopRight,
		HopUp,
		HopDown,

		Alive,
		Die,
		Count
	};

	enum class Direction
	{
		Left,
		Right,
		Up,
		Down
	};

	enum class Type
	{
		PlayerFrog,
		BonusFrog
	};

public:
	Frog(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts);

	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual unsigned int	getCategory() const override;

	void					updateScore(int points);
	int						getScore();
	void					setDeathStatus(bool s);

	bool					isOnRiver();
	bool					isInBounds();

	virtual sf::FloatRect	getBoundingRect() const override;


	virtual bool			isMarkedForRemoval() const override;
	virtual void			remove() override;

	void					hop(Direction hopDirection);

	void					playDeathAnimation(sf::Time dt);

	void					playLocalSound(CommandQueue& commands, SoundEffectID effect);

private:
	void					updateStates();
	

	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands) override;
	void					updateTexts();

private:
	Type										type;
	State										state;
	Direction									direction;
	sf::Sprite									sprite;

	std::map<Direction, Animation2>				animations;

	bool										isDead = false;
	bool										hasPlayedDeathSound;
	bool										isHopping = false;
	bool										hasPlayedHopSound;


	int											score = 0;
	TextNode*									pointsDisplay;
};
