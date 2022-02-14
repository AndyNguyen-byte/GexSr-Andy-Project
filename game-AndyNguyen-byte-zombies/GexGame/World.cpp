#include "World.h"
#include "Utility.h"
#include "Pickup.h"
#include "Projectile.h"
#include "ParticleNode.h"
#include "Lilypad.h"
#include "SoundNode.h"

#include <algorithm>
#include <SFML/Graphics/RenderWindow.hpp>

const float TITLE_HEIGHT = 1024 / 16;
const float TITLE_WIDTH = 578 / 15;


World::World(sf::RenderTarget& outputTarget, FontHolder_t& fonts, SoundPlayer& sounds)
	: target(outputTarget)
	, worldView(outputTarget.getDefaultView())
	, textures()
	, sounds(sounds)
	, fonts(fonts)
	, sceneGraph()
	, sceneLayers()
	, commands()
	, worldBounds(0.f, 0.f, worldView.getSize().x , worldView.getSize().y)
	, spawnPosition(worldView.getSize().x /2.f,
		worldBounds.height - worldView.getSize().y/2.f)
	, scrollSpeed(-50.f)
	, playerAircraft(nullptr)
{
	sceneTexture.create(target.getSize().x, target.getSize().y);

	loadTextures();
	buildScene();

	worldView.setCenter(spawnPosition);
}

void World::update(sf::Time dt)
{
	// sroll the world
	// worldView.move(0.f, scrollSpeed * dt.asSeconds());
	playerAircraft->setVelocity(0.f, 0.f);

	resetGroundPos();

	destroyEntitiesOutOfView();
	guideMissiles();

	// apply all command
	while (!commands.isEmpty())
		sceneGraph.onCommand(commands.pop(), dt);
	
	adaptPlayerVelocity();

	checkFrogPos();
	handleCollisions();
	if (isDrowning) {
		killFrog();
	}
	if (!isInBounds) {
		killFrog();
	}
	
	sceneGraph.removeWrecks();
	spawnEnemies();

	sceneGraph.update(dt, commands); 
	adaptPlayerPosition(); 

	updateSounds();
}

void World::updateSounds()
{
	sounds.setListnerPosition(playerAircraft->getWorldPosition());
	sounds.removeStoppedSounds();
}

void World::draw()
{
	if (PostEffect::isSupported())
	{
		sceneTexture.clear();
		sceneTexture.setView(worldView);
		sceneTexture.draw(sceneGraph);
		sceneTexture.display();
		bloomEffect.apply(sceneTexture, target);
	}
	else
	{
		target.setView(worldView);
		target.draw(sceneGraph);
	}
	 
}

CommandQueue& World::getCommandQueue()
{
	return commands;
}

bool World::hasAlivePlayer() const
{
	return !frogLives<=0;
}

bool World::hasPlayerReachedEnd() const
{
	return numberOfLilypadsOccupied==5;
}

void World::loadTextures()
{
	textures.load(TextureID::Entities, "../Media/Textures/Entities.png"); 
	
	textures.load(TextureID::Particle, "../Media/Textures/Particle.png");
	textures.load(TextureID::FinishLine, "../Media/Textures/FinishLine.png");
	textures.load(TextureID::Explosion, "../Media/Textures/Explosion.png");

	textures.load(TextureID::Atlas, "../Media/Textures/Atlas.png");
	textures.load(TextureID::Marsh, "../Media/Textures/BackGround1.png");

	textures.load(TextureID::MissisleRefill, "../Media/Textures/MissileRefill.png");
	textures.load(TextureID::Ground, "../Media/Textures/Ground.png");

	/*textures.load(TextureID::Zombie1, "../Media/Textures/Zombie1.png");
	textures.load(TextureID::Zombie2, "../Media/Textures/Zombie2.png");
	textures.load(TextureID::Zombie3, "../Media/Textures/Zombie3.png");
	textures.load(TextureID::Zombie4, "../Media/Textures/Zombie4.png");
	textures.load(TextureID::Zombie5, "../Media/Textures/Zombie5.png");
	textures.load(TextureID::Hero2, "../Media/Textures/hero2.png");
	textures.load(TextureID::Road, "../Media/Textures/Road.png");*/

}

void World::buildScene()
{
	// set up layer nodes
	for (std::size_t i = 0; i < LayerCount; ++i) {

		Category::type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		sceneLayers[i] = layer.get();
		sceneGraph.attachChild(std::move(layer));
	}

	// sound effects 
	std::unique_ptr<SoundNode> sNode(new SoundNode(sounds));
	sceneGraph.attachChild(std::move(sNode));

	// prepare tiled background 
	sf::Texture& texture = textures.get(TextureID::Marsh);
	texture.setRepeated(true);

	// background
	sf::IntRect  textureRect(worldBounds);
	auto backgroundSprite =  std::make_unique<SpriteNode>(texture, textureRect);
	backgroundSprite->setPosition(worldBounds.left, worldBounds.top);
	sceneLayers[Background]->attachChild(std::move(backgroundSprite));


	//// Add smoke particle node to the scene
	//std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(Particle::Type::Smoke, textures));
	//sceneLayers[LowerAir]->attachChild(std::move(smokeNode));

	//// Add propellant particle node to the scene
	//std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(Particle::Type::Propellant, textures));
	//sceneLayers[LowerAir]->attachChild(std::move(propellantNode));


	// Add player's frog
	auto leader = std::make_unique<Frog>(Frog::Type::PlayerFrog, textures, fonts);
	playerAircraft = leader.get();
	playerAircraft->setPosition(spawnPosition.x,spawnPosition.y + worldView.getSize().y / 2.f);
	playerAircraft->setVelocity(40.f, scrollSpeed);
	sceneLayers[UpperAir]->attachChild(std::move(leader));

	//Add Lily Pads
	buildLilyPad();

	auto Ground1 = std::make_unique<Ground>(textures);
	ground1 = Ground1.get();
	ground1->setPosition(TITLE_WIDTH * 8.75, TITLE_HEIGHT * 15);
	ground1->setVelocity(-3 * TITLE_WIDTH, 0);
	sceneLayers[LowerAir]->attachChild(std::move(Ground1));

	auto Ground2 = std::make_unique<Ground>(textures);
	ground2 = Ground2.get();
	ground2->setPosition(TITLE_WIDTH * 20, TITLE_HEIGHT * 15);
	ground2->setVelocity(-3 * TITLE_WIDTH, 0);
	sceneLayers[LowerAir]->attachChild(std::move(Ground2));
}

void World::makeLilyPad(float x)
{
	auto lilypad = std::make_unique<Lilypad>(textures);
	lilypad->setPosition(TITLE_WIDTH * x, TITLE_HEIGHT * 2.5);
	sceneLayers[LowerAir]->attachChild(std::move(lilypad));
}

void World::buildLilyPad()
{
	// add lilypad
	makeLilyPad(1.05);
	makeLilyPad(4);
	makeLilyPad(7);
	makeLilyPad(10);
	makeLilyPad(13);
}

void World::resetGroundPos()
{
	if (ground1->getPosition().x < TITLE_WIDTH * -5)
	{
		ground1->setPosition(TITLE_WIDTH * 20, TITLE_HEIGHT * 15);
	}

	if (ground2->getPosition().x < TITLE_WIDTH * -5)
	{
		ground2->setPosition(TITLE_WIDTH * 20, TITLE_HEIGHT * 15);
	}
}


void World::makeVehicle(Vehicles::Type type, float x, float y)
{
	auto car = std::make_unique<Vehicles>(type, textures);
	car->setPosition(TITLE_WIDTH * x, TITLE_HEIGHT * y);
	car->setVelocity(car->getSpeed() * TITLE_WIDTH, 0);
	sceneLayers[LowerAir]->attachChild(std::move(car));
}

void World::makeRiverEntities(RiverEntities::Type type, float x, float y)
{
	auto river = std::make_unique<RiverEntities>(type, textures);
	river->setPosition(TITLE_WIDTH * x, TITLE_HEIGHT * y);
	river->setVelocity(river->getSpeed() * TITLE_WIDTH, 0);
	sceneLayers[LowerAir]->attachChild(std::move(river));
}

void World::checkFrogPos()
{
	isDrowning = playerAircraft->isOnRiver();
	isInBounds = playerAircraft->isInBounds();
}

void World::killFrog()
{
	frogLives -= 1;
	playerAircraft->setDeathStatus(true);
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds(worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize());
	
	const float borderDistance = 20.f;

	sf::Vector2f position = playerAircraft->getPosition();

	//position.x = std::max(position.x, viewBounds.left + borderDistance);
	//position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);

	playerAircraft->setPosition(position);

}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = playerAircraft->getVelocity();

	if (velocity.x != 0.f && velocity.y != 0.f)
		playerAircraft->setVelocity(velocity / std::sqrt(2.f));

	playerAircraft->accelerate(0.f, 0.f);

}

void World::spawnEnemies()
{
	while (!enemySpawnPoints.empty() && 
			enemySpawnPoints.back().y > getBattlefield().top )
		
	{
		auto spawn = enemySpawnPoints.back();
		std::unique_ptr<Actor> enemy(new Actor(spawn.type, textures, fonts));
		enemy->setPosition(spawn.x, spawn.y);
		sceneLayers[UpperAir]->attachChild(std::move(enemy));
		enemySpawnPoints.pop_back();
	}

 }

void World::addEnemies()
{
	

	addEnemy(Actor::Type::Zombie1, -250.f, 200.f);
	addEnemy(Actor::Type::Zombie1, 0.f, 200.f);
	addEnemy(Actor::Type::Zombie1, 250.f, 200.f);
	addEnemy(Actor::Type::Zombie2, -250.f, 600.f);
	addEnemy(Actor::Type::Zombie3, 0.f, 600.f);
	addEnemy(Actor::Type::Zombie2, 250.f, 600.f);
	addEnemy(Actor::Type::Zombie3, -70.f, 400.f);
	addEnemy(Actor::Type::Zombie1, 70.f, 400.f);
	addEnemy(Actor::Type::Zombie3, -70.f, 800.f);
	addEnemy(Actor::Type::Zombie4, 70.f, 800.f);
	addEnemy(Actor::Type::Zombie5, -170.f, 850.f);
	addEnemy(Actor::Type::Zombie4, 170.f, 850.f);

	std::sort(enemySpawnPoints.begin(), enemySpawnPoints.end(),
		[](SpawnPoint lhs, SpawnPoint rhs)
		{
			return lhs.y < rhs.y;
		});
}

void World::addEnemy(Actor::Type type, float relX, float relY)
{
	SpawnPoint spawn(type, spawnPosition.x + relX, spawnPosition.y - relY);
	enemySpawnPoints.push_back(spawn);
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(worldView.getCenter() - worldView.getSize() / 2.f , worldView.getSize() );
}

sf::FloatRect World::getBattlefield() const
{
	sf::FloatRect bounds = getViewBounds();
	bounds.left -= 100.f;
	bounds.width += 600.f;

	return bounds;
}

void World::guideMissiles()
{
	// build a list of active Enemies
	Command enemyCollector;
	enemyCollector.category = Category::EnemyAircraft;
	enemyCollector.action = derivedAction<Actor>([this](Actor& enemy, sf::Time dt)
		{
			if (!enemy.isDestroyed())
				activeEnemies.push_back(&enemy);
		});

	Command missileGuider;
	missileGuider.category = Category::type::AlliedProjectile;
	missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time dt)
		{
			// ignore bullets
			if (!missile.isGuided())
				return;

		 	float minDistance = std::numeric_limits<float>::max();
			Actor* closestEnemy = nullptr;

			for (Actor* e : activeEnemies)
			{
				auto d = distance(missile, *e);
				if (d < minDistance)
				{
					minDistance = d;
					closestEnemy = e;
				}
			}

			if (closestEnemy)
				missile.guideTowards(closestEnemy->getWorldPosition());
		});

	commands.push(enemyCollector);
	commands.push(missileGuider);

	activeEnemies.clear();

}


bool matchesCategories(SceneNode::Pair& colliders, Category::type type1, Category::type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	sceneGraph.checkSceneCollision(sceneGraph, collisionPairs);

	//check frog y pos for river if not on log kill on log not kill

	for (auto pair : collisionPairs)
	{
		if (matchesCategories(pair, Category::type::PlayerFrog, Category::type::LilyPad))
		{
			auto& frog = static_cast<Frog&>(*(pair.first));
			auto& lilypad = static_cast<Lilypad&>(*(pair.second));

			if (!lilypad.isLilyPadOccupied())
			{
				numberOfLilypadsOccupied++;
				lilypad.addFrog();
				lilypad.setOccupied(true);
				frog.setPosition(TITLE_WIDTH * 7, TITLE_HEIGHT * 15);
				isDrowning = false;
			}
			else {
				killFrog();
			}
		}
		if (matchesCategories(pair, Category::type::PlayerFrog, Category::type::Vehicle))
		{
			auto& frog = static_cast<Frog&>(*(pair.first));
			auto& vehicle = static_cast<Vehicles&>(*(pair.second));

			killFrog();
		}
		if (matchesCategories(pair, Category::type::PlayerFrog, Category::type::RiverEntities))
		{
			auto& frog = static_cast<Frog&>(*(pair.first));
			auto& riverEntity = static_cast<RiverEntities&>(*(pair.second));

			isDrowning = false;

			frog.setVelocity(riverEntity.getVelocity());
		}
	}
}

void World::destroyEntitiesOutOfView()
{
	auto battlefield = getBattlefield();

	Command command;
	command.category = Category::SpaceJunk;

	command.action = derivedAction<Entity>([this, battlefield](Entity& e, sf::Time dt)
		{
			if (!battlefield.intersects(e.getBoundingRect()))
				e.remove();
		});

	commands.push(command);
}
