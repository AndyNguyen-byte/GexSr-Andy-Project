#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

#include <vector>

//
// An animaiton is a collection of frames. 
// 

using Frame = sf::IntRect;

class Animation2
{
public:
	explicit 					Animation2(bool repeat = true);

	void						addFrame(Frame frame);
	void						addFrameSet(std::vector<Frame> frames);
	void 						setDuration(sf::Time d);
	sf::Time 					getDuration() const;

	void 						setRepeating(bool flag);
	bool 						isRepeating() const;

	void 						restart();
	bool 						isFinished() const;

	Frame						getCurrentFrame() const;

	Frame 						update(sf::Time dt);

public:
	std::vector<Frame>			frames;
	std::size_t					currentFrame;
	sf::Time					elapsedTime;
	sf::Time					duration;
	//sf::Time					timePerFrame;
	bool						repeat;

};
