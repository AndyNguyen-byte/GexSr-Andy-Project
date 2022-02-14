#pragma once
#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Utility.h"
#include <SFML/Graphics/Sprite.hpp>

class RiverEntities : public Entity
{
public:

	enum class Type
	{
		SmallLog,
		BigLog,
		Croc,
		SmallTurtle,
		BigTurtle,
	};

	RiverEntities(Type type, const TextureHolder_t& textures);

	float					getSpeed();

	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual unsigned int	getCategory() const override;

	virtual sf::FloatRect	getBoundingRect() const override;

private:
	Type					type;
	sf::IntRect				textureRect;
	sf::Sprite				sprite;
};

