#pragma once
#include "Pale/User_Interface/User_Interface.h"

namespace Pale {
	namespace UI {
		namespace CommandLine {
			class Command_Line_UI: public User_Interface {

			public:
				Command_Line_UI() = delete;
				Command_Line_UI(std::shared_ptr<Chess_Logic::Board_Representation<std::string>> boardRepresentation, unsigned short int numberAI) : User_Interface(boardRepresentation), _applicationState(APP_STATES::MAIN_MENU_STATE), _insertMoveCommand(false), _numberOfAIInstances(numberAI), _printHelp(false), _invalidCommandInserted(false) {}
				~Command_Line_UI() = default;

				void Draw() const override;
				void Update(std::shared_ptr<Chess_Logic::Board_Representation<std::string>> boardRepresentation, APP_STATES applicationState) override;

			private:
				//--- Private function rendering each component ---//
				void RenderLogo() const;
				void RenderBoard() const;
				void RenderMainMenu() const;
				void RenderGameMenu() const;
				void RenderUserHelp() const;
				void RenderEndstate() const;

				APP_STATES _applicationState;
				bool _insertMoveCommand, _printHelp, _invalidCommandInserted;
				unsigned short int _numberOfAIInstances, _turnCounter;
			};
		}
	}
}