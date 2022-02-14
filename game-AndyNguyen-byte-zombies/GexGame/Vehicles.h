#pragma once
#include "Entity.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>


class Vehicles : public Entity
{
public:

	enum class Type
	{
		Truck,
		Tractor,
		LeftRaceCar,
		RightRaceCar,
		Car

	};

	Vehicles(Type type,const TextureHolder_t& textures);

	float					getSpeed();

	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual unsigned int	getCategory() const override;
	

	virtual sf::FloatRect	getBoundingRect() const override;

private:
	Type					type;
	sf::IntRect				textureRect;
	sf::Sprite				sprite;
};

