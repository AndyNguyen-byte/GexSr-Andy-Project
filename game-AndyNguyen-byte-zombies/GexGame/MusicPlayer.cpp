#include "MusicPlayer.h"
#include <stdexcept>

MusicPlayer::MusicPlayer()
	: music()
	, filenames()
	, volume(25)
{


	filenames[MusicID::TitleTheme] = "../Media/Music/dp_frogger.flac";
	filenames[MusicID::GameTheme] = "../Media/Music/dp_frogger_tweener.flac";
}

void MusicPlayer::play(MusicID theme)
{
	if (!music.openFromFile(filenames[theme]))
		throw std::runtime_error("Music could not open file");

	music.setVolume(volume);
	music.setLoop(true);
	music.play();
}

void MusicPlayer::stop()
{
	music.stop();
}

void MusicPlayer::setPaused(bool paused)
{
	if (paused)
		music.pause();
	else
		music.play();
}

void MusicPlayer::setVolume(float volume)
{
	volume = volume;
	music.setVolume(volume);
}
