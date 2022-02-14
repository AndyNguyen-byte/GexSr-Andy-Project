#include "Vehicles.h"
#include "ResourceHolder.h"

#include "Utility.h"
#include "Category.h"
#include "DataTables.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
    const auto TABLE = initializeVehicleData();
}

Vehicles::Vehicles(Type type,const TextureHolder_t& textures)
    : Entity(1)
    , type(type)
    , sprite(textures.get(TABLE.at(type).texture))
{
    sprite.setTextureRect(TABLE.at(type).textureRect);
    centerOrigin(sprite);
}

void Vehicles::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}

unsigned int Vehicles::getCategory() const
{
    return Category::Vehicle;
}

float Vehicles::getSpeed()
{
    return TABLE.at(type).speed;
}

sf::FloatRect Vehicles::getBoundingRect() const
{
    return  getWorldTransform().transformRect(sprite.getGlobalBounds());
}
