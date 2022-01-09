#include "PlayerControl.h"
#include "Aircraft.h"
#include "Command.h"
#include "CommandQueue.h"
#include "Actor.h"

PlayerControl::PlayerControl()
{
    initializeKeys();
    initializeActions();

    //set categories
    for (auto& [action,cmd] : actionBindings) {
        cmd.category = Category::Hero;
    }
}

void PlayerControl::handleEvent(const sf::Event& event, CommandQueue& commands)
{
    if (event.type == sf::Event::KeyPressed)
    {
        auto found = keyBindings.find(event.key.code);

        if (found != keyBindings.end() && !isRealTimeAction(found->second))
            commands.push(actionBindings[found->second]);
    }
}

void PlayerControl::handleRealTimeInput(CommandQueue& commands)
{
    for (auto [key,action]:keyBindings)
    {
        if (sf::Keyboard::isKeyPressed(key) && isRealTimeAction(action))
            commands.push(actionBindings[action]);
    }
}

void PlayerControl::assignKey(Action action, sf::Keyboard::Key key)
{
    // remove all keys that already map to action
    for (auto itr = keyBindings.begin(); itr != keyBindings.end();)
    {
        if (itr->second == action)
            keyBindings.erase(itr++);
        else
            ++itr;
    }

    // insert new binding
    keyBindings[key] = action;
}

sf::Keyboard::Key PlayerControl::getAssignedKey(Action action) const
{
    for (auto [key, boundAction] : keyBindings)
    {
        if (boundAction == action)
            return key;
    }
    return sf::Keyboard::Unknown;
}

void PlayerControl::setMissionStatus(MissionStatus status)
{
    currentMissionStatus = status;
}

PlayerControl::MissionStatus PlayerControl::getMissionStatus() const
{
    return currentMissionStatus;
}



void PlayerControl::initializeKeys()
{
    //arrows
    keyBindings[sf::Keyboard::Left]     = Action::MoveLeft;
    keyBindings[sf::Keyboard::Right]    = Action::MoveRight;
    keyBindings[sf::Keyboard::Up]       = Action::MoveUp;
    keyBindings[sf::Keyboard::Down]     = Action::MoveDown;

    //ASWD
    keyBindings[sf::Keyboard::A]        = Action::MoveLeft;
    keyBindings[sf::Keyboard::D]        = Action::MoveRight;
    keyBindings[sf::Keyboard::W]        = Action::MoveUp;
    keyBindings[sf::Keyboard::S]        = Action::MoveDown;

    //projectiles
    keyBindings[sf::Keyboard::Space]    = Action::Attack;
    keyBindings[sf::Keyboard::K]        = Action::ForceField;
}

void PlayerControl::initializeActions()
{
    const float speed = 200.f;

    actionBindings[Action::MoveLeft].action = derivedAction<Actor>([=](Actor& n, sf::Time dt) {n.accelerate(-speed, 0.f); });
    actionBindings[Action::MoveLeft].category = Category::Hero;

    actionBindings[Action::MoveRight].action = derivedAction<Actor>([=](Actor& n, sf::Time dt) {n.accelerate(speed, 0.f); });
    actionBindings[Action::MoveRight].category = Category::Hero;

    actionBindings[Action::MoveUp].action = derivedAction<Actor>([=](Actor& n, sf::Time dt) {n.accelerate(0.f, -speed); });
    actionBindings[Action::MoveUp].category = Category::Hero;

    actionBindings[Action::MoveDown].action = derivedAction<Actor>([=](Actor& n, sf::Time dt) {n.accelerate(0.f, speed); });
    actionBindings[Action::MoveDown].category = Category::Hero;

    actionBindings[Action::Attack].action = derivedAction<Actor>([=](Actor& n, sf::Time dt) {n.attack(); });
    actionBindings[Action::Attack].category = Category::Hero;

    actionBindings[Action::ForceField].action = derivedAction<Actor>([=](Actor& n, sf::Time dt) {n.forceField(); });
    actionBindings[Action::ForceField].category = Category::Hero;


}

bool PlayerControl::isRealTimeAction(Action action)
{
    switch (action)
    {
    case PlayerControl::Action::MoveLeft:
    case PlayerControl::Action::MoveRight:
    case PlayerControl::Action::MoveUp:
    case PlayerControl::Action::MoveDown:
    case PlayerControl::Action::Attack:
        return true;
    default:
        return false;
    }
    
}
