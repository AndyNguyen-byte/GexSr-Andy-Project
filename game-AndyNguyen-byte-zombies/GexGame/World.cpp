#include "World.h"
#include "Ultility.h"
#include "Pickup.h"
#include "Projectile.h"
#include "ParticleNode.h"
#include "SoundNode.h"

#include <algorithm>

#include <SFML/Graphics/RenderWindow.hpp>


World::World(sf::RenderTarget& outputTarget, FontHolder_t& fonts,SoundPlayer& sounds)
	:target(outputTarget)
	, worldView(outputTarget.getDefaultView())
	, textures()
	, sounds(sounds)
	, fonts(fonts)
	, sceneGraph()
	, sceneLayers()
	, commands()
	, worldBounds(0.f, 0.f, worldView.getSize().x, 4000.f)
	, spawnPosition(worldView.getSize().x / 2.f,
		worldBounds.height - worldView.getSize().y / 2.f)
	, scrollSpeed(-50.f)
	,playerAircraft(nullptr)

{
	sceneTexture.create(target.getSize().x, target.getSize().y);

	loadTextures();
	buildScene();
	worldView.setCenter(spawnPosition);
}

void World::update(sf::Time dt)
{
	playerAircraft->setVelocity(0.f, 0.f);

	//scroll the world
	//worldView.move(0.f, scrollSpeed * dt.asSeconds());

	destroyEnitiesOutOfView();
	guideMissiles();

	while (!commands.isEmpty())
		sceneGraph.onCommand(commands.pop(), dt);

	adaptPlayerVelocity();

	//create new enemies
	spawnEnemies();

	handleCollisions();
	sceneGraph.removeWrecks();

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
	return !playerAircraft->isDestroyed();
}
bool World::hasPlayerReachedEnd() const
{
	return !worldBounds.contains(playerAircraft->getPosition());
}

void World::loadTextures()
{
	textures.load(TextureID::Entities, "../Media/Media/Textures/Entities.png");

	textures.load(TextureID::Jungle, "../Media/Media/Textures/Jungle.png");
	textures.load(TextureID::FinishLine, "../Media/Media/Textures/FinishLine.png");
	textures.load(TextureID::Particle, "../Media/Media/Textures/Particle.png");
	textures.load(TextureID::Explosion, "../Media/Media/Textures/Explosion.png");

	textures.load(TextureID::Zombie1, "../Zombies_Media/Textures/Zombie1.png");
	textures.load(TextureID::Zombie2, "../Zombies_Media/Textures/Zombie2.png");
	textures.load(TextureID::Zombie3, "../Zombies_Media/Textures/Zombie3.png");
	textures.load(TextureID::Zombie4, "../Zombies_Media/Textures/Zombie4.png");
	textures.load(TextureID::Zombie5, "../Zombies_Media/Textures/Zombie5.png");
	textures.load(TextureID::Hero2, "../Zombies_Media/Textures/hero2.png");
	textures.load(TextureID::Road, "../Zombies_Media/Textures/Road.png");
}

void World::buildScene()
{
	//set up layer nodes
	for (std::size_t i = 0; i < LayerCount; ++i) {
		Category::type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		sceneLayers[i] = layer.get();
		sceneGraph.attachChild(std::move(layer));

	}
	// Add the finish line to the scene
	sf::Texture& finishTexture = textures.get(TextureID::FinishLine);
	std::unique_ptr<SpriteNode> finishSprite(new SpriteNode(finishTexture));
	finishSprite->setPosition(0.f, -76.f);
	sceneLayers[Background]->attachChild(std::move(finishSprite));

	//Add smoke particle node to the scene
	std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(Particle::Type::Smoke,textures));
	sceneLayers[LowerAir]->attachChild(std::move(smokeNode));

	//Add smoke propellant particle node to the scene
	std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(Particle::Type::Propellant,textures));
	sceneLayers[LowerAir]->attachChild(std::move(propellantNode));

	// sound effects
	std::unique_ptr<SoundNode> sNode(new SoundNode(sounds));
	sceneGraph.attachChild(std::move(sNode));

	//prepare tiled background	
	sf::Texture& texture = textures.get(TextureID::Road);
	texture.setRepeated(true);
	//background
	sf::IntRect		textureRect(worldBounds);
	auto backgroundSprite = std::make_unique<SpriteNode>(texture, textureRect);
	backgroundSprite->setPosition(worldBounds.left, worldBounds.top);
	sceneLayers[Background]->attachChild(std::move(backgroundSprite));



	//add players aircraft
	auto hero = std::make_unique<Actor>(Actor::Type::Hero2, textures, fonts);
	playerAircraft = hero.get();
	hero->setPosition(spawnPosition);
	hero->setVelocity(40.f, scrollSpeed);
	sceneLayers[UpperAir]->attachChild(std::move(hero));


	addEnemies();
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds(worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize());
	const float borderDistance = 40.f;

	sf::Vector2f position = playerAircraft->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	playerAircraft->setPosition(position);
}

void World::adaptPlayerVelocity()
{

	sf::Vector2f velocity = playerAircraft->getVelocity();

	if (velocity.x != 0 && velocity.y != 0) {
		playerAircraft->setVelocity(velocity / std::sqrt(2.f));
	}

	playerAircraft->accelerate(0.f,0.f);

}

void World::spawnEnemies()
{
	while (!enemySpawnPoints.empty() && enemySpawnPoints.back().y > getBattlefield().top)
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
		}); std::sort(enemySpawnPoints.begin(), enemySpawnPoints.end(),
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
	return sf::FloatRect(worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize());
}

sf::FloatRect World::getBattlefield() const
{
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;
	return bounds;
}

void World::guideMissiles()
{
	//build a list of active enemies
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

	for (auto pair : collisionPairs)
	{
		if (matchesCategories(pair, Category::type::Hero, Category::type::Zombie))
		{
			auto& hero = static_cast<Actor&>(*(pair.first));
			auto& zombie = static_cast<Actor&>(*(pair.second));

			zombie.damage(hero.attackPoints());
			hero.damage(zombie.attackPoints());

			auto zpos = zombie.getPosition();
			auto hpos = hero.getPosition();
			auto diffPos = zpos - hpos;
			zombie.setPosition(zpos + 0.2f * diffPos);
			hero.setPosition(hpos + 0.1f * diffPos);
		}
	}
}

void World::destroyEnitiesOutOfView()
{
	Command command;
	command.category = Category::SpaceJunk;

	command.action = derivedAction<Entity>([this](Entity& e, sf::Time dt)
		{
			if(!getBattlefield().intersects(e.getBoundingRect()))
				e.remove();
		});
	commands.push(command);
}
