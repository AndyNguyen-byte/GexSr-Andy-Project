#pragma once
#include "SceneNode.h"
#include "Command.h"
#include "CommandQueue.h"

class Entity : public SceneNode
{
public:
	explicit			Entity(int hitPoints);
	void				setVelocity(sf::Vector2f v);
	void				setVelocity(float vx, float vy);
	virtual void		accelerate(sf::Vector2f velocity);
	virtual void		accelerate(float vx, float vy);
	sf::Vector2f		getVelocity() const;

	int					getHitPoints() const;
	virtual void		damage(int points);
	void				repair(int points);
	void				destroy();
	virtual bool		isDestroyed() const;
	virtual  void		remove();

protected:
	virtual void		updateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	sf::Vector2f		velocity;
	int					hitPoints;

};

