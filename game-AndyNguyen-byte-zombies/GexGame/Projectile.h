#pragma once

#include "Entity.h"
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>


class Projectile : public Entity
{
public:

	enum class Type
	{
		AlliedBullet,
		EnemyBullet,
		Missile, 
	};

public:
							Projectile(Type type, const TextureHolder_t& textures);

	void					guideTowards(sf::Vector2f position);
	bool					isGuided() const;


	virtual unsigned int	getCategory() const override;
	virtual sf::FloatRect	getBoundingRect() const override;

	float					getMaxSpeed() const;
	int						getDamage() const;


private:
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	Type					type;
	sf::Sprite				sprite;
	sf::Vector2f			targetDirection;

};

