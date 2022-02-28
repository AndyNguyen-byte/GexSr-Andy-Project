#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SceneNode.h"

#include "SpriteNode.h"
#include "Aircraft.h"
#include "CommandQueue.h"
#include "Command.h"
#include "BloomEffect.h"
#include "Actor.h"
#include "Frog.h"
#include  "Sharks.h"
#include "Ground.h"
#include "Lilypad.h"
#include "Vehicles.h"
#include "RiverEntities.h"

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include "Turtle.h"


namespace sf {
	class RenderTarget;
}
class SoundPlayer;

class World 
{
public: 
					World(const World&) = delete;
					World(sf::RenderTarget& outputTarget, FontHolder_t& fonts, SoundPlayer& sounds);

	void			update(sf::Time dt);
	void			updateSounds();
	void			draw();
	 
	CommandQueue&	getCommandQueue();

	bool			hasAlivePlayer() const;
	bool			hasPlayerReachedEnd() const;
	int				getScore();

private:
	void			loadTextures();
	void			buildScene();

	void			resetGroundPos();
	void			makePillarChunk(float y);
	void			spawnPillars(sf::Time dt);
	void			makeSharks(Sharks::Type type, float x, float y);
	void			spawnSharks(sf::Time dt);
	void			checkTurtlePos();
	void			killTurtle();
	void			difficultySet();
	void			reverseTurtle();
	void			reverseGamePlay();
	
	sf::FloatRect	getViewBounds() const;
	sf::FloatRect	getBattlefield() const;

	void			handleCollisions();
	void			destroyEntitiesOutOfView();
	

private:

	enum Layer
	{
		Background = 0,
		LowerAir,
		UpperAir,
		LayerCount
	};


private:
	sf::RenderTarget&					target;
	sf::RenderTexture					sceneTexture;

	sf::View							worldView;
	TextureHolder_t						textures;
	FontHolder_t&						fonts;
	SoundPlayer&						sounds;



	SceneNode							sceneGraph;
	std::array<SceneNode*, LayerCount>	sceneLayers;
	CommandQueue						commands;

	sf::FloatRect						worldBounds;

	sf::Vector2f						spawnPosition;
	float								scrollSpeed;

	Turtle*								playerTurtle;
	Ground*								ground1;
	Ground*								ground2;


	sf::Time							spawnTimeCountDown1 = sf::seconds(1.f);
	sf::Time							spawnTimeCountDown2 = sf::seconds(1.f);

	sf::Time							pillarSpawnRate = sf::seconds(2.5f);
	sf::Time							sharkSpawnRate = sf::seconds(3.5f);
	
	bool								enableSharks = false;
	bool								enableReverseGameplay = false;

	int									numberOfLilypadsOccupied = 0;
	int									turtleLives = 2;

	BloomEffect							bloomEffect;
};

