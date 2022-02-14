#include "Actor.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "JsonFrameParser.h"
#include "Utility.h"
#include "DataTables.h"
#include "TextNode.h"

#include <iostream>

#include <SFML/Graphics/CircleShape.hpp>

namespace
{
	const std::map<Actor::Type, ActorData> TABLE = initializeActorData();
}

Actor::Actor(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts)
	: Entity(100)
	, type(type)
	, state(State::Walk)
	, sprite(textures.get(TABLE.at(type).texture))
	, direction(Direction::Right)
	, travelDistance(0.f)
	, directionIndex(0)
	, attack_(false)
	, forceFieldTimer(sf::Time::Zero)
	, forceField_(false)

{
	for (auto a : TABLE.at(type).animations)
	{
		animations[a.first] = a.second;
	}


	//if (Actor::getCategory() == Category::Frog)
	//	state = State::Idle;

	sprite.setTextureRect(sf::IntRect());
	centerOrigin(sprite);

	//
	// set up text for health and missiles
	// 
	std::unique_ptr<TextNode> health(new TextNode(fonts, ""));
	healthDisplay = health.get();
	attachChild(std::move(health));

	updateTexts();
}



unsigned int Actor::getCategory() const
{
	switch (type)
	{
	case Type::Hero2:
		return Category::Hero;
		break;
	case Type::Zombie1:
	case Type::Zombie2:
	case Type::Zombie3:
	case Type::Zombie4:
	case Type::Zombie5:
		return Category::Zombie;
		break;
	}

	return Category::Zombie;
}

void Actor::updateMovementPattern(sf::Time dt)
{
	// movement pattern
	const auto directions = TABLE.at(type).directions;

	if (!directions.empty())
	{
		if (travelDistance > (directions[directionIndex].distance))
		{
			directionIndex = (++directionIndex) % directions.size();
			travelDistance = 0;
		}

		float bering = directions[directionIndex].angle + 90.f;
		float vx = getMaxSpeed() * gex::cos(bering);
		float vy = getMaxSpeed() * gex::sin(bering);

		setVelocity(vx, vy);
		travelDistance += getMaxSpeed() * dt.asSeconds();
	}
}

sf::FloatRect Actor::getBoundingRect() const
{
	auto box = getWorldTransform().transformRect(sprite.getGlobalBounds());
	box.width -= 30; // tighten up bounding box for more realistic collisions
	box.left += 15;
	return box;
}

float Actor::getMaxSpeed() const
{
	return TABLE.at(type).speed;
}

void Actor::accelerate(sf::Vector2f velocity)
{
	switch (state)
	{
	case State::Attack:	
		Entity::accelerate(velocity);
		break;
	case State::Walk:
		Entity::accelerate(velocity);
		break;
	case State::Run:
		Entity::accelerate(velocity * 2.f);
	}
}

void Actor::accelerate(float vx, float vy)
{
	accelerate(sf::Vector2f(vx, vy));
}

void Actor::hop(float x, float y)
{
}

bool Actor::isMarkedForRemoval() const
{
	return false; // (state_ == State::Dead && animations_.at(state_).isFinished());
}

void Actor::damage(int points)
{
	if (!forceField_)
		Entity::damage(points);
}

void Actor::attack()
{
	attack_ = true;
}


void Actor::setState(State state)
{
	state = state;
	animations[state].restart();
}

int Actor::attackPoints() const
{
	if (type == Type::Hero2 && state == State::Attack)
		return TABLE.at(type).damageDone;
	else if (type != Type::Hero2)
		return TABLE.at(type).damageDone;
	else
		return 0;
}


void Actor::updateStates()
{
	if (isDestroyed())
		state = State::Dead;

	if (state == State::Attack && animations[state].isFinished())
		state = State::Walk;

	if (attack_ && state != State::Attack)
	{
		state = State::Attack;
		animations[state].restart();
		attack_ = false;
	}

	if (state == State::Rise && animations[state].isFinished())
		state = State::Idle;

	if (state == State::Idle && length(getVelocity()) > 0.1f)
		state = State::Walk;
}


void Actor::forceField()
{
	if (!forceField_ && forceFieldTimer <= sf::Time::Zero)
	{
		forceField_ = true;
		forceFieldTimer = sf::seconds(3);
	}
}
void Actor::updateForceField(sf::Time dt)
{
	if (forceFieldTimer > sf::Time::Zero)
		forceFieldTimer -= dt;

	if (forceField_ && forceFieldTimer < sf::Time::Zero)
	{
		forceField_ = false;
		forceFieldTimer = sf::seconds(3);
	}
}


void Actor::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	updateStates();
	updateForceField(dt);

	auto rec = animations.at(state).update(dt);

	// std::cout << animations.at(state).currentFrame << "\n";
	if (state != State::Dead)
	{
		if (direction == Direction::Left && getVelocity().x > 10)
			direction = Direction::Right;
		if (direction == Direction::Right && getVelocity().x < 0)
			direction = Direction::Left;
	}

	// flip image left right
	if (direction == Direction::Left)
		rec = flip(rec);

	sprite.setTextureRect(rec);
	centerOrigin(sprite);

	if (state != State::Dead) // dont move it while dying
		Entity::updateCurrent(dt, commands);

	updateMovementPattern(dt);
	updateTexts();
}

void Actor::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{

	if (forceField_)
	{
		sf::CircleShape circle;
		circle.setRadius(60);
		circle.setOrigin(60, 60);
		circle.setFillColor(sf::Color(0, 255, 255, 100));
		circle.setOutlineColor(sf::Color(0, 255, 255));
		circle.setOutlineThickness(1);
		circle.setPosition(0, 0);
		target.draw(circle, states);
	}

	target.draw(sprite, states);
}

void Actor::updateTexts()
{
	healthDisplay->setText(std::to_string(getHitpoints()) + "HP");
	healthDisplay->setPosition(0.f, 70.f);
	healthDisplay->setRotation(-getRotation());
}

