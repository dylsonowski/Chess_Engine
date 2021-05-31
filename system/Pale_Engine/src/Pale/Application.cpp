#include "palepch.h"
#include "Application.h"

namespace Pale {
	void Application::Run()	{
		LOG_SYSTEM_INIT;
		Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>> board(8, 8);
		//Pale::Chess_Logic::Board_Representation<int> intBoard = Pale::Chess_Logic::ConvertBoard<int>(board);
		//Pale::Chess_Logic::Board_Representation<std::string> stringBoard = Pale::Chess_Logic::ConvertBoard<std::string>(board);
		//Pale::Chess_Logic::MoveCommand command = Pale::Chess_Logic::ProcessMoveCommand("ike2e4");
		//board.SetPlateValue(1, 4, std::make_shared<Pale::Chess_Logic::Blank>(1, 4));
		std::cout << board << "\n";
		board.MovePiece(Pale::Chess_Logic::MoveCommand(), *board.GetPlateValue(0, 4));
		std::cout << board << "\n";
		while (1);
	}
}