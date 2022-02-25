#include "World.h"
#include "Utility.h"
#include "Pickup.h"
#include "Projectile.h"
#include "ParticleNode.h"
#include "Lilypad.h"
#include "SoundNode.h"

#include <algorithm>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Pillar.h"
#include "PillarGroup.h"

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

	resetGroundPos();

	destroyEntitiesOutOfView();

	// apply all command
	while (!commands.isEmpty())
		sceneGraph.onCommand(commands.pop(), dt);
	
	adaptPlayerVelocity();

	handleCollisions();

	sceneGraph.removeWrecks();
	spawnPillars(dt);

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
	if (false && PostEffect::isSupported())
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
	textures.load(TextureID::Turtle1, "../Media/Textures/Turtle1.png");
	textures.load(TextureID::PillarUp1, "../Media/Textures/PillarUp.png");
	textures.load(TextureID::PillarDown1, "../Media/Textures/PillarDown.png");

	textures.load(TextureID::MissisleRefill, "../Media/Textures/MissileRefill.png");
	textures.load(TextureID::Ground, "../Media/Textures/Ground.png");

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


	// Add player's turtle
	auto leader = std::make_unique<Turtle>(Turtle::Type::NormalTurtle, textures, fonts);
	playerAircraft = leader.get();
	playerAircraft->setPosition(spawnPosition.x,spawnPosition.y);
	playerAircraft->setVelocity(0.f,0.f);
	sceneLayers[UpperAir]->attachChild(std::move(leader));


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

	/*auto pillar = std::make_unique<Pillar>(Pillar::Type::PillarDown, textures);
	pillar->setPosition(TITLE_WIDTH * 15, TITLE_HEIGHT * 5);
	pillar->setVelocity(-3 * TITLE_WIDTH, 0);
	sceneLayers[LowerAir]->attachChild(std::move(pillar));*/

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

void World::makePillarChunk(float y)
{
	auto pillarChunk = std::make_unique<PillarGroup>(textures);
	pillarChunk->setPosition(TITLE_WIDTH * 17, TITLE_HEIGHT * y);
	sceneLayers[LowerAir]->attachChild(std::move(pillarChunk));
}

void World::spawnPillars(sf::Time dt)
{
	if (spawnTimeCountDown1 <= sf::Time::Zero)
	{
		makePillarChunk(3);

		spawnTimeCountDown1 = pillarSpawnRate;
	}
	else {
		spawnTimeCountDown1 -= dt;
	}
}

void World::checkFrogPos()
{

}

void World::killTurtle()
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


sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(worldView.getCenter() - worldView.getSize() / 2.f , worldView.getSize() );
}

sf::FloatRect World::getBattlefield() const
{
	sf::FloatRect bounds = getViewBounds();
	bounds.left -= 100.f;
	bounds.width += 600.f;
	bounds.top -= 2000.f;
	bounds.height += 4000.f;

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
		if (matchesCategories(pair, Category::type::Turtle, Category::type::Pillar))
		{
			auto& turtle = static_cast<Turtle&>(*(pair.first));
			auto& pillar = static_cast<Pillar&>(*(pair.second));

			killTurtle();
		}
		if (matchesCategories(pair, Category::type::Turtle, Category::type::ScorePillar))
		{
			auto& turtle = static_cast<Turtle&>(*(pair.first));
			auto& pillar = static_cast<Pillar&>(*(pair.second));

 			turtle.updateScore(1);
		}
		if (matchesCategories(pair, Category::type::PlayerFrog, Category::type::RiverEntities))
		{
			auto& frog = static_cast<Frog&>(*(pair.first));
			auto& riverEntity = static_cast<RiverEntities&>(*(pair.second));


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
