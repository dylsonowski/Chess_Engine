#pragma once
#include "Pale/User_Interface/User_Interface.h"

namespace Pale::UI {
	class Command_Line_UI: public User_Interface {

	public:
		Command_Line_UI() = delete;
		Command_Line_UI(std::shared_ptr<Chess_Logic::Board_Representation<std::string>> boardRepresentation, unsigned short int numberAI, APP_STATES applicationState) 
			: User_Interface(boardRepresentation), _applicationState(applicationState), _drawing(true), _insertMoveCommand(false), _skipUserInput(false), _numberOfAIInstances(numberAI), _printHelp(false), _invalidCommandInserted(false), _turnCounter(1), _resetEnvironment(false), _whiteKingCheck(false), _blackKingCheck(false) {}
		~Command_Line_UI() = default;

		void Draw() override;
		void Update(std::shared_ptr<Chess_Logic::Board_Representation<std::string>> boardRepresentation, APP_STATES applicationState, unsigned short int turn, bool whiteKingStatus, bool blackKingStatus) override;
		void UserInput();

	private:
		//--- Private function rendering each component ---//
		void RenderLogo() const;
		void RenderBoard() const;
		void RenderMainMenu() const;
		void RenderGameMenu() const;
		void RenderUserHelp() const;
		void RenderEndstate() const;

		APP_STATES _applicationState;
		bool _drawing, _skipUserInput, _insertMoveCommand, _printHelp, _invalidCommandInserted, _resetEnvironment, _whiteKingCheck, _blackKingCheck;
		unsigned short int _numberOfAIInstances, _turnCounter;
	};
}