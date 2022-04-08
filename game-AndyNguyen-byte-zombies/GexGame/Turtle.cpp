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
	, sprite(textures.get(TextureID::Turtle1), sf::IntRect(173, 15, 77, 53))
	, score(0)
	, pointsDisplay(nullptr)
	, _movementClock()
{
	centerOrigin(sprite);

	std::unique_ptr<TextNode> score(new TextNode(fonts, ""));
	pointsDisplay = score.get(); ///< raw pointer to scoreDisplay node
	attachChild(std::move(score));

	updateTexts();
}

void Turtle::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
	//drawBoundingBox(target, states);
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
	sf::IntRect textureRect = sf::IntRect(173, 15, 77, 59);

	// Fly Up: Texture rect offset twice
	if (getVelocity().y < 0.f)
	{
		textureRect.left += 2*textureRect.width;
		sprite.setRotation(-15);
	}
	// Fly Right: Texture rect offset twice
	else if (getVelocity().y > 0.f)
	{
		textureRect.left = 30;
		sprite.setRotation(15);
	}
	sprite.setTextureRect(textureRect);
}

void Turtle::reverseGravity(bool b)
{
	atStart = false;
	reverse = b;
}

void Turtle::flipPointDisplay(bool state)
{
	if (state)
	{
		pointsDisplay->setScale(-1.f, 1.f);
	}
	else
	{
		pointsDisplay->setScale(-1.f, 1.f);
		pointsDisplay->setScale(-1.f, 1.f);
	}
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
			isHopping = true;
			state = State::Flying;
			break;
		default:
		break;
	}
}

void Turtle::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	centerOrigin(sprite);

	updateTexts();

	if (Turtle::State::Falling == state)
	{
		if (reverse)
		{
			accelerate(0.f, -GRAVITY * dt.asSeconds());
		}
		else {
			accelerate(0.f, GRAVITY * dt.asSeconds());
		}
	} else if (Turtle::State::Flying == state)
	{
		if (reverse)
		{
			accelerate(0.f, -FLYING_SPEED * dt.asSeconds());
		}
		else {
			accelerate(0.f, FLYING_SPEED * dt.asSeconds());
		}
	}

	if (_movementClock.getElapsedTime().asSeconds() > FLYING_DURATION)
	{
		_movementClock.restart();
		state = State::Falling;
	}

	if (isHopping) {
		isHopping = false;
		SoundEffectID effect = SoundEffectID::HopSound;
		playLocalSound(commands, effect);
		hasPlayedHopSound = true;
	}
	else {
		hasPlayedHopSound = false;
	}

	if (isDead) {
		if (!hasPlayedDeathSound)
		{
			hasPlayedDeathSound = true;
			SoundEffectID effect = SoundEffectID::DeathSound;
			SoundEffectID effect2 = SoundEffectID::HitSound;
			playLocalSound(commands, effect2);
			playLocalSound(commands, effect);
		}
	}

	updateFlyAnimation();

	Entity::updateCurrent(dt, commands);
}

void Turtle::updateTexts()
{
	pointsDisplay->setText("Score: " + std::to_string(getScore()));
	pointsDisplay->setPosition(0.f, 50.f);
	if (!reverse) {
		pointsDisplay->setRotation(-getRotation());
		if (!atStart)
		{
			flipPointDisplay(false);
		}
	}
	else {
		pointsDisplay->setRotation(getRotation());
		flipPointDisplay(true);
	}
}
