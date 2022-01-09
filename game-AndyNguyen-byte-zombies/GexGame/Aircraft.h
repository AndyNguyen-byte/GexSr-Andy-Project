#pragma once

#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "TextNode.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Projectile.h"
#include "Animation.h"

#include "SFML/Graphics/Sprite.hpp"


class Aircraft : public Entity
{
public:
	enum class Type 
	{
		Eagle,
		Avenger,
		Raptor,
	};
public:
	Aircraft(Type type, const TextureHolder_t& textures,const FontHolder_t& fonts);

	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual unsigned int	getCategory() const override;

	void					fire();
	void					launchMissile();
	bool					isAllied() const;
	void					playLocalSound(CommandQueue& commands, SoundEffectID effect);

	void					increaseFireRate();
	void					increaseFireSpread();
	void					collectMissiles(unsigned int count);

	virtual sf::FloatRect	getBoundingRect() const;

	void					remove() override;
	bool					isMarkedForRemoval() const override;
	
private:
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands) override;
	void					updateRollAnimation();
	void					updateTexts();


	void					updateMovementPattern(sf::Time dt);

	void					checkProjectileLaunch(sf::Time dt,CommandQueue& commands);
	void					checkPickUpDrop(CommandQueue& commands);

	void					createBullets(SceneNode& node, const TextureHolder_t& textures) const;
	void					createProjectile(SceneNode& node, Projectile::Type type,
												float xOffset, float yOffset,
												const TextureHolder_t& textures) const;
	void					createPickUp(SceneNode& node, const TextureHolder_t& textures) const;
private:
	Type					type;
	sf::Sprite				sprite;

	Animation				explosion;
	bool					showExplosion;
	bool					hasSpawnedPickup;

	int						missileAmmo;
	int						fireRateLevel;
	int						spreadLevel;

	std::size_t				directionIndex;
	float					travelledDistance;

	Command					fireCommand;
	Command					missileCommand;
	Command					dropPickupCommand;
	sf::Time				fireCountDown;

	bool					isFiring;
	bool					isLaunchingMissile;
	bool					_isMarkedForRemoval;
	bool					hasPlayedExplosionSound;

	TextNode*				healthDisplay;
	TextNode*				missileDisplay;
};

