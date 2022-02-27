#pragma once
#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Utility.h"
#include <SFML/Graphics/Sprite.hpp>

class Sharks : public Entity
{
public:

	enum class Type
	{
		LeftShark,
		RightShark
	};

	Sharks(Type type, const TextureHolder_t& textures);

	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual unsigned int	getCategory() const override;

	virtual sf::FloatRect	getBoundingRect() const override;

private:
	Type					type;
	sf::Sprite				sprite;
};

