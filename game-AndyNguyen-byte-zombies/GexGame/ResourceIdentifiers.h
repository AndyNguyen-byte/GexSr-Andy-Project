#pragma once

namespace sf {
	class Texture;
	class Font;
}

enum class TextureID
{
	Zombie1,
	Zombie2,
	Zombie3,
	Zombie4,
	Zombie5,
	Hero2,

	Entities,
	Jungle,
	Road,
	TitleScreen,
	Explosion,
	Particle,
	FinishLine,

	MissisleRefill,

	Marsh,
	Ground,
	Turtle1,
	Turtle2,
	Turtle3,
	PillarUp1,
	PillarDown1,
	SharkLeft,
	SharkRight,

	PlayerFrog,
	BonusFrog,
	FrogIcon,
	Atlas,
	
};

enum class FontID
{
	Main
};

enum class SoundEffectID
{
	AlliedGunfire,
	EnemyGunfire,
	Explosion1,
	Explosion2,
	LaunchMissile,
	CollectPickup,
	Button,

	FrogDie,
	FrogJump,

	HopSound,
	ScoreSound,
	DeathSound,
	HitSound,
};


enum class  MusicID
{
	MenuTheme,
	MissionTheme,

	TitleTheme,
	GameTheme,
};


// forward declaration
template <typename Resource, typename Id>
class ResourceHolder;

using TextureHolder_t = ResourceHolder<sf::Texture, TextureID>;
using FontHolder_t = ResourceHolder<sf::Font, FontID>;