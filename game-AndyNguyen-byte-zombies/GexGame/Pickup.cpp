#include "Pickup.h"
#include "DataTables.h"
#include "Ultility.h"

#include <SFML/Graphics/RenderTarget.hpp>


namespace
{
	const std::map<Pickup::Type, PickupData> Table = initializePickupData();
}

Pickup::Pickup(Type type, const TextureHolder_t& textures)
	:Entity(1)
	,type(type)
	,sprite(textures.get(Table.at(type).texture), Table.at(type).textureRect)
{
	centerOrigin(sprite);
}

unsigned int Pickup::getCategory() const
{
	return Category::Pickup;
}

sf::FloatRect Pickup::getBoundingRect() const
{
	return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

void Pickup::apply(Aircraft& player)
{
	Table.at(type).action(player);
}

void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}
