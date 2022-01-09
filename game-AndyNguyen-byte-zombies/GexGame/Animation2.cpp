#include "Animation2.h"

#include <cassert>


Animation2::Animation2(bool repeat)
	: currentFrame(0)
	, duration(sf::Time::Zero)
	, elapsedTime(sf::Time::Zero)
	, repeat(repeat)
{}

void Animation2::addFrame(Frame f)
{
	frames.emplace_back(f);
}

void Animation2::addFrameSet(std::vector<Frame> f )
{
	frames = f ;
}

void Animation2::setDuration(sf::Time d )
{
	duration = d ;
}

sf::Time Animation2::getDuration() const
{
	return duration;
}

void Animation2::setRepeating(bool flag)
{
	repeat = flag;
}

bool Animation2::isRepeating() const
{
	return repeat;
}

void Animation2::restart()
{
	currentFrame = 0;
}

bool Animation2::isFinished() const
{
	return (!repeat && currentFrame >= frames.size());
}

Frame Animation2::getCurrentFrame() const
{
	return frames[currentFrame >= frames.size() ? frames.size() - 1 : currentFrame];
}


Frame Animation2::update(sf::Time dt)
{
	sf::Time timePerFrame = duration / static_cast<float>(frames.size());
	elapsedTime += dt;

	// While we have a frame to process
	while (elapsedTime >= timePerFrame && (currentFrame < frames.size() || repeat))
	{
		// And progress to next frame
		elapsedTime -= timePerFrame;
		currentFrame++;
		if (repeat)
		{
			currentFrame %= frames.size();
		}
	}
	return getCurrentFrame();
}
