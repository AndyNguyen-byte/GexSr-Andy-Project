#include "Animation.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


Animation::Animation()
	: sprite()
	, frameSize()
	, numberOfFrames(0)
	, currentFrame(0)
	, duration(sf::Time::Zero)
	, elapsedTime(sf::Time::Zero)
	, repeat(false)
{
}

Animation::Animation(const sf::Texture& texture)
	: sprite(texture)
	, frameSize()
	, numberOfFrames(0)
	, currentFrame(0)
	, duration(sf::Time::Zero)
	, elapsedTime(sf::Time::Zero)
	, repeat(false)
{
}

void Animation::setTexture(const sf::Texture& texture)
{
	sprite.setTexture(texture);
}

const sf::Texture* Animation::getTexture() const
{
	return sprite.getTexture();
}

void Animation::setFrameSize(sf::Vector2i fsize)
{
	frameSize = fsize;
}

sf::Vector2i Animation::getFrameSize() const
{
	return frameSize;
}

void Animation::setNumFrames(std::size_t numFrames)
{
	numberOfFrames = numFrames;
}

std::size_t Animation::getNumFrames() const
{
	return numberOfFrames;
}

void Animation::setDuration(sf::Time d)
{
	duration = d;
}

sf::Time Animation::getDuration() const
{
	return duration;
}

void Animation::setRepeating(bool flag)
{
	repeat = flag;
}

bool Animation::isRepeating() const
{
	return repeat;
}

void Animation::restart()
{
	currentFrame = 0;
}

bool Animation::isFinished() const
{
	return currentFrame >= numberOfFrames;
}

sf::FloatRect Animation::getLocalBounds() const
{
	return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}

sf::FloatRect Animation::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void Animation::update(sf::Time dt)
{
	sf::Time timePerFrame = duration / static_cast<float>(numberOfFrames);
	elapsedTime += dt;

	sf::Vector2i textureBounds(sprite.getTexture()->getSize());
	sf::IntRect textureRect = sprite.getTextureRect();

	if (currentFrame == 0)
		textureRect = sf::IntRect(0, 0, frameSize.x, frameSize.y);

	// While we have a frame to process
	while (elapsedTime >= timePerFrame && (currentFrame <= numberOfFrames || repeat))
	{
		// Move the texture rect left
		textureRect.left += textureRect.width;

		// If we reach the end of the texture
		if (textureRect.left + textureRect.width > textureBounds.x)
		{
			// Move it down one line
			textureRect.left = 0;
			textureRect.top += textureRect.height;
		}

		// And progress to next frame
		elapsedTime -= timePerFrame;
		if (repeat)
		{
			currentFrame = (currentFrame + 1) % numberOfFrames;

			if (currentFrame == 0)
				textureRect = sf::IntRect(0, 0, frameSize.x, frameSize.y);
		}
		else
		{
			currentFrame++;
		}
	}

	sprite.setTextureRect(textureRect);
}


void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite, states);
}
