#include "DataTables.h"
#include "jsonFrameParser.h"
#include "Animation2.h"


std::map<Turtle::Type, TurtleData> initalizeTurtleData()
{
	std::map<Turtle::Type,TurtleData> data;

	// Turtle Data
	data[Turtle::Type::NormalTurtle].hitpoint = 1;
	data[Turtle::Type::NormalTurtle].texture = TextureID::Turtle1;
	data[Turtle::Type::NormalTurtle].hasFlyAnimation = true;


	return data;
}

std::map<Pillar::Type, PillarData> initalizePillarData()
{
	std::map<Pillar::Type, PillarData> data;

	data[Pillar::Type::PillarDown].texture = TextureID::PillarDown1;

	data[Pillar::Type::PillarUp].texture = TextureID::PillarUp1;

	data[Pillar::Type::PillarInvisible].texture = TextureID::PillarDown1;
	data[Pillar::Type::PillarInvisible].color = sf::Color(0,0,0,0);

	return data;
}

std::map<Sharks::Type, SharkData> initalizeSharkData()
{
	std::map<Sharks::Type, SharkData> data;

	data[Sharks::Type::LeftShark].texture = TextureID::SharkLeft;
	data[Sharks::Type::RightShark].texture = TextureID::SharkRight;

	return data;
}

std::map<Particle::Type, ParticleData> initializeParticleData()
{
    std::map<Particle::Type, ParticleData> data;

    data[Particle::Type::Propellant].color = sf::Color(255, 255, 50);
    data[Particle::Type::Propellant].lifetime = sf::seconds(0.6f);

    data[Particle::Type::Smoke].color = sf::Color(50, 50, 50);
    data[Particle::Type::Smoke].lifetime = sf::seconds(4.f);

    return data;
}