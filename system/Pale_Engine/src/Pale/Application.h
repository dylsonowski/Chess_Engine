#pragma once
#include "Chess_Module/Board_Representation.h"
#include "User_Interface/Command_Line_UI/Command_Line_UI.h"
#include "User_Interface/Graphical_UI/Graphical_UI.h"

namespace Pale {
	class Application {

	public:
		Application() = delete;
		Application(const bool consoleUIVersion, unsigned short int numberOfAIInstances);
		~Application() = default;

		void Run();
		void OnEvent();

	private:
		void ResetEnvironment();
		void MakeMove(std::string moveCommand);
		void EndGame();

		unsigned short int _numberOfAIInstances, _gameTreeDepth, _turnCounter;
		bool _isRunning;
		APP_STATES _applicationState;
		std::shared_ptr<Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>>> _boardRepresentation;
		std::shared_ptr<Pale::UI::User_Interface> _UI;
		std::vector<std::string> _moveHistory;
	};
}