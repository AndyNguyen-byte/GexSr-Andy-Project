#include "Pillar.h"
#include "ResourceHolder.h"

#include "Utility.h"
#include "Category.h"
#include "DataTables.h"

#include <SFML/Graphics/RenderTarget.hpp>

const float TITLE_HEIGHT = 1024 / 16;
const float TITLE_WIDTH = 1156 / 15;
const float PILLAR_MOVEMENT_SPEED = 200.f;

namespace
{
    const auto TABLE = initalizePillarData();
}

Pillar::Pillar(Type type, const TextureHolder_t& textures)
    :Entity(1)
    , type(type)
    , sprite(textures.get(TABLE.at(type).texture))
{
    centerOrigin(sprite);
    if (type == Type::PillarInvisible) {
        sprite.setColor(sf::Color(0, 0, 0, 0));
    }
}

void Pillar::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
     target.draw(sprite, states);
     //drawBoundingBox(target, states);
}

unsigned int Pillar::getCategory() const
{
    switch (type)
    {
    case Type::PillarInvisible:
        return Category::ScorePillar;
        break;
    default:
        return Category::Pillar;
        break;
    }
}

sf::FloatRect Pillar::getBoundingRect() const
{
    return  getWorldTransform().transformRect(sprite.getGlobalBounds());
}
