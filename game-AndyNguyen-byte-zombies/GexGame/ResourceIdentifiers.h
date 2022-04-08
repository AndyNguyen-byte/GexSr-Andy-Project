#pragma once

namespace sf {
	class Texture;
	class Font;
}

enum class TextureID
{
	TitleScreen,
	Particle,
	Marsh,
	Ground,
	Turtle1,
	Turtle2,
	Turtle3,
	PillarUp1,
	PillarDown1,
	SharkLeft,
	SharkRight,
	
};

enum class FontID
{
	Main
};

enum class SoundEffectID
{
	HopSound,
	ScoreSound,
	DeathSound,
	HitSound,
};


enum class  MusicID
{
	TitleTheme,
	GameTheme,
};


// forward declaration
template <typename Resource, typename Id>
class ResourceHolder;

using TextureHolder_t = ResourceHolder<sf::Texture, TextureID>;
using FontHolder_t = ResourceHolder<sf::Font, FontID>;