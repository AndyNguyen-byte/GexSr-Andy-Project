#include "Projectile.h"
#include "DataTables.h"
#include "Ultility.h"
#include "ResourceHolder.h"
#include "Category.h"
#include "EmitterNode.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>
#include <iostream>


namespace 
{
    const auto Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder_t& textures)
    :Entity(1)
    ,type(type)
    ,sprite(textures.get(Table.at(type).texture),Table.at(type).textureRect)
    ,targetDirection()
{
    //Add particle system for missiles
    if (isGuided())
    {
        std::unique_ptr<EmitterNode> smoke(new EmitterNode(Particle::Type::Smoke));
        smoke->setPosition(0.f, Projectile::getBoundingRect().height / 2.f);
        attachChild(std::move(smoke));

        std::unique_ptr<EmitterNode> propellant(new EmitterNode(Particle::Type::Propellant));
        propellant->setPosition(0.f, Projectile::getBoundingRect().height / 2.f);
        attachChild(std::move(propellant));
    }

    centerOrigin(sprite);
}

void Projectile::guideTowards(sf::Vector2f position)
{
    assert(isGuided());
    targetDirection = normalizeVector(position - getWorldPosition());
}

bool Projectile::isGuided() const
{
    return type == Type::Missile;
}

unsigned int Projectile::getCategory() const
{
    if (type == Type::EnemyBullet)
        return Category::EnemyProjectile;
    else
        return Category::AlliedProjectile;
}

sf::FloatRect Projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
    return Table.at(type).speed;
}

int Projectile::getDamage() const
{
    return Table.at(type).damage;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    // guide missile
    if (isGuided())
    {
        const float approachRate = 200.f;

        sf::Vector2f newVelocity = normalizeVector(approachRate * dt.asSeconds() * targetDirection + getVelocity());
        newVelocity *= getMaxSpeed();
        float angle = gex::atan2(newVelocity.y, newVelocity.x);


        setRotation(angle + 90.f);
        setVelocity(newVelocity);
    }


    Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}
