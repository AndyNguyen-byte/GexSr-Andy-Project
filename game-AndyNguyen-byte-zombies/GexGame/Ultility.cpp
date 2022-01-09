#include "Ultility.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cassert>
#include <random>
#include <cmath>

namespace
{
    std::default_random_engine createRandomEngine()
    {
        auto seed = static_cast<unsigned long>(std::time(nullptr));
        return std::default_random_engine(seed);
    }

    auto RandomEngine = createRandomEngine();
}

int randomInt(int exclusiveMax)
{
    std::uniform_int_distribution<>distr(0, exclusiveMax - 1);
    return distr(RandomEngine);
}

void centerOrigin(sf::Sprite& sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    /*sprite.setOrigin(std::floor(bounds.left+bounds.width/2.f)
                    ,std::floor(bounds.top+bounds.height/2.f));*/
}

void centerOrigin(sf::Text& text)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    /*text.setOrigin(std::floor(bounds.left + bounds.width / 2.f)
        , std::floor(bounds.top + bounds.height / 2.f));*/
}

void centerOrigin(Animation& animation)
{
    sf::FloatRect bounds = animation.getLocalBounds();
    animation.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
}


float length(sf::Vector2f v)
{
    return std::sqrt(v.x*v.x+v.y*v.y);
}

sf::Vector2f normalizeVector(sf::Vector2f v)
{
    static const float epsilon = 0.0001f;
    if (length(v) > epsilon)
        v /= length(v);

    return v;

}

sf::IntRect flip(const sf::IntRect& rec)
{
    auto temp = rec;
    temp.left += temp.width;
    temp.width *= -1;
    return temp;
}