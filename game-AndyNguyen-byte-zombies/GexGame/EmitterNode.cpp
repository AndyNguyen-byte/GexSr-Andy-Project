#include "EmitterNode.h"
#include "Command.h"
#include "CommandQueue.h"
#include "ParticleNode.h"

EmitterNode::EmitterNode(Particle::Type type)
	: SceneNode()
	, accumulatedTime(sf::Time::Zero)
	, type(type)
	, particleSystem(nullptr)
{}

void EmitterNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (particleSystem)
	{
		emitParticles(dt);
	}
	else
	{
		// Find particle node with the same type as emitter node
		auto finder = [this](ParticleNode& container, sf::Time)
		{
			if (container.getParticleType() == type)
				particleSystem = &container;
		};

		Command command;
		command.category = Category::ParticleSystem;
		command.action = derivedAction<ParticleNode>(finder);

		commands.push(command);
	}
}

void EmitterNode::emitParticles(sf::Time dt)
{
	const float emissionRate = 30.f;
	const sf::Time interval = sf::seconds(1.f) / emissionRate;

	accumulatedTime += dt;

	while (accumulatedTime > interval)
	{
		accumulatedTime -= interval;
		particleSystem->addParticle(getWorldPosition());
	}
}
