#pragma once


#include "Entity.h"
#include "ResourceIdentifiers.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Projectile.h"
#include "Animation.h"

#include <SFML/Graphics/Sprite.hpp>

class TextNode;

class Aircraft : public Entity
{
public:
	enum class Type
	{
		Eagle,
		Raptor,
		Avenger
	};


public:
	Aircraft(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts);

	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual unsigned int	getCategory() const override;

	void 					fire();
	void					launchMissile();
	bool					isAllied() const;
	void					playLocalSound(CommandQueue& commands, SoundEffectID effect);


	void					increaseFireRate();
	void					increaseFireSpread();
	void					collectMissiles(unsigned int count);

	virtual sf::FloatRect	getBoundingRect() const override;

	 
	virtual bool			isMarkedForRemoval() const override;
	virtual void			remove() override;


private: 
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands) override;
	void					updateRollAnimation();
	void					updateTexts();

	void					updateMovementPattern(sf::Time dt);

	void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
	void					checkPickupDrop(CommandQueue& commands);

	void					createBullets(SceneNode& node, const TextureHolder_t& textures) const;
	void					createProjectile(SceneNode& node, Projectile::Type type,
												float xOffset, float yOffset,
												const TextureHolder_t& textures) const;
	void					createPickup(SceneNode& node, const TextureHolder_t& textures) const;

private:
	Type					type;
	sf::Sprite				sprite;

	Animation				explosion;
	bool					showExplosion;
	bool					hasSpawnedPickup;

	int						missileAmmo;
	int						fireRateLevel;
	int						spreadLevel;

	size_t					directionIndex;
	float					travelledDistance;

	Command 				fireCommand;
	Command					missileCommand;
	Command					dropPickupCommand;

	sf::Time				fireCountdown;

	bool 					isFiring;
	bool					isLaunchingMissile;
	bool					_isMarkedForRemoval;
	bool					hasPlayedExplosionSound;

	TextNode*				healthDisplay;
	TextNode*				missileDisplay;
};