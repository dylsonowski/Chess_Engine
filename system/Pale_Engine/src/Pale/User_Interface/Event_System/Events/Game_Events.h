#pragma once
#include "../Event_System.h"

//--- Game events - actions related to game itself		   ---//
//--- List:												   ---//
//---	1. Reset environment (can set new game tree depth) ---// 
//---	2. Make move event 								   ---//
namespace Pale::UI::Event_System::Game_Events {
	class Reset_Environment_Event : public Event {

	public:
		Reset_Environment_Event() = delete;
		Reset_Environment_Event(unsigned short int newGameTreeDepth) : Event(), _newGameTreeDepth(newGameTreeDepth) { _isHandled = false; }
		~Reset_Environment_Event() = default;

		inline unsigned short int GetNewGameTreeDepth() const { return _newGameTreeDepth; }
		std::string ToString() const override {
			std::stringstream ss;
			ss << "ResetEnvironmentEvent: New game tree depth = " << _newGameTreeDepth;
			return ss.str();
		}

		EVENT_TYPE(GAME_RESET_ENVIRONMENT);
		EVENT_CATEGORY(APPLICATION_EVENT | GAME_EVENT);

	private:
		const unsigned short int _newGameTreeDepth;
	};

	class Make_Move_Event : public Event {

	public:
		Make_Move_Event() = delete;
		Make_Move_Event(std::string moveCommand) : Event(), _moveCommand(moveCommand) { _isHandled = false; }
		~Make_Move_Event() = default;

		inline std::string GetMoveCommand() const { return _moveCommand; }
		std::string ToString() const override {
			std::stringstream ss;
			ss << "MakeMoveEvent: Move command = " << _moveCommand;
			return ss.str();
		}

		EVENT_TYPE(GAME_MAKE_MOVE);
		EVENT_CATEGORY(GAME_EVENT);

	private:
		std::string _moveCommand;
	};
}