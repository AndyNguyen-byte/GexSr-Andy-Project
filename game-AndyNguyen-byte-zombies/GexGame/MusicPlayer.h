#pragma once


#include <SFML/Audio/Music.hpp>
#include "ResourceIdentifiers.h"
#include <string>
#include <map>


class MusicPlayer
{
public:
									MusicPlayer();
									~MusicPlayer() = default;
									MusicPlayer(const MusicPlayer&) = delete;
									MusicPlayer& operator=(const MusicPlayer&) = delete;

	void							play(MusicID theme);
	void							stop();
	void							setPaused(bool paused);
	void							setVolume(float volume);


private:
	sf::Music						music;
	std::map<MusicID, std::string>	filenames;
	float							volume;
};

