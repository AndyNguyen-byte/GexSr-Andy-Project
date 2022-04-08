#pragma once

#include "ResourceIdentifiers.h"
#include "Particle.h"
#include "Animation2.h"
#include "Turtle.h"
#include "Pillar.h"
#include "Sharks.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>
#include <map>



struct Direction
{
	Direction(float a, float d) : angle(a), distance(d) {}
	float angle; 
	float distance;
};


struct ParticleData
{
	sf::Color						color;
	sf::Time						lifetime;
};

struct TurtleData
{
	int						hitpoint;
	TextureID				texture;
	sf::IntRect				textureRect;
	bool					hasFlyAnimation{ false };
};

struct PillarData
{
	sf::Color				color;
	TextureID				texture;
};

struct SharkData
{
	TextureID				texture;
};


/// functions to fill data tables

std::map<Turtle::Type, TurtleData>						initalizeTurtleData();
std::map<Pillar::Type, PillarData>						initalizePillarData();
std::map<Sharks::Type, SharkData>						initalizeSharkData();

std::map<Particle::Type, ParticleData>					initializeParticleData();