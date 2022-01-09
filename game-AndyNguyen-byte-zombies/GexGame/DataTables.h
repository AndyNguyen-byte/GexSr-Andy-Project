#pragma once

#include "ResourceIdentifiers.h"
#include "Aircraft.h"
#include "Projectile.h"
#include "Pickup.h"
#include "Particle.h"
#include "Actor.h"
#include "Animation2.h"
#include "JsonFrameParser.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>
#include <map>

struct Direction
{
	Direction(float a, float d) :angle(a), distance(d) {}
	float angle;
	float distance;
};
struct AircraftData
{
	int						hitpoint;
	float					speed;
	TextureID				texture;
	sf::IntRect				textureRect;
	sf::Time				fireInterval;
	std::vector<Direction>  directions;
	bool					hasRollAnimation{ false };
};

struct ProjectileData
{
	int								damage;
	float							speed;
	TextureID						texture;
	sf::IntRect						textureRect;
};

struct PickupData
{
	std::function<void(Aircraft&)>	action;
	TextureID						texture;
	sf::IntRect						textureRect;
};


struct ParticleData
{
	sf::Color						color;
	sf::Time						lifetime;
};

struct ActorData
{
	int									hitpoints;
	int									damageDone;
	float								speed;
	TextureID							texture;
	std::map<Actor::State, Animation2>	animations;
	std::vector<Direction>				directions;

};

/// functions to fill data tables
std::map<Aircraft::Type, AircraftData>		initalizeAircraftData();
std::map<Projectile::Type, ProjectileData>	initializeProjectileData();
std::map<Pickup::Type, PickupData>			initializePickupData();
std::map<Particle::Type, ParticleData>		initializeParticleData();
std::map<Actor::Type, ActorData>			initializeActorData();

