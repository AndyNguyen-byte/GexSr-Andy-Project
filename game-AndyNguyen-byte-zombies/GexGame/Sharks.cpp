#include "Sharks.h"
#include "ResourceHolder.h"

#include "Utility.h"
#include "Category.h"
#include "DataTables.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
    const auto TABLE = initalizeSharkData();
}

Sharks::Sharks(Type type,const TextureHolder_t& textures)
    :Entity(1)
    ,sprite(textures.get(TABLE.at(type).texture))
    ,type(type)
{
    centerOrigin(sprite);
}

void Sharks::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
    drawBoundingBox(target, states);
}

unsigned int Sharks::getCategory() const
{
    return Category::Shark;
}

sf::FloatRect Sharks::getBoundingRect() const
{
    return  getWorldTransform().transformRect(sprite.getGlobalBounds());
}
