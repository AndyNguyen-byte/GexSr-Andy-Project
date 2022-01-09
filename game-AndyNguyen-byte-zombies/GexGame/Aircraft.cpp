#include "Aircraft.h"
#include "ResourceHolder.h"
#include "Ultility.h"
#include "Category.h"
#include "TextNode.h"
#include "DataTables.h"
#include "SoundNode.h"

#include <memory>
#include <stdexcept>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace {
	const std::map<Aircraft::Type, AircraftData> Table = initalizeAircraftData();
}

Aircraft::Aircraft(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts)
	:Entity(Table.at(type).hitpoint)
	,type(type)
	,sprite(textures.get(Table.at(type).texture),Table.at(type).textureRect)
	,explosion(textures.get(TextureID::Explosion))
	,showExplosion(true)
	,hasSpawnedPickup(false)
	,missileAmmo(5)
	,fireRateLevel(1)
	,spreadLevel(1)
	,directionIndex(0)
	,travelledDistance(0)
	,fireCommand()
	,missileCommand()
	,dropPickupCommand()
	,fireCountDown(sf::Time::Zero)
	,isFiring(false)
	,isLaunchingMissile(false)
	,_isMarkedForRemoval(false)
	,hasPlayedExplosionSound(false)
	,healthDisplay(nullptr)
	,missileDisplay(nullptr)
{
	explosion.setFrameSize(sf::Vector2i(256, 256));
	explosion.setNumFrames(16);
	explosion.setDuration(sf::seconds(1));
	centerOrigin(explosion);

	centerOrigin(sprite);

	fireCommand.category = Category::SceneAirLayer;
	fireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		this->createBullets(node, textures);
	};

	missileCommand.category = Category::SceneAirLayer;
	missileCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		this->createProjectile(node, Projectile::Type::Missile, 0.f, 0.5f, textures);
	};

	dropPickupCommand.category = Category::SceneAirLayer;
	dropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		this->createPickUp(node,textures);
	};

	std::unique_ptr<TextNode> health(new TextNode(fonts, ""));
	healthDisplay = health.get(); ///< raw pointer to healthDisplay node
	attachChild(std::move(health));

	if (getCategory() == Category::PlayerAircraft)
	{
		std::unique_ptr<TextNode> missiles(new TextNode(fonts, ""));
		missiles->setPosition(0, 70);
		missileDisplay = missiles.get();
		attachChild(std::move(missiles));
	}

	updateTexts();
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && showExplosion)
		target.draw(explosion, states);
	else
		target.draw(sprite, states);	
}

unsigned int Aircraft::getCategory() const
{
	switch (type)
	{
	case Type::Eagle:
		return Category::PlayerAircraft;
	/*case Type::Raptor:
		return Category::AlliedAircraft;*/
	default:
		return Category::EnemyAircraft;
	}
}

void Aircraft::fire()
{
	if (Table.at(type).fireInterval != sf::Time::Zero)
	{
		isFiring = true;
	}
}

void Aircraft::launchMissile()
{
	if (missileAmmo > 0)
	{
		isLaunchingMissile = true;
		--missileAmmo;
	}
}

bool Aircraft::isAllied() const
{
	return type == Type::Eagle;
}

void Aircraft::playLocalSound(CommandQueue& commands, SoundEffectID effect)
{
	Command playSoundCommand;
	auto pos = getWorldPosition();
	playSoundCommand.category = Category::SoundEffect;
	playSoundCommand.action = derivedAction<SoundNode>([pos, effect](SoundNode& node, sf::Time) 
		{
			node.playSound(effect, pos);
		});

	commands.push(playSoundCommand);
}

void Aircraft::increaseFireRate()
{
	if (fireRateLevel < 10)
		++fireRateLevel;
}

void Aircraft::increaseFireSpread()
{
	if (spreadLevel < 3)
		++spreadLevel;
}

void Aircraft::collectMissiles(unsigned int count)
{
	missileAmmo += count;
}

sf::FloatRect Aircraft::getBoundingRect() const
{
	return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

void Aircraft::remove()
{
	Entity::remove();
	showExplosion = false;
}

bool Aircraft::isMarkedForRemoval() const
{
	if (isAllied())
		return false;
	else
		return _isMarkedForRemoval;
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{

	updateRollAnimation();
	updateTexts();

	//check if aircraft has been destroyed
	if (isDestroyed())
	{
		checkPickUpDrop(commands);
		explosion.update(dt);

		if (!hasPlayedExplosionSound)
		{
			hasPlayedExplosionSound = true;
			SoundEffectID effect = (randomInt(2) == 0 ? SoundEffectID::Explosion1 : SoundEffectID::Explosion2);
			playLocalSound(commands, effect);
		}

		return;
	}
	
	//check if bullets needs to be fired
	checkProjectileLaunch(dt, commands);

	updateMovementPattern(dt); //follow autopilot

	Entity::updateCurrent(dt, commands);
}

void Aircraft::updateRollAnimation()
{
	//update roll animation
	if (Table.at(type).hasRollAnimation)
	{
		sf::IntRect textureRect = Table.at(type).textureRect;

		//Roll left: Texture Rect offset once
		if (getVelocity().x < 0.f)
			textureRect.left += textureRect.width;

		//Roll left: Texture Rect offset twice
		else if (getVelocity().x > 0.f)
			textureRect.left += 2 * textureRect.width;

		sprite.setTextureRect(textureRect);
	}
}

void Aircraft::updateTexts()
{
	healthDisplay->setText(std::to_string(getHitPoints()) + " HP");
	healthDisplay->setPosition(0.f, 50.f);
	healthDisplay->setRotation(-getRotation());

	if (missileDisplay)
	{
		if (missileAmmo == 0)
			missileDisplay->setText("");
		else
			missileDisplay->setText("M: " + std::to_string(missileAmmo));
	}
}

void Aircraft::updateMovementPattern(sf::Time dt)
{
	const std::vector<Direction>& directions = Table.at(type).directions;

	if (!directions.empty())
	{
		float distanceToTravel = directions.at(directionIndex).distance;

		if (travelledDistance > distanceToTravel)
		{
			// go to next leg
			directionIndex = ++directionIndex % directions.size();
			travelledDistance = 0;
		}
		travelledDistance += Table.at(type).speed * dt.asSeconds();

		//set velocity from autopilot
		float dirAngle = directions[directionIndex].angle + 90.f;
		float vx = Table.at(type).speed * gex::cos(dirAngle);
		float vy = Table.at(type).speed * gex::sin(dirAngle);
		setVelocity(vx, vy);
	}
}

void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	//eneymy try to fire all the time
	if (!isAllied())
		fire();


	//check for bullet fire
	if (isFiring && fireCountDown <= sf::Time::Zero) 
	{
		//Interval Expired push new fire command
		commands.push(fireCommand);
		playLocalSound(commands, isAllied() ? SoundEffectID::AlliedGunfire : SoundEffectID::EnemyGunfire);
		fireCountDown += Table.at(type).fireInterval / (fireRateLevel + 1.f);
	}
	else if (fireCountDown > sf::Time::Zero)
	{
		//Interval not expired: Decrease it further
		fireCountDown -= dt;
	}

	//check for missile launch
	if (isLaunchingMissile)
	{
		commands.push(missileCommand);
		playLocalSound(commands, SoundEffectID::LaunchMissile);
	}

	isLaunchingMissile = false;
	isFiring = false;

}

void Aircraft::checkPickUpDrop(CommandQueue& commands)
{
	if (!isAllied() && randomInt(2) == 0 && !hasSpawnedPickup)
		commands.push(dropPickupCommand);
	
	hasSpawnedPickup = true;
}

void Aircraft::createBullets(SceneNode& node, const TextureHolder_t& textures) const
{
	Projectile::Type type = isAllied() ? Projectile::Type::AlliedBullet : Projectile::Type::EnemyBullet;

	switch (spreadLevel)
	{
	case 1:
		createProjectile(node, type, 0.f, 0.5f, textures);
		break;
	case 2:
		createProjectile(node, type, -0.33f, 0.33f, textures);
		createProjectile(node, type, +0.33f, 0.33f, textures);
		break;
	case 3:
		createProjectile(node, type, -0.5f, 0.33f, textures);
		createProjectile(node, type, 0.f, 0.5f, textures);
		createProjectile(node, type, 0.5f, 0.33f, textures);
		break;
	}
}

void Aircraft::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder_t& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(xOffset * sprite.getGlobalBounds().width, yOffset * sprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	float sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(std::move(projectile));

}

void Aircraft::createPickUp(SceneNode& node, const TextureHolder_t& textures) const
{
	auto type = static_cast<Pickup::Type>(randomInt(static_cast<int>(Pickup::Type::Count)));

	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	pickup->setPosition(getWorldPosition());
	pickup->setVelocity(0.f, 1.f);
	node.attachChild(std::move(pickup));
}
