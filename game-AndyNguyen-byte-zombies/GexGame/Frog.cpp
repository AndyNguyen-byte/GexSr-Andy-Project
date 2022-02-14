#include "Frog.h"
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

const float TITLE_HEIGHT = 600 / 15;
const float TITLE_WIDTH = 480 / 14;

namespace
{
	const auto TABLE = initializeFrogData();
}

Frog::Frog(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts)
	:Entity(100)
	,type(type)
	,state(State::Alive)
	,direction(Direction::Up)
	,sprite(textures.get(TABLE.at(type).texture))
	,score(0)
	,pointsDisplay(nullptr) 
{
	for (auto a : TABLE.at(type).animations)
	{
		animations[a.first] = a.second;
	}

	if (Frog::getCategory() == Category::PlayerFrog)
		state = State::Alive;

	sprite.setTextureRect(sf::IntRect());

	centerOrigin(sprite);

}

void Frog::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

unsigned int Frog::getCategory() const
{
	switch (type)
	{
	case Type::PlayerFrog:
		return Category::PlayerFrog;
		break;
	case Type::BonusFrog:
		return Category::BonusFrog;
		break;
	}
	return Category::BonusFrog;
}


void Frog::updateScore(int points)
{
	score += points;
}

int Frog::getScore()
{
	return score;
}

void Frog::setDeathStatus(bool s)
{
	isDead = s;
}

bool Frog::isOnRiver()
{
	return getPosition().y < TITLE_HEIGHT*8.5;
}

bool Frog::isInBounds()
{
	return getPosition().x > 0 && getPosition().x < TITLE_WIDTH * 14;
}

sf::FloatRect Frog::getBoundingRect() const
{
	return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

bool Frog::isMarkedForRemoval() const
{
	return isDestroyed();
}

void Frog::remove()
{
	Entity::remove();
}

void Frog::updateStates()
{
	
}

void Frog::hop(Direction hopDirection)
{
	if (!isDead) {
		switch (hopDirection)
		{
		case Direction::Up:
			setPosition(getPosition().x, getPosition().y - TITLE_HEIGHT);
			direction = Direction::Up;
			break;
		case Direction::Down:
			setPosition(getPosition().x, getPosition().y + TITLE_HEIGHT);
			direction = Direction::Down;
			break;
		case Direction::Left:
			setPosition(getPosition().x - TITLE_WIDTH, getPosition().y);
			direction = Direction::Left;
			break;
		case Direction::Right:
			setPosition(getPosition().x + TITLE_WIDTH, getPosition().y);
			direction = Direction::Right;
			break;
		default:
			break;
		}
		//isHopping = true;
	}
}

void Frog::playDeathAnimation(sf::Time dt)
{
	static auto animation = TABLE.at(type).deathAnimation;
	
	if (animation.isFinished())
	{
		isDead = false;
		setPosition(7 * TITLE_WIDTH, 15 * TITLE_HEIGHT);
		animation.restart();
	}
	sprite.setTextureRect(animation.update(dt));

}

void Frog::playLocalSound(CommandQueue& commands, SoundEffectID effect)
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

void Frog::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isDead) {
		playDeathAnimation(dt);
		if (!hasPlayedDeathSound)
		{
			hasPlayedDeathSound = true;
			SoundEffectID effect = SoundEffectID::FrogDie;
			playLocalSound(commands, effect);
		}
	}
	else {
		auto rec = animations.at(direction).update(dt);
		sprite.setTextureRect(rec);
		hasPlayedDeathSound = false;
	}

	/*if (isHopping) {
		isHopping = false;
		SoundEffectID effect = SoundEffectID::FrogJump;
		playLocalSound(commands, effect);
		hasPlayedHopSound = true;
	}
	else {
		hasPlayedHopSound = false;
	}*/
	
	centerOrigin(sprite);

	Entity::updateCurrent(dt, commands);
}

void Frog::updateTexts()
{
	pointsDisplay->setText("Score: " + std::to_string(getScore()));
	pointsDisplay->setPosition(0.f, 50.f);
	pointsDisplay->setRotation(-getRotation());
}
