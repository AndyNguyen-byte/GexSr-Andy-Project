#pragma once
#include "Entity.h"

#include "ResourceIdentifiers.h"
#include "Command.h"
#include "CommandQueue.h"

#include <SFML/Graphics/Sprite.hpp>

class Lilypad : public Entity
{

public:

	enum class State
	{
		Empty,
		Frog,
		Croc,

	};

	Lilypad(const TextureHolder_t& textures);

	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual unsigned int	getCategory() const override;

	virtual sf::FloatRect	getBoundingRect() const override;

	bool					isLilyPadOccupied();
	void					setOccupied(bool x);
	void					addFrog();

private:
	int						id;
	sf::Sprite				sprite;
	bool					isOccupied=false;
};

