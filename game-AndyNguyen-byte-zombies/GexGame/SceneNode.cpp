#include "SceneNode.h"
#include "Category.h"
#include "Command.h"  
#include "Utility.h"

#include <algorithm>
#include <cassert>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

SceneNode::SceneNode(Category::type category)
    : parent(nullptr)
    , defaultCategory(category)
{}

void SceneNode::attachChild(Ptr child)
{
    child->parent = this;
    children.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detatchCild(const SceneNode& node)
{
    auto found = std::find_if(
        children.begin(),
        children.end(),
        [&](Ptr& p) {return p.get() == &node; }
    );

    assert(found != children.end());

    Ptr result = std::move(*found);
    result->parent = nullptr;
    children.erase(found);
    return result;
}


sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;

    for (const SceneNode* node = this; node != nullptr; node = node->parent) {
        transform = node->getTransform() * transform;
    }

    return transform;
}

unsigned int SceneNode::getCategory() const
{
    return defaultCategory;
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
    // execute if category matches
    if (command.category & getCategory())
        command.action(*this, dt);

    // recurse through all child nodes
    for (auto& child : children)
        child->onCommand(command, dt);
}

sf::FloatRect SceneNode::getBoundingRect() const
{
    return sf::FloatRect();
}

void SceneNode::drawBoundingBox(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::FloatRect rect = getBoundingRect();

    sf::RectangleShape box;
    box.setPosition(sf::Vector2f(rect.left, rect.top));
    box.setSize(sf::Vector2f(rect.width, rect.height));
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::Cyan);
    box.setOutlineThickness(1.f);

    target.draw(box);
}

void SceneNode::checkSceneCollision(SceneNode& rootNode, std::set<Pair>& collisionPairs)
{
    checkNodeCollision(rootNode, collisionPairs);
    for (Ptr& c : rootNode.children)
        checkSceneCollision(*c, collisionPairs);
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs)
{
    if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
        collisionPairs.insert(std::minmax(this, &node));

    for (Ptr& c : children)
        c->checkNodeCollision(node, collisionPairs);
}

bool SceneNode::isDestroyed() const
{
    return false;
}

bool SceneNode::isMarkedForRemoval() const
{
    return isDestroyed();
}

void SceneNode::removeWrecks()
{
    // remove all marked for removal
    auto wreckfieldBegin = std::remove_if(children.begin(), children.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
    children.erase(wreckfieldBegin, children.end());

    std::for_each(children.begin(), children.end(), std::mem_fn(&SceneNode::removeWrecks));
}
 

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
    updateCurrent(dt, commands);
    updateChildren(dt, commands);
}


void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    // virtual do nothing in base class
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
    for (auto& c : children)
        c->update(dt, commands);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // get current transfrorm
    auto transform = getTransform();

    // apply transform to states
    states.transform *= transform;

    // draw 
    drawCurrent(target, states);
    drawChildren(target, states);

    // drawBoundingBox(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    // virtual 
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& c : children)
        c->draw(target, states);
}


bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}


float distance(const SceneNode& lhs, const SceneNode& rhs)
{
    return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}