#pragma once
namespace sf {
	class Texture;
	class Font;
}
enum class TextureID
{
	Entities,
	Jungle,
	Desert,
	TitleScreen,
	Explosion,
	Particle,
	FinishLine,

	Road,
	Hero2,
	Zombie1,
	Zombie2,
	Zombie3,
	Zombie4,
	Zombie5
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
	Button
};

enum class MusicID
{
	MenuTheme,
	MissionTheme
};

//forward declaration
template<typename Resource, typename Id>
class ResourceHolder;

using TextureHolder_t = ResourceHolder<sf::Texture, TextureID>;
using FontHolder_t = ResourceHolder<sf::Font, FontID>;