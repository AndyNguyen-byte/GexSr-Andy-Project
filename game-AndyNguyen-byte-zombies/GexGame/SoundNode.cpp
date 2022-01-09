#include "SoundNode.h"
#include "SoundPlayer.h"

SoundNode::SoundNode(SoundPlayer& player)
	: SceneNode()
	, sounds(player)
{}

void SoundNode::playSound(SoundEffectID sound, sf::Vector2f position)
{
	sounds.play(sound, position);
}

unsigned SoundNode::getCategory() const
{
	return Category::SoundEffect;
}
