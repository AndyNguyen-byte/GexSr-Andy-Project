#pragma once

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>

#include "Command.h"
#include "CommandQueue.h"
#include "Category.h"
#include "Ultility.h"

#include <vector>
#include <memory>
#include <set>

class SceneNode :public sf::Transformable, public sf::Drawable
{
public:
	using Ptr = std::unique_ptr<SceneNode>;
	using Pair = std::pair<SceneNode*, SceneNode*>;

public:
	SceneNode(const SceneNode&) = delete;//non copyable
	SceneNode(Category::type category = Category::None);

	void								attachChild(Ptr child);
	Ptr									dettachChild(const SceneNode& node);

	void								update(sf::Time dt, CommandQueue& commands);

	sf::Vector2f						getWorldPosition() const;
	sf::Transform						getWorldTransform() const;

	virtual unsigned int				getCategory() const;
	void								onCommand(const Command& command,sf::Time dt);

	virtual sf::FloatRect				getBoundingRect() const;
	void								drawBoundingBox(sf::RenderTarget& target, sf::RenderStates states) const;


	void								checkSceneCollision(SceneNode& rootNode, std::set<Pair>& collisionPairs);
	void								checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);

	virtual bool						isDestroyed() const;
	virtual bool						isMarkedForRemoval() const;
	void								removeWrecks();
private:

	virtual void						updateCurrent(sf::Time dt, CommandQueue& commands);
	void								updateChildren(sf::Time dt, CommandQueue& commands);

	virtual void						draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void						drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void								drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<Ptr>					children;
	SceneNode*							parent;
	Category::type						defauftCategory;

};

bool collision(const SceneNode& lhs, const SceneNode& rhs);
bool distance(const SceneNode& lhs, const SceneNode& rhs);