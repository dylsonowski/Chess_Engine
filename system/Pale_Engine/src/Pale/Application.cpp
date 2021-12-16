#include "palepch.h"
#include "Application.h"
#include <SFML/Graphics.hpp>

namespace Pale {
	Application::Application(const bool consoleUIVersion, unsigned short int numberOfAIInstances) 
		: _numberOfAIInstances(numberOfAIInstances), _isRunning(true), _turnCounter(1), _gameTreeDepth(3), _applicationState(APP_STATES::MAIN_MENU_STATE) {
		//--- Setting up logging method ---//
		consoleUIVersion ? LOG_SYSTEM_INIT(LOGGING_METHOD::BASIC) : LOG_SYSTEM_INIT(LOGGING_METHOD::FILE_AND_TERMINAL);

		//--- First, in the costructor needs to be created _boardRepresentation because it is necessary for UI class ---//
		_boardRepresentation = std::make_shared<Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>>>(8, 8, false);

		//--- Set up UI instance ---//
		//--- Convertion board from object type to string type is neccessary because UI use string type for printing ---//
		std::shared_ptr<Pale::Chess_Logic::Board_Representation<std::string>> tempBoardRepresentation = std::make_shared<Pale::Chess_Logic::Board_Representation<std::string>>(Pale::Chess_Logic::ConvertBoard<std::string>(*_boardRepresentation));

		if (consoleUIVersion)
			_UI = std::make_shared<Pale::UI::Command_Line_UI>(tempBoardRepresentation, numberOfAIInstances, _applicationState);
		// else
			// _UI = std::make_shared<Pale::UI::Graphical_UI>(); // TODO: Implement Graphical UI class
	}

	void Application::OnEvent() {
		_UI->_eventEmitter.On<Pale::UI::Event_System::Application_Events::Game_Tree_Setup_Event>([this](Pale::UI::Event_System::Application_Events::Game_Tree_Setup_Event& gameTreeSetupEvent) {
			PALE_ENGINE_TRACE("Event occured! {0}", gameTreeSetupEvent.ToString());
			_gameTreeDepth = gameTreeSetupEvent.GetGameTreeDepth();
			_applicationState = APP_STATES::GAME_STATE;
			}); // TODO: Implement when ResetEnvironment function will be finished

		_UI->_eventEmitter.On<Pale::UI::Event_System::Game_Events::Reset_Environment_Event>([this](Pale::UI::Event_System::Game_Events::Reset_Environment_Event& resetEnvironmentEvent) {
			PALE_ENGINE_TRACE("Event occured! {0}", resetEnvironmentEvent.ToString());
			_gameTreeDepth = resetEnvironmentEvent.GetNewGameTreeDepth();
			}); // TODO: Implement when AI module will be finished

		_UI->_eventEmitter.On<Pale::UI::Event_System::Game_Events::Make_Move_Event>([this](Pale::UI::Event_System::Game_Events::Make_Move_Event& makeMoveEvent) {
			PALE_ENGINE_TRACE("Event occured! {0}", makeMoveEvent.ToString()); 
			_turnCounter++;
			}); // TODO: Implement move hendling

		_UI->_eventEmitter.On<Pale::UI::Event_System::Application_Events::Application_Close_Event>([this](Pale::UI::Event_System::Application_Events::Application_Close_Event& exitApplicationEvent) {
			PALE_ENGINE_TRACE("Event occured! {0}", exitApplicationEvent.ToString());
			system("pause");
			_isRunning = false;
			});
	}

	void Application::Run()	{
		while (_isRunning) {
			_UI->Update(std::make_shared<Pale::Chess_Logic::Board_Representation<std::string>>(Pale::Chess_Logic::ConvertBoard<std::string>(*_boardRepresentation)), _applicationState, _turnCounter);
			OnEvent();
			_UI->Draw();
		}
	}

	//--- Testing purpuses ---//
	//Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>> board(8, 8, false);
	//std::cout << board << "\n\n";

	//Pale::Chess_Logic::Board_Representation<int> intBoard = Pale::Chess_Logic::ConvertBoard<int>(board);
	//std::cout << intBoard << "\n\n";

	//Pale::Chess_Logic::Board_Representation<std::string> stringBoard = Pale::Chess_Logic::ConvertBoard<std::string>(board);
	//std::cout << stringBoard << "\n\n";

	//Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>> objectBoard = Pale::Chess_Logic::ConvertBoard<std::shared_ptr<Pale::Chess_Logic::Pieces>>(stringBoard);
	//std::cout << objectBoard << "\n\n";


	////Move_Command commandCorrect = Pale::Chess_Logic::ProcessMoveCommand("Nb8a6", OWNERS::BLACK, board);
	//Move_Command commandWrong = Pale::Chess_Logic::ProcessMoveCommand("ike2e4", OWNERS::WHITE, board);


	//board.SetPlateValue(1, 4, std::make_shared<Pale::Chess_Logic::Blank>(1, 4));
	//std::cout << board << "\n\n";

	//Move_Command command = Pale::Chess_Logic::ProcessMoveCommand("Pa7a5", OWNERS::BLACK, board);
	//bool canMove = board.MovePiece(command, *board.GetPlateValue(1, 0));
	//std::cout << board << "\n\n";

	//Move_Command commandBad = Pale::Chess_Logic::ProcessMoveCommand("Nb8d8", OWNERS::BLACK, board);
	//bool canMoveBad = board.MovePiece(commandBad, *board.GetPlateValue(1, 0));
	//std::cout << board << "\n\n";

	//std::vector<Move_Command> movesFirst = board.GetPlateValue(0, 1)->GenerateLegitMoves(board.GetBoardRef());
	//std::vector<Move_Command> movesSecond = board.GetPlateValue(0, 3)->GenerateLegitMoves(board.GetBoardRef());
}