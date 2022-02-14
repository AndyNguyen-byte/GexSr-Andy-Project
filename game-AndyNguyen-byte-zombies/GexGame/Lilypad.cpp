#include "Lilypad.h"

#include "ResourceHolder.h"
#include "Utility.h"
#include "Category.h"
#include "DataTables.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
    const auto TABLE = initializeLilyPadData();
}

Lilypad::Lilypad(const TextureHolder_t& textures)
    : Entity(1)
    , sprite(textures.get(TextureID::Atlas))
{
    sprite.setTextureRect(TABLE.at(State::Empty).textureRect);
    centerOrigin(sprite);
}

void Lilypad::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}

unsigned int Lilypad::getCategory() const
{
    return Category::LilyPad;
}

sf::FloatRect Lilypad::getBoundingRect() const
{
    return  getWorldTransform().transformRect(sprite.getGlobalBounds());
}

bool Lilypad::isLilyPadOccupied()
{
    return isOccupied;
}

void Lilypad::setOccupied(bool x)
{
    isOccupied = x;
}

void Lilypad::addFrog()
{
   sprite.setTextureRect(TABLE.at(State::Frog).textureRect); 
   centerOrigin(sprite);
}
