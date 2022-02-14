#include "RiverEntities.h"
#include "ResourceHolder.h"

#include "Utility.h"
#include "Category.h"
#include "DataTables.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
    const auto TABLE = initializeRiverEntitiesData();
}

RiverEntities::RiverEntities(Type type, const TextureHolder_t& textures)
    :Entity(1)
    ,type(type)
    , sprite(textures.get(TABLE.at(type).texture))
{
    sprite.setTextureRect(TABLE.at(type).textureRect);
    centerOrigin(sprite);
}

float RiverEntities::getSpeed()
{
    return TABLE.at(type).speed;
}


void RiverEntities::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}

unsigned int RiverEntities::getCategory() const
{
    return Category::RiverEntities;
}

sf::FloatRect RiverEntities::getBoundingRect() const
{
    return  getWorldTransform().transformRect(sprite.getGlobalBounds());
}
