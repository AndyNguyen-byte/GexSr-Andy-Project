#include "Ground.h"
#include "ResourceHolder.h"
#include "Utility.h"
#include "Category.h"
#include "DataTables.h"

#include <SFML/Graphics/RenderTarget.hpp>


Ground::Ground(const TextureHolder_t& textures)
	: Entity(1)
	,sprite(textures.get(TextureID::Ground))
{
	centerOrigin(sprite);
}

void Ground::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

unsigned int Ground::getCategory() const
{
	return Category::Ground;
}

sf::FloatRect Ground::getBoundingRect() const
{
	return  getWorldTransform().transformRect(sprite.getGlobalBounds());
}
