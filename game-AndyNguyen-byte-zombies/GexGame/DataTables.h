#pragma once

#include "ResourceIdentifiers.h"
#include "Aircraft.h"
#include "Projectile.h"
#include "Pickup.h"
#include "Particle.h"
#include "Actor.h"
#include "Animation2.h"
#include "Turtle.h"
#include "Lilypad.h"
#include "Vehicles.h"
#include"RiverEntities.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>
#include <map>
#include "Frog.h"


struct Direction
{
	Direction(float a, float d) : angle(a), distance(d) {}
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
	std::vector<Direction>	directions;
	bool					hasRollAnimation{false};
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

struct FrogData
{
	int									hitPoints;
	TextureID							texture;
	std::map<Frog::Direction, Animation2>	animations;
	Animation2							deathAnimation;
	std::vector<Direction>				directions;
};

struct TurtleData
{
	int						hitpoint;
	TextureID				texture;
	sf::IntRect				textureRect;
	bool					hasFlyAnimation{ false };
};

struct LilyPadData
{
	TextureID							texture;
	sf::IntRect							textureRect;
};

struct VehicleData
{
	TextureID							texture;
	sf::IntRect							textureRect;
	float 								speed;
};

struct RiverEntitiesData
{
	TextureID							texture;
	sf::IntRect							textureRect;
	float								speed;
	Animation2							animations;
};

/// functions to fill data tables
std::map<Actor::Type, ActorData>						initializeActorData();

std::map<Frog::Type, FrogData>							initializeFrogData();
std::map<Lilypad::State, LilyPadData>					initializeLilyPadData();
std::map<Vehicles::Type, VehicleData>					initializeVehicleData();
std::map<RiverEntities::Type, RiverEntitiesData>		initializeRiverEntitiesData();

std::map<Turtle::Type, TurtleData>						initalizeTurtleData();

std::map<Aircraft::Type, AircraftData>					initalizeAircraftData();
std::map<Projectile::Type, ProjectileData>				initializeProjectileData();
std::map<Pickup::Type, PickupData>						initializePickupData();
std::map<Particle::Type, ParticleData>					initializeParticleData();