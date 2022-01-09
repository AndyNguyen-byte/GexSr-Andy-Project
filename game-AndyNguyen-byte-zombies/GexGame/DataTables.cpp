#include "DataTables.h"

std::map<Aircraft::Type, AircraftData> initalizeAircraftData()
{
	std::map<Aircraft::Type, AircraftData> data;

	//Eagle Data
	data[Aircraft::Type::Eagle].hitpoint = 100;
	data[Aircraft::Type::Eagle].speed = 200.f;
	data[Aircraft::Type::Eagle].texture = TextureID::Entities;
	data[Aircraft::Type::Eagle].textureRect = sf::IntRect(0, 0, 48, 64);
	data[Aircraft::Type::Eagle].fireInterval = sf::seconds(1);
	data[Aircraft::Type::Eagle].hasRollAnimation = true;

	//Raptor Data
	data[Aircraft::Type::Raptor].hitpoint = 100;
	data[Aircraft::Type::Raptor].speed = 80.f;
	data[Aircraft::Type::Raptor].texture = TextureID::Entities;
	data[Aircraft::Type::Raptor].textureRect = sf::IntRect(144, 0, 84, 64);
	data[Aircraft::Type::Raptor].fireInterval = sf::Time::Zero;

	data[Aircraft::Type::Raptor].directions.push_back(Direction(+80.f, 80.f));
	data[Aircraft::Type::Raptor].directions.push_back(Direction(-80.f, 160.f));
	data[Aircraft::Type::Raptor].directions.push_back(Direction(+80.f, 80.f));

	//Avenger Data
	data[Aircraft::Type::Avenger].hitpoint = 100;
	data[Aircraft::Type::Avenger].speed = 150.f;
	data[Aircraft::Type::Avenger].texture = TextureID::Entities;
	data[Aircraft::Type::Avenger].textureRect = sf::IntRect(228, 0, 60, 59);
	data[Aircraft::Type::Avenger].fireInterval = sf::seconds(1);

	data[Aircraft::Type::Avenger].directions.push_back(Direction(+70.f, 150.f));
	data[Aircraft::Type::Avenger].directions.push_back(Direction(0.f, 25.f));
	data[Aircraft::Type::Avenger].directions.push_back(Direction(-70.f, 300.f));
	data[Aircraft::Type::Avenger].directions.push_back(Direction(0.f, 25.f));
	data[Aircraft::Type::Avenger].directions.push_back(Direction(+70.f, 150.f));


	return data;
}

std::map<Projectile::Type, ProjectileData> initializeProjectileData()
{
	std::map<Projectile::Type, ProjectileData> data;

	data[Projectile::Type::AlliedBullet].damage = 10;
	data[Projectile::Type::AlliedBullet].speed = 300.f;
	data[Projectile::Type::AlliedBullet].texture = TextureID::Entities;
	data[Projectile::Type::AlliedBullet].textureRect = sf::IntRect(175, 64, 3, 14);

	data[Projectile::Type::EnemyBullet].damage = 10;
	data[Projectile::Type::EnemyBullet].speed = 300.f;
	data[Projectile::Type::EnemyBullet].texture = TextureID::Entities;
	data[Projectile::Type::EnemyBullet].textureRect = sf::IntRect(175, 64, 3, 14);

	data[Projectile::Type::Missile].damage = 200;
	data[Projectile::Type::Missile].speed = 150.f;
	data[Projectile::Type::Missile].texture = TextureID::Entities;
	data[Projectile::Type::Missile].textureRect = sf::IntRect(160, 64, 15, 32);

	return data;
}


std::map<Pickup::Type, PickupData> initializePickupData()
{
	std::map<Pickup::Type, PickupData> data;

	data[Pickup::Type::HealthRefill].texture = TextureID::Entities;
	data[Pickup::Type::HealthRefill].textureRect = sf::IntRect(0, 64, 40, 40);
	data[Pickup::Type::HealthRefill].action = [](Aircraft& a) {a.repair(25); };

	data[Pickup::Type::MissileRefill].texture = TextureID::Entities;
	data[Pickup::Type::MissileRefill].textureRect = sf::IntRect(40, 64, 40, 40);
	data[Pickup::Type::MissileRefill].action = [](Aircraft& a) {a.collectMissiles(3); };

	data[Pickup::Type::FireSpread].texture = TextureID::Entities;
	data[Pickup::Type::FireSpread].textureRect = sf::IntRect(80, 64, 40, 40);
	data[Pickup::Type::FireSpread].action = [](Aircraft& a) {a.increaseFireSpread(); };

	data[Pickup::Type::FireRate].texture = TextureID::Entities;
	data[Pickup::Type::FireRate].textureRect = sf::IntRect(120, 64, 40, 40);
	data[Pickup::Type::FireRate].action = [](Aircraft& a) {a.increaseFireRate(); };

	return data;
}

std::map<Particle::Type, ParticleData> initializeParticleData()
{
	std::map<Particle::Type, ParticleData> data;

	data[Particle::Type::Propellant].color = sf::Color(255, 255, 50);
	data[Particle::Type::Propellant].lifetime = sf::seconds(0.6f);

	data[Particle::Type::Smoke].color = sf::Color(50, 50, 50);
	data[Particle::Type::Smoke].lifetime = sf::seconds(4.f);

	return data;
}

std::map<Actor::Type, ActorData> initializeActorData()
{
    std::map<Actor::Type, ActorData> data;
    data[Actor::Type::Hero2].texture = TextureID::Hero2;
    data[Actor::Type::Hero2].hitpoints = 100;
    data[Actor::Type::Hero2].damageDone = 5;
    data[Actor::Type::Hero2].speed = 50;
    JsonFrameParser frames = JsonFrameParser("../Zombies_Media/Textures/hero2.json");
    data[Actor::Type::Hero2].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
    data[Actor::Type::Hero2].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero2].animations[Actor::State::Idle].setRepeating(true);
    data[Actor::Type::Hero2].animations[Actor::State::Jump].addFrameSet(frames.getFramesFor("jump"));
    data[Actor::Type::Hero2].animations[Actor::State::Jump].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero2].animations[Actor::State::Jump].setRepeating(false);
    data[Actor::Type::Hero2].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
    data[Actor::Type::Hero2].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero2].animations[Actor::State::Attack].setRepeating(false);
    data[Actor::Type::Hero2].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
    data[Actor::Type::Hero2].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero2].animations[Actor::State::Walk].setRepeating(true);
    data[Actor::Type::Hero2].animations[Actor::State::Run].addFrameSet(frames.getFramesFor("run"));
    data[Actor::Type::Hero2].animations[Actor::State::Run].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero2].animations[Actor::State::Run].setRepeating(true);
    data[Actor::Type::Hero2].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
    data[Actor::Type::Hero2].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
    data[Actor::Type::Hero2].animations[Actor::State::Dead].setRepeating(false);

    data[Actor::Type::Zombie1].texture = TextureID::Zombie1;
    data[Actor::Type::Zombie1].hitpoints = 100;
    data[Actor::Type::Zombie1].damageDone = 1;
    data[Actor::Type::Zombie1].speed = 50;
    frames = JsonFrameParser("../Zombies_Media/Textures/zombie1.json");
    data[Actor::Type::Zombie1].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
    data[Actor::Type::Zombie1].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie1].animations[Actor::State::Idle].setRepeating(true);
    data[Actor::Type::Zombie1].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
    data[Actor::Type::Zombie1].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie1].animations[Actor::State::Attack].setRepeating(true);
    data[Actor::Type::Zombie1].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
    data[Actor::Type::Zombie1].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie1].animations[Actor::State::Walk].setRepeating(true);
    data[Actor::Type::Zombie1].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
    data[Actor::Type::Zombie1].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie1].animations[Actor::State::Dead].setRepeating(false);
    data[Actor::Type::Zombie1].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("rise"));
    data[Actor::Type::Zombie1].animations[Actor::State::Rise].setDuration(sf::seconds(2.f));
    data[Actor::Type::Zombie1].animations[Actor::State::Rise].setRepeating(false);
    data[Actor::Type::Zombie1].directions.emplace_back(Direction(45.f, 50.f));
    data[Actor::Type::Zombie1].directions.emplace_back(Direction(-45.f, 100.f));
    data[Actor::Type::Zombie1].directions.emplace_back(Direction(45.f, 50.f));

    data[Actor::Type::Zombie2].texture = TextureID::Zombie2;
    data[Actor::Type::Zombie2].hitpoints = 100;
    data[Actor::Type::Zombie2].damageDone = 1;
    data[Actor::Type::Zombie2].speed = 50;
    frames = JsonFrameParser("../Zombies_Media/Textures/zombie2.json");
    data[Actor::Type::Zombie2].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
    data[Actor::Type::Zombie2].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie2].animations[Actor::State::Idle].setRepeating(true);
    data[Actor::Type::Zombie2].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
    data[Actor::Type::Zombie2].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie2].animations[Actor::State::Attack].setRepeating(true);
    data[Actor::Type::Zombie2].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
    data[Actor::Type::Zombie2].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie2].animations[Actor::State::Walk].setRepeating(true);
    data[Actor::Type::Zombie2].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
    data[Actor::Type::Zombie2].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie2].animations[Actor::State::Dead].setRepeating(false);
    data[Actor::Type::Zombie2].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("rise"));
    data[Actor::Type::Zombie2].animations[Actor::State::Rise].setDuration(sf::seconds(2.f));
    data[Actor::Type::Zombie2].animations[Actor::State::Rise].setRepeating(false);
    data[Actor::Type::Zombie2].directions.emplace_back(Direction(45.f, 50.f));
    data[Actor::Type::Zombie2].directions.emplace_back(Direction(-45.f, 100.f));
    data[Actor::Type::Zombie2].directions.emplace_back(Direction(45.f, 50.f));

    data[Actor::Type::Zombie3].texture = TextureID::Zombie3;
    data[Actor::Type::Zombie3].hitpoints = 100;
    data[Actor::Type::Zombie3].damageDone = 1;
    data[Actor::Type::Zombie3].speed = 50;
    frames = JsonFrameParser("../Zombies_Media/Textures/zombie3.json");
    data[Actor::Type::Zombie3].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
    data[Actor::Type::Zombie3].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie3].animations[Actor::State::Idle].setRepeating(true);
    data[Actor::Type::Zombie3].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
    data[Actor::Type::Zombie3].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie3].animations[Actor::State::Attack].setRepeating(true);
    data[Actor::Type::Zombie3].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
    data[Actor::Type::Zombie3].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie3].animations[Actor::State::Walk].setRepeating(true);
    data[Actor::Type::Zombie3].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
    data[Actor::Type::Zombie3].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie3].animations[Actor::State::Dead].setRepeating(false);
    data[Actor::Type::Zombie3].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("walk"));
    data[Actor::Type::Zombie3].animations[Actor::State::Rise].setDuration(sf::seconds(2.f));
    data[Actor::Type::Zombie3].animations[Actor::State::Rise].setRepeating(false);
    data[Actor::Type::Zombie3].directions.emplace_back(Direction(45.f, 50.f));
    data[Actor::Type::Zombie3].directions.emplace_back(Direction(-45.f, 100.f));
    data[Actor::Type::Zombie3].directions.emplace_back(Direction(45.f, 50.f));
    data[Actor::Type::Zombie4].texture = TextureID::Zombie4;
    data[Actor::Type::Zombie4].hitpoints = 100;
    data[Actor::Type::Zombie4].damageDone = 1;
    data[Actor::Type::Zombie4].speed = 50;
    frames = JsonFrameParser("../Zombies_Media/Textures/zombie4.json");
    data[Actor::Type::Zombie4].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
    data[Actor::Type::Zombie4].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie4].animations[Actor::State::Idle].setRepeating(true);
    data[Actor::Type::Zombie4].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
    data[Actor::Type::Zombie4].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie4].animations[Actor::State::Attack].setRepeating(true);
    data[Actor::Type::Zombie4].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
    data[Actor::Type::Zombie4].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie4].animations[Actor::State::Walk].setRepeating(true);
    data[Actor::Type::Zombie4].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
    data[Actor::Type::Zombie4].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie4].animations[Actor::State::Dead].setRepeating(false);
    data[Actor::Type::Zombie4].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("walk"));
    data[Actor::Type::Zombie4].animations[Actor::State::Rise].setDuration(sf::seconds(2.f));
    data[Actor::Type::Zombie4].animations[Actor::State::Rise].setRepeating(false);
    data[Actor::Type::Zombie4].directions.emplace_back(Direction(45.f, 50.f));
    data[Actor::Type::Zombie4].directions.emplace_back(Direction(-45.f, 100.f));
    data[Actor::Type::Zombie4].directions.emplace_back(Direction(45.f, 50.f));
    data[Actor::Type::Zombie5].texture = TextureID::Zombie5;
    data[Actor::Type::Zombie5].hitpoints = 100;
    data[Actor::Type::Zombie5].damageDone = 1;
    data[Actor::Type::Zombie5].speed = 50;
    frames = JsonFrameParser("../Zombies_Media/Textures/zombie5.json");
    data[Actor::Type::Zombie5].animations[Actor::State::Idle].addFrameSet(frames.getFramesFor("idle"));
    data[Actor::Type::Zombie5].animations[Actor::State::Idle].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie5].animations[Actor::State::Idle].setRepeating(true);
    data[Actor::Type::Zombie5].animations[Actor::State::Attack].addFrameSet(frames.getFramesFor("attack"));
    data[Actor::Type::Zombie5].animations[Actor::State::Attack].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie5].animations[Actor::State::Attack].setRepeating(true);
    data[Actor::Type::Zombie5].animations[Actor::State::Walk].addFrameSet(frames.getFramesFor("walk"));
    data[Actor::Type::Zombie5].animations[Actor::State::Walk].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie5].animations[Actor::State::Walk].setRepeating(true);
    data[Actor::Type::Zombie5].animations[Actor::State::Dead].addFrameSet(frames.getFramesFor("dead"));
    data[Actor::Type::Zombie5].animations[Actor::State::Dead].setDuration(sf::seconds(1.f));
    data[Actor::Type::Zombie5].animations[Actor::State::Dead].setRepeating(false);
    data[Actor::Type::Zombie5].animations[Actor::State::Rise].addFrameSet(frames.getFramesFor("walk"));
    data[Actor::Type::Zombie5].animations[Actor::State::Rise].setDuration(sf::seconds(2.f));
    data[Actor::Type::Zombie5].animations[Actor::State::Rise].setRepeating(false);
    data[Actor::Type::Zombie5].directions.emplace_back(Direction(45.f, 50.f));
    data[Actor::Type::Zombie5].directions.emplace_back(Direction(-45.f, 100.f));
    data[Actor::Type::Zombie5].directions.emplace_back(Direction(45.f, 50.f));

    return data;
}