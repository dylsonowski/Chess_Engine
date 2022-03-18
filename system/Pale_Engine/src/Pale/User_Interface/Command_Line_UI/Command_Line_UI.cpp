#include "palepch.h"
#include "Command_Line_UI.h"

// IMPORTANT: Game loop = Start => Draw() => Update() => GoTo(Start)

namespace Pale::UI {
	//--- Metod responsible for rendering all necessary components. It work based on Application States to render correct components. ---//
	//--- Start state: logo, welcome message, setting AI depth level. ---//
	//--- Game state: logo, board, game menu, action choosing, move insertion, user help (optional). ---//
	//--- End state: End information, information about winner. ---//
	void Command_Line_UI::Draw()  {
		switch (_applicationState) {
		case APP_STATES::GAME_STATE: {
			system("cls");
			RenderLogo();
			RenderBoard();
			RenderGameMenu();
			break;
		}
		case APP_STATES::END_STATE: {
			system("cls");
			RenderEndstate();
			break;
		}
		default: {
			system("cls");
			RenderMainMenu();
			break;
		}
		}

		//--- Informe Update() function when drawing process has been finished ---//
		_drawing = false;
	}

	void Command_Line_UI::Update(std::shared_ptr<Chess_Logic::Board_Representation<std::string>> boardRepresentation, APP_STATES applicationState, unsigned short int turn, bool whiteKingStatus, bool blackKingStatus) {
		//--- If board state was changed it needs to be updated ---//
		if (*_boardRepresentation != *boardRepresentation) {
			_boardRepresentation = boardRepresentation;
			PALE_ENGINE_INFO("Command_Line_UI->Update() [40]: Command line UI: Board updated!");
		}

		//--- If application state was changed it needs to be updated ---//
		if (_applicationState != applicationState) {
			_applicationState = applicationState;
			PALE_ENGINE_INFO("Command_Line_UI->Update() [46]: Command line UI: Application state updated!");
		}

		//--- If turn counter was changed it needs to be updated ---//
		_turnCounter = turn;

		//--- Change statuses of both kings to check if there are no check ---//
		_whiteKingCheck = whiteKingStatus;
		_blackKingCheck = blackKingStatus;
	}

	void Command_Line_UI::UserInput() {
		if (!_skipUserInput) {
			// IMPORTANT: Depending on appliacation state, different action handling needs to be used
			switch (_applicationState) {
			//--- Application state - Game state ---//
			case APP_STATES::GAME_STATE: {
				if (!_printHelp) {
					if (_resetEnvironment) {
						short unsigned int newTreeDepth;
						std::cin >> newTreeDepth;

						Event_System::Game_Events::Reset_Environment_Event resetEnvironmentEvent(newTreeDepth);
						_eventEmitter.Emit(resetEnvironmentEvent);

						_resetEnvironment = false;
					}
					else if (_insertMoveCommand) {
						//--- Hendling user input for move command ---//
						std::string moveCommand;
						std::cin >> moveCommand;

						//--- Handling request for printing out user help ---//
						if (moveCommand == "?")
							_printHelp = true;
						else {
							Event_System::Game_Events::Make_Move_Event makeMoveEvent(moveCommand);
							_eventEmitter.Emit(makeMoveEvent);
							_insertMoveCommand = false;
						}
					}
					else if (_invalidCommandInserted) {
						//--- Hendling user input in case of invalid move command ---//
						char command;
						std::cin >> command;

						if (command == 'Y' || command == 'y')
							_printHelp = true;
						else if (command == 'N' || command == 'n') {
							_invalidCommandInserted = false;
							_insertMoveCommand = true;
						}
					}
					else {
						//--- Hendling user input for menu command ---//
						char command;
						std::cin >> command;

						//--- Reset whole environmantr and start new game ---//
						if (command == 'N' || command == 'n') {
							_resetEnvironment = true;
						}
						//--- Allow for inserting move command ---//
						else if (command == 'M' || command == 'm') {
							_insertMoveCommand = true;
						}
						//--- Stop the application ---//
						else if (command == 'Q' || command == 'q') {
							Event_System::Application_Events::Application_Close_Event applicationCloseEvent(1000);
							_eventEmitter.Emit(applicationCloseEvent);
						}
					}
				}
				else {
					system("pause");
					_printHelp = false;
				}
				break;
			}
			//--- Application state - Ending state ---//
			case APP_STATES::END_STATE: {
				system("pause");
				Event_System::Application_Events::Application_Close_Event applicationCloseEvent(1000);
				_eventEmitter.Emit(applicationCloseEvent);
				break;
			}
			//--- Application state - Main menu state ---//
			default: {
				short unsigned int treeDepth;
				std::cin >> treeDepth;

				Event_System::Application_Events::Game_Tree_Setup_Event gameTreeSetupEvent(treeDepth);
				_eventEmitter.Emit(gameTreeSetupEvent);
				break;
			}
			}
		}
	}

	void Command_Line_UI::RenderLogo() const {
		std::cout << "															\n";
		std::cout << "		 .d8888b.  888										\n";
		std::cout << "		d88P  Y88b 888										\n";
		std::cout << "		888    888 888										\n";
		std::cout << "		888        88888b.   .d88b.  .d8888b  .d8888b		\n";
		std::cout << "		888        888 \"88b d8P  Y8b 88K      88K			\n";
		std::cout << "		888    888 888  888 88888888 \"Y8888b. \"Y8888b.	\n";
		std::cout << "		Y88b  d88P 888  888 Y8b.          X88      X88		\n";
		std::cout << "		 \"Y8888P\"  888  888  \"Y8888   88888P\'  88888P\'	\n";
		std::cout << "															\n";
	}

	//--- Method rendering chess board state ---//
	// IMPORTANT: Because entries in death list are of type 'int', it needs to be converted into proper 'char' befor printing.
	void Command_Line_UI::RenderBoard() const {
		std::cout << "Captured pieces (WHITE): ";
		//--- Converting 'int' piece ID into char for white player. ---//
		for (const auto& iterator : Pale::Chess_Logic::s_deathList) {	
			switch (iterator) {
			case 7: { std::cout << "K "; break; }
			case 5: { std::cout << "Q "; break; }
			case 4: { std::cout << "B "; break; }
			case 3: { std::cout << "N "; break; }
			case 2: { std::cout << "R "; break; }
			case 1: { std::cout << "P "; break; }
			}
		}

		std::cout << "\n\n";
		std::cout << "Captured pieces (BLACK): ";
		//--- Converting 'int' piece ID into char for black player. ---//
		for (const auto& iterator : Pale::Chess_Logic::s_deathList) {
			switch (iterator) {
			case -7: { std::cout << "K "; break; }
			case -5: { std::cout << "Q "; break; }
			case -4: { std::cout << "B "; break; }
			case -3: { std::cout << "N "; break; }
			case -2: { std::cout << "R "; break; }
			case -1: { std::cout << "P "; break; }
			}
		}

		//--- Printing when one site king is under check ---//
		if (_whiteKingCheck)
			std::cout << "\n\nWhite king check!";

		if (_blackKingCheck)
			std::cout << "\n\nBlack king check!";

		//--- Printing out board state. ---//
		// IMPORTANT: Method which create 'string' from board state was defined inside 'Board_Representation' class.
		std::cout << "\n\n" << *_boardRepresentation << "\n\n";
	}

	//--- Method rendering application main menu. It allows user to specify game tree search depth and start new game. ---// 
	void Command_Line_UI::RenderMainMenu() const {
		RenderLogo();
		std::cout << "Welcome in Chess AI program!\n\n How deep should the AI look for moves?\n Warning: values above 3 will be very slow. [n]? --> ";
	}

	//--- Method rendering game menu ---//
	// IMPORTANT: Because this method should only print stuff, it depends on copple of 'bool' variables which are manged by 'Update()' function.
	void Command_Line_UI::RenderGameMenu() const {
		try {
			if (!_printHelp) {
				//--- Standard behavior ---//
				switch (_numberOfAIInstances) {
				//--- Option for Player vs Player scenario. Both sites needs menu. ---//
				case 0: {
					if (_turnCounter % 2 == 0)
						std::cout << "Turn " << _turnCounter << "(BLACK)\n";
					else
						std::cout << "Turn " << _turnCounter << "(WHITE)\n";

					std::cout << "Commands: (N)ew game\t (M)ove\t (Q)uit\n\n";
					std::cout << "Insert command here --> ";
					break;
				}
				//--- Option for Player vs AI scenario. Only 1 site needs menu. ---//
				case 1: {
					if (_turnCounter % 2 == 0)
						std::cout << "Turn " << _turnCounter << "(BLACK)\n";
					else {
						std::cout << "Turn " << _turnCounter << "(WHITE)\n";

						std::cout << "Commands: (N)ew game\t (M)ove\t (Q)uit\n\n";
						std::cout << "Insert command here --> ";
					}
					break;
				}
				//--- Option for AI vs AI scenario. None menu needs to be printed. ---//
				case 2: {
					if (_turnCounter % 2 == 0)
						std::cout << "Turn " << _turnCounter << "(BLACK)\n";
					else
						std::cout << "Turn " << _turnCounter << "(WHITE)\n";

					//--- If set to 'true' user will have to press button for application to continue running ---//
					if (!FULLY_AUTOMATED)
						std::cout << "Continue?: ";
					break;
				}
				default: { throw PaleEngineException("Exception happened!", 'e', "Command_Line_UI.cpp", 20, "DrawGameMenu", UI_INVALID_AI_NUMBER); break; }
				}

				//--- Needs to be printed out if user will choose to insert move command ---//
				if (_insertMoveCommand)
					std::cout << "\nType '?' to see move command help. Insert command here --> ";

				//--- Allows to print out user help if invalid move command will be inserted ---//
				if (_invalidCommandInserted)
					std::cout << "\nYou gave incorrect move command!\nWould you like to see move command help?\t (Y)es\t (N)o: ";

				//--- By reseting the environment user can specify new game tree depth ---//
				if(_resetEnvironment)
					std::cout << "\nReseting the environment! How deep should the AI look for moves?\n Warning: values above 3 will be very slow. [n]? --> ";
			}
			//--- Print out user help if necessary ---//
			else
				RenderUserHelp();
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
		}
	}

	//--- Method rendering user help if invalid move command has been inserted ---//
	void Command_Line_UI::RenderUserHelp() const {
		system("cls");
		std::cout << "Typical move command: <figure><start position><end position>\n";
		std::cout << "\t figure := K - King, Q - Queen, B - Bishop, N - Knight, R - Rook, P - Pawn\n";
		std::cout << "\t start position := end position := a...h, 1...8\n\n";
		std::cout << "Special move commands: \n";
		std::cout << "\t 0-0 - Right castling,\n";
		std::cout << "\t 0-0-0 - Left castling,\n";
		std::cout << "\t P<start position>-><figure to change> - Promotion,\n";
		std::cout << "\t P<start position>x<x coordinate of end position> - En passant\n";
		std::cout << "\t\t x coordinate of end position := 1...8\n";
	}

	//--- Method rendering game result at the end game ---//
	// IMPORTANT: This should be the las application state. End state = application exit point ---//
	void Command_Line_UI::RenderEndstate() const {
		std::cout << "Game finished!\nThe winner is: ";
		//--- If game ended on white player turn (white checkmate) that's mean black player won ---//
		if (_turnCounter % 2 != 0)
			std::cout << "Dark Site.\n";
		//--- If game ended on black player turn (black checkmate) that's mean white player won ---//
		else
			std::cout << "White Site.\n";
	}
}