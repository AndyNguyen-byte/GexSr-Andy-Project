#include "World.h"
#include "Utility.h"
#include "ParticleNode.h"
#include "SoundNode.h"
#include "Pillar.h"
#include "PillarGroup.h"
#include "Sharks.h"

#include <algorithm>
#include <SFML/Graphics/RenderWindow.hpp>


const float TITLE_HEIGHT = 1024 / 16;
const float TITLE_WIDTH = 1156 / 15;
const int	RIGHT_SPAWN = -5;
const int	LEFT_SPAWN = 17;

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
	, playerTurtle(nullptr)
	, ground1(nullptr)
	, ground2(nullptr)
{
	sceneTexture.create(target.getSize().x, target.getSize().y);

	loadTextures();
	buildScene();

	worldView.setCenter(spawnPosition);
}

void World::update(sf::Time dt)
{
	resetGroundPos();

	checkTurtlePos();

	destroyEntitiesOutOfView();

	// apply all command
	while (!commands.isEmpty())
		sceneGraph.onCommand(commands.pop(), dt);

	handleCollisions();

	sceneGraph.removeWrecks();
	spawnPillars(dt);
	difficultySet();

	if (enableSharks)
	{
		spawnSharks(dt);
	}
	sceneGraph.update(dt, commands); 

	updateSounds();
}

void World::updateSounds()
{
	sounds.setListnerPosition(playerTurtle->getWorldPosition());
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
	return !turtleLives <=0;
}

bool World::hasPlayerReachedEnd() const
{
	return numberOfLilypadsOccupied==5;
}

int World::getScore()
{
	return playerTurtle->getScore();
}

void World::loadTextures()
{
	textures.load(TextureID::Marsh, "../Media/Textures/BackGround1.png");
	textures.load(TextureID::Turtle1, "../Media/Textures/Turtle1.png");
	textures.load(TextureID::PillarUp1, "../Media/Textures/PillarUp.png");
	textures.load(TextureID::PillarDown1, "../Media/Textures/PillarDown.png");
	textures.load(TextureID::SharkLeft, "../Media/Textures/Shark1.png");
	textures.load(TextureID::SharkRight, "../Media/Textures/Shark2.png");
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


	// Add player's turtle
	auto leader = std::make_unique<Turtle>(Turtle::Type::NormalTurtle, textures, fonts);
	playerTurtle = leader.get();
	playerTurtle->setPosition(spawnPosition.x,spawnPosition.y);
	playerTurtle->setVelocity(0.f,0.f);
	sceneLayers[UpperAir]->attachChild(std::move(leader));

	//Add The Grounds
	auto Ground1 = std::make_unique<Ground>(textures);
	ground1 = Ground1.get();
	ground1->setPosition(TITLE_WIDTH * 7.5, TITLE_HEIGHT * 15);
	ground1->setVelocity(-3 * TITLE_WIDTH, 0);
	sceneLayers[LowerAir]->attachChild(std::move(Ground1));

	//Add The Grounds
	auto Ground2 = std::make_unique<Ground>(textures);
	ground2 = Ground2.get();
	ground2->setPosition(TITLE_WIDTH * 22.5, TITLE_HEIGHT * 15);
	ground2->setVelocity(-3 * TITLE_WIDTH, 0);
	sceneLayers[LowerAir]->attachChild(std::move(Ground2));

}


void World::resetGroundPos()
{
	if (ground1->getPosition().x < TITLE_WIDTH * -7.5)
	{
		ground1->setPosition(TITLE_WIDTH * 22.5, TITLE_HEIGHT * 15);
	}

	if (ground2->getPosition().x < TITLE_WIDTH * -7.5)
	{
		ground2->setPosition(TITLE_WIDTH * 22.5, TITLE_HEIGHT * 15);
	}
}

void World::makePillarChunk(float y)
{
	if (!enableReverseGameplay)
	{ 
		auto pillarChunk = std::make_unique<PillarGroup>(textures,-3);
		pillarChunk->setPosition(TITLE_WIDTH * LEFT_SPAWN, TITLE_HEIGHT * y);
		sceneLayers[LowerAir]->attachChild(std::move(pillarChunk));
	}
	else {
		auto pillarChunk = std::make_unique<PillarGroup>(textures,3);
		pillarChunk->setPosition(TITLE_WIDTH * RIGHT_SPAWN, TITLE_HEIGHT * y);
		sceneLayers[LowerAir]->attachChild(std::move(pillarChunk));
	}
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

void World::makeSharks(Sharks::Type type, float x, float y)
{
	auto testShark = std::make_unique<Sharks>(type, textures);
	testShark->setPosition(TITLE_WIDTH * x, TITLE_HEIGHT * y);
	testShark->setVelocity(-3.5 * TITLE_WIDTH, 0);
	sceneLayers[LowerAir]->attachChild(std::move(testShark));
}

void World::spawnSharks(sf::Time dt)
{
	if (spawnTimeCountDown2 <= sf::Time::Zero)
	{
		int y = randomInt(13);

		makeSharks(Sharks::Type::RightShark,17, y);

		spawnTimeCountDown2 = sharkSpawnRate;
	}
	else {
		spawnTimeCountDown2 -= dt;
	}
}

void World::checkTurtlePos()
{
	if (playerTurtle->getPosition().y > TITLE_HEIGHT * 13)
	{
		killTurtle();
	}
}


void World::killTurtle()
{
	turtleLives -= 1;
	playerTurtle->setDeathStatus(true);
}

void World::difficultySet()
{
	/*if (playerTurtle->getScore() == 10)
	{
		enableSharks = true;
	}*/
	if (playerTurtle->getScore() == 4)
	{
		reverseTurtle(true);
		reverseGamePlay(true);
		playerTurtle->setScale(1.f, 1.f);
		//enableSharks = false;
	}
	if (playerTurtle->getScore() == 7)
	{
		reverseTurtle(false);
	}
	/*if (playerTurtle->getScore() == 20)
	{
		reverseGamePlay(true);
		enableSharks = true;
	}*/
}

void World::reverseTurtle(bool state)
{
	playerTurtle->reverseGravity(state);
}

void World::reverseGamePlay(bool state)
{
	enableReverseGameplay = state;
	if (state)
	{
		playerTurtle->setScale(-1.f, 1.f);
	}
	else
	{
		playerTurtle->setScale(1.f, 1.f);
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
			pillar.destroy();

			turtle.playLocalSound(commands, SoundEffectID::ScoreSound);
		}
		if (matchesCategories(pair, Category::type::Turtle, Category::type::Shark))
		{
			auto& turtle = static_cast<Turtle&>(*(pair.first));
			auto& shark = static_cast<Sharks&>(*(pair.second));

			killTurtle();
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
