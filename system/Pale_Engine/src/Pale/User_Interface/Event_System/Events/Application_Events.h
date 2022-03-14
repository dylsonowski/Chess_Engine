#pragma once
#include "../Event_System.h"

//--- Application events - application core actions ---//
//--- List:											---//
//---	1. Setup game tree depth (default: 3)		---// 
//---	2. Exit application							---// 
namespace Pale::UI::Event_System::Application_Events {
	class Game_Tree_Setup_Event : public Event {

	public:
		Game_Tree_Setup_Event() = delete;
		Game_Tree_Setup_Event(unsigned short int gameTreeDepth) : Event(), _gameTreeDepth(gameTreeDepth) { _isHandled = false; }
		~Game_Tree_Setup_Event() = default;

		inline unsigned short int GetGameTreeDepth() const { return _gameTreeDepth; }
		std::string ToString() const override {
			std::stringstream ss;
			ss << "GameTreeDepthSetupEvent: Game tree depth = " << _gameTreeDepth;
			return ss.str();
		}

		EVENT_TYPE(APPLICATION_GAME_TREE_SETUP);
		EVENT_CATEGORY(APPLICATION_EVENT | GAME_EVENT);

	private:
		const unsigned short int _gameTreeDepth;
	};

	class Application_Close_Event : public Event {

	public:
		Application_Close_Event() = delete;
		Application_Close_Event(unsigned int time) : Event(), _exitTimeout(time) { _isHandled = false; }
		~Application_Close_Event() = default;

		inline unsigned int GetExitTimeout() const { return _exitTimeout; }
		std::string ToString() const override {
			std::stringstream ss;
			ss << "ApplicationCloseEvent: Exit timeout = " << _exitTimeout;
			return ss.str();
		}

		EVENT_TYPE(APPLICATION_CLOSE);
		EVENT_CATEGORY(APPLICATION_EVENT);

	private:
		unsigned int _exitTimeout;
	};
}