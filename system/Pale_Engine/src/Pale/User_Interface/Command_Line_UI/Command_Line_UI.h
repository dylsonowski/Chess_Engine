#pragma once
#include "Pale/User_Interface/User_Interface.h"

namespace Pale {
	namespace UI {
		namespace CommandLine {
			class Command_Line_UI: public User_Interface {

			public:
				Command_Line_UI() = delete;
				Command_Line_UI(std::shared_ptr<Chess_Logic::Board_Representation<std::string>> boardRepresentation, unsigned short int numberAI) : User_Interface(boardRepresentation), _applicationState(APP_STATES::MAIN_MENU_STATE), _insertMoveCommand(false), _numberOfAIInstances(numberAI) {}
				~Command_Line_UI() = default;

				void Draw() const override;
				void Update() override;

			private:
				void RenderLogo() const;
				void RenderBoard() const;
				void RenderMainMenu() const;
				void RenderGameMenu(unsigned int turnCounter) const;

				APP_STATES _applicationState;
				bool _insertMoveCommand;
				unsigned short int _numberOfAIInstances;
			};
		}
	}
}