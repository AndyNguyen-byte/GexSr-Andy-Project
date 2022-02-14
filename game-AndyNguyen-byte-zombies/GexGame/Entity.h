#pragma once
 
#include "SceneNode.h"

class Entity : public SceneNode
{

public: 
	explicit			Entity(int hitPoints);

	void				setVelocity(sf::Vector2f v);
	void				setVelocity(float vx, float vy);
	virtual void		accelerate(sf::Vector2f velocity);
	virtual void		accelerate(float vx, float vy);
	virtual void		hop(float x, float y);
	sf::Vector2f		getVelocity() const; 

	int					getHitpoints() const;
	virtual void		damage(int points);
	void				repair(int points);
	void				destroy(); 
	virtual bool		isDestroyed() const;
	virtual void		remove();

protected:
	virtual void	updateCurrent(sf::Time dt, CommandQueue& commands) override;

private:
	sf::Vector2f	velocity; 
	int				hitPoints;
};

