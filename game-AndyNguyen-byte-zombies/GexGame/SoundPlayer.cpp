#include "SoundPlayer.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Audio/Listener.hpp>
#include <cassert>
#include <cmath>
#include <stdexcept>

namespace
{
	// Sound coordinate system, point of view of a player in front of the screen:
	// X = left; Y = up; Z = back (out of the screen)
	const float ListenerZ = 300.f;
	const float Attenuation = 8.f;
	const float MinDistance2D = 200.f;
	const float MinDistance3D = std::sqrt(MinDistance2D * MinDistance2D + ListenerZ * ListenerZ);
}


SoundPlayer::SoundPlayer()
	: soundBuffers()
	, sounds()
{
	loadBuffer(SoundEffectID::AlliedGunfire, "../Media/Sound/AlliedGunfire.wav");
	loadBuffer(SoundEffectID::EnemyGunfire, "../Media/Sound/EnemyGunfire.wav");
	loadBuffer(SoundEffectID::Explosion1, "../Media/Sound/Explosion1.wav");
	loadBuffer(SoundEffectID::Explosion2, "../Media/Sound/Explosion2.wav");
	loadBuffer(SoundEffectID::LaunchMissile, "../Media/Sound/LaunchMissile.wav");
	loadBuffer(SoundEffectID::CollectPickup, "../Media/Sound/CollectPickup.wav");
	loadBuffer(SoundEffectID::Button, "../Media/Sound/Button.wav");
	//loadBuffer(SoundEffectID::FrogJump, "../Media/Sound/sound-frogger-hop.wav");
	loadBuffer(SoundEffectID::FrogDie, "../Media/Sound/froggerDie.wav");

	// Listener points towards the screen (default in SFML)
	sf::Listener::setDirection(0.f, 0.f, -1.f);
}

void SoundPlayer::play(SoundEffectID effect)
{
	play(effect, getListnerPosition());
}

void SoundPlayer::play(SoundEffectID effect, sf::Vector2f position)
{
	sounds.push_back(sf::Sound());
	sf::Sound& sound = sounds.back();

	sound.setBuffer(*soundBuffers[effect]);
	sound.setPosition(position.x, -position.y, 0.f);
	sound.setAttenuation(Attenuation);
	sound.setMinDistance(MinDistance3D);

	sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
	sounds.remove_if([](const sf::Sound& s)
		{
			return s.getStatus() == sf::Sound::Stopped;
		});
}

void SoundPlayer::setListnerPosition(sf::Vector2f position)
{
	sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListnerPosition() const
{
	sf::Vector3f pos = sf::Listener::getPosition();
	return sf::Vector2f(pos.x, -pos.y);
}

void SoundPlayer::loadBuffer(SoundEffectID id, const std::string path)
{
	std::unique_ptr<sf::SoundBuffer> buffer(new sf::SoundBuffer);
	if (!buffer->loadFromFile(path))
		throw std::runtime_error("Sound Effect Load Failed");

	auto inserted = soundBuffers.insert(std::make_pair(id, std::move(buffer)));
	assert(inserted.second);
}
