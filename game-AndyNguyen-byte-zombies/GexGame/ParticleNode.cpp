#include "ParticleNode.h"

#include <algorithm>
#include "DataTables.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace
{
	const std::map<Particle::Type, ParticleData> TABLE = initializeParticleData();
}


ParticleNode::ParticleNode(Particle::Type type, const TextureHolder_t& textures)
	: SceneNode()
	, particles()
	, texture(textures.get(TextureID::Particle))
	, type(type)
	, vertexArray(sf::Quads)
	, needsVertexUpdate(true)
{
}

void ParticleNode::addParticle(sf::Vector2f position)
{
	Particle particle;
	particle.position = position;
	particle.color = TABLE.at(type).color;
	particle.lifetime = TABLE.at(type).lifetime;

	particles.push_back(particle);
}

Particle::Type ParticleNode::getParticleType() const
{
    return type;
}

unsigned int ParticleNode::getCategory() const
{
    return Category::ParticleSystem;
}

void ParticleNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Remove expired particles at beginning
	while (!particles.empty() && particles.front().lifetime <= sf::Time::Zero)
		particles.pop_front();

	// Decrease lifetime of existing particles
	for (Particle& p : particles)
		p.lifetime -= dt;

	needsVertexUpdate = true;
}

void ParticleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (needsVertexUpdate)
	{
		computeVertices();
		needsVertexUpdate = false;
	}

	// Apply particle texture
	states.texture = &texture;

	// Draw vertices
	target.draw(vertexArray, states);
}

void ParticleNode::addVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const
{
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(worldX, worldY);
	vertex.texCoords = sf::Vector2f(texCoordX, texCoordY);
	vertex.color = color;

	vertexArray.append(vertex);
}

void ParticleNode::computeVertices() const
{
	sf::Vector2f size(texture.getSize());
	sf::Vector2f half = size / 2.f;

	// Refill vertex array
	vertexArray.clear();
	for (const Particle& p : particles)
	{
		sf::Vector2f pos = p.position;
		sf::Color color = p.color;

		float ratio = p.lifetime.asSeconds() / TABLE.at(type).lifetime.asSeconds();
		color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));

		addVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, color);
		addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, color);
		addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
		addVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, color);
	}
}
