#pragma once
#include "Entity.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

class Aircraft;

class Pickup : public Entity
{
public:

	enum class Type
	{
		HealthRefill,
		MissileRefill,
		FireSpread,
		FireRate,
		Count
	};

public:
	Pickup(Type type, const TextureHolder_t& textures);
	~Pickup() = default;

	unsigned int		getCategory() const override;
	sf::FloatRect		getBoundingRect() const override;

	void						apply(Aircraft& player);

private:
	void						drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Sprite					sprite;
	Type						type;
};

