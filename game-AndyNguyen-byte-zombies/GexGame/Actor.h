#pragma once
#include "Entity.h"
#include "Animation2.h"
#include "ResourceIdentifiers.h"

#include <map>

#include <SFML/Graphics/Sprite.hpp>


class TextureManager;
class TextNode;

class Actor : public Entity
{
 
public:
	enum class Type
	{
		Hero2,
		Zombie1,
		Zombie2,
		Zombie3,
		Zombie4,
		Zombie5,

		Frog

	};
	
	enum class State {
		Attack,
		Dead,
		Jump,
		Rise,
		Walk,
		Run,

		HopLeft,
		HopRight,
		HopUp,
		HopDown,
		Idle,
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


public:
					Actor(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts);
					~Actor() = default;

	unsigned int	getCategory() const override;
	sf::FloatRect	getBoundingRect() const override;
	float			getMaxSpeed() const;
	void			accelerate(sf::Vector2f velocity) override;
	void			accelerate(float vx, float vy) override;
	void			hop(float x, float y) override;
	bool			isMarkedForRemoval() const override;
	void			damage(int points) override;

	void		    attack();
	void			forceField();
	void			setState(State state);
	int				attackPoints() const;



private:
	void			updateStates();
	void			updateForceField(sf::Time dt);
	void			updateCurrent(sf::Time dt, CommandQueue& commands) override;
	void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	void			updateMovementPattern(sf::Time dt);
	void			updateTexts();


private:
	Type										type;
	State										state;
	mutable sf::Sprite							sprite;
	std::map<State, Animation2>					animations;
	Direction									direction;
	TextNode*									healthDisplay;

	float										travelDistance;
	std::size_t									directionIndex;
	bool										attack_;
	sf::Time									forceFieldTimer;
	bool										forceField_;

};

