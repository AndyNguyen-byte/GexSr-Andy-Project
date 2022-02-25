#include "Turtle.h"
#include "DataTables.h"
#include "Utility.h"
#include "Category.h"
#include "TextNode.h"
#include "JsonFrameParser.h"
#include "ResourceIdentifiers.h"
#include "SoundNode.h"


#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <memory>
#include <stdexcept>

const float TITLE_HEIGHT = 1024 / 16;
const float TITLE_WIDTH = 578 / 15;
const float GRAVITY = 350.f;
const float FLYING_SPEED = -500.f;
const float FLYING_DURATION = 0.25f;

Turtle::Turtle(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts)
	:Entity(100)
	, type(type)
	, state(State::Alive)
	, direction(Direction::Up)
	, sprite(textures.get(TextureID::Turtle1), sf::IntRect(144.6, 0, 144.6, 101))
	, score(0)
	, pointsDisplay(nullptr)
	, _movementClock()
{
	centerOrigin(sprite);
}

void Turtle::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

unsigned int Turtle::getCategory() const
{
	return Category::Turtle;
}

void Turtle::updateScore(int points)
{
	score += points;
}

int Turtle::getScore()
{
	return score;
}

void Turtle::setDeathStatus(bool s)
{
	isDead = s;
}

void Turtle::updateFlyAnimation()
{
	sf::IntRect textureRect = sf::IntRect(144.6, 0, 144.6, 101);

	// Roll left: Texture rect offset once
	if (getVelocity().y < 0.f)
	{
		textureRect.left += textureRect.width;
		sprite.setRotation(-15);
	}
	// Roll right: Texture rect offset twice
	else if (getVelocity().y > 0.f)
	{
		textureRect.left -= textureRect.width;
		sprite.setRotation(15);
	}
	sprite.setTextureRect(textureRect);
}

sf::FloatRect Turtle::getBoundingRect() const
{
	return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

bool Turtle::isMarkedForRemoval() const
{
	return isDestroyed();
}

void Turtle::remove()
{
	Entity::remove();
}

void Turtle::playLocalSound(CommandQueue& commands, SoundEffectID effect)
{
	Command playSoundCommand;
	auto pos = getWorldPosition();
	playSoundCommand.category = Category::SoundEffect;
	playSoundCommand.action = derivedAction<SoundNode>([pos, effect](SoundNode& node, sf::Time)
		{
			node.playSound(effect, pos);
		});

	commands.push(playSoundCommand);
}

void Turtle::hop(Direction hopDirection)
{
	switch (hopDirection)
	{
		case Direction::Up:
			direction = Direction::Up;
			_movementClock.restart();
			setVelocity(0.f, 0.f);
			state = State::Flying;
			break;
		default:
		break;
	}
}

void Turtle::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	centerOrigin(sprite);

	if (Turtle::State::Falling == state)
	{
		accelerate(0.f, GRAVITY * dt.asSeconds());
	} else if (Turtle::State::Flying == state)
	{
		accelerate(0.f, FLYING_SPEED * dt.asSeconds());
	}

	if (_movementClock.getElapsedTime().asSeconds() > FLYING_DURATION)
	{
		_movementClock.restart();
		state = State::Falling;
	}

	updateFlyAnimation();

	Entity::updateCurrent(dt, commands);
}

void Turtle::updateTexts()
{
	pointsDisplay->setText("Score: " + std::to_string(getScore()));
	pointsDisplay->setPosition(0.f, 50.f);
	pointsDisplay->setRotation(-getRotation());
}
