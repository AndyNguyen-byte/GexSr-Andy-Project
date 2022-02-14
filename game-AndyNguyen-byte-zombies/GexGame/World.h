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
#include "Ground.h"
#include "Lilypad.h"
#include "Vehicles.h"
#include "RiverEntities.h"

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>


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

private:
	void			loadTextures();
	void			buildScene();

	void			makeLilyPad(float x);
	void			buildLilyPad();
	void			resetGroundPos();
	void			makeVehicle(Vehicles::Type type, float x, float y);
	void			makeRiverEntities(RiverEntities::Type type,float x,float y);
	void			checkFrogPos();
	void			killFrog();

	void			adaptPlayerPosition();
	void			adaptPlayerVelocity();

	void			spawnEnemies();
	void			addEnemies();
	void			addEnemy(Actor::Type type, float relX, float relY);
	
	sf::FloatRect	getViewBounds() const;
	sf::FloatRect	getBattlefield() const;

	void			guideMissiles();
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


	struct SpawnPoint
	{
		SpawnPoint(Actor::Type type, float x, float y) : type(type), x(x), y(y) {}
		Actor::Type						type;
		float							x;
		float							y;
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

	Frog*								playerAircraft;
	Ground*								ground1;
	Ground*								ground2;

	
	int									numberOfLilypadsOccupied = 0;
	int									frogLives = 9999;

	bool								isDrowning;
	bool								isInBounds;

	std::vector<SpawnPoint>				enemySpawnPoints;
	std::vector<Actor*>					activeEnemies;

	BloomEffect							bloomEffect;
};

