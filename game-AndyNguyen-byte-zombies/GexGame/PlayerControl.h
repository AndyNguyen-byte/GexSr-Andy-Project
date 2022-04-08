#pragma once

#include <SFML/Window/Event.hpp>
#include "Command.h"
#include <map>

class CommandQueue;

class PlayerControl
{
public:

	enum class Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Fire,
		LaunchMissile,
		Attack,
		Jump,
		ForceField,
		ActionCount
	};

	enum class MissionStatus
	{
		MissionRunning,
		MissionSuccess,
		MissionFailure
	};


public:

						PlayerControl();
						PlayerControl(PlayerControl&) = delete;

	void				handleEvent(const sf::Event& event, CommandQueue& commands);
	void				handleRealTimeInput(CommandQueue& commands);

	void				assignKey(Action action, sf::Keyboard::Key key);
	sf::Keyboard::Key	getAssignedKey(Action action) const;

	void				setMissionStatus(MissionStatus status);
	MissionStatus		getMissionStatus() const;

	void				setScore(int score);
	int					getScore() const;

	void				addScoreToList(int score);
	std::string			getHighScores();

private:
	void				initializeKeys();
	void				initializeActions();
	static bool			isRealTimeAction(Action action);

private:
	std::map<sf::Keyboard::Key, Action>		keyBindings;
	std::map<Action, Command>				actionBindings;
	MissionStatus							currentMissionStatus;
	int										_score;
	std::vector<int>						_highScores;
};

