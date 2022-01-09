#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/System/Vector2.hpp>

#include <map>
#include <list>
#include <string>
#include <memory>

#include "ResourceIdentifiers.h"


class SoundPlayer
{
public:
					SoundPlayer();
					~SoundPlayer() = default;
					SoundPlayer(const SoundPlayer&) = delete;
					SoundPlayer& operator=(const SoundPlayer&) = delete;

	void			play(SoundEffectID effect);
	void			play(SoundEffectID effect, sf::Vector2f position);

	void			removeStoppedSounds();
	void			setListnerPosition(sf::Vector2f position);
	sf::Vector2f	getListnerPosition() const;

private:
	void			loadBuffer(SoundEffectID id, const std::string path);

private:
	std::map<SoundEffectID, std::unique_ptr<sf::SoundBuffer>>	soundBuffers;
	std::list<sf::Sound>										sounds;

};
