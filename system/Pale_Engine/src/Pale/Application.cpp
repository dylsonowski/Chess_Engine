#include "palepch.h"
#include "Application.h"

namespace Pale {
	void Application::Run()	{
		LOG_SYSTEM_INIT;
		Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>> board(8, 8, false);
		std::cout << board << "\n\n";

		Pale::Chess_Logic::Board_Representation<int> intBoard = Pale::Chess_Logic::ConvertBoard<int>(board);
		std::cout << intBoard << "\n\n";

		Pale::Chess_Logic::Board_Representation<std::string> stringBoard = Pale::Chess_Logic::ConvertBoard<std::string>(board);
		std::cout << stringBoard << "\n\n";

		Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>> objectBoard = Pale::Chess_Logic::ConvertBoard<std::shared_ptr<Pale::Chess_Logic::Pieces>>(stringBoard);
		std::cout << objectBoard << "\n\n";


		Move_Command commandCorrect = Pale::Chess_Logic::ProcessMoveCommand("Nb8a6", OWNERS::BLACK, board);
		//Move_Command commandWrong = Pale::Chess_Logic::ProcessMoveCommand("ike2e4", OWNERS::WHITE, board);


		board.SetPlateValue(1, 4, std::make_shared<Pale::Chess_Logic::Blank>(1, 4));
		std::cout << board << "\n\n";

		Move_Command command = Pale::Chess_Logic::ProcessMoveCommand("Pa7a5", OWNERS::BLACK, board);
		bool canMove = board.MovePiece(command, *board.GetPlateValue(1, 0));
		std::cout << board << "\n\n";

		Move_Command commandBad = Pale::Chess_Logic::ProcessMoveCommand("Nb8d8", OWNERS::BLACK, board);
		bool canMoveBad = board.MovePiece(commandBad, *board.GetPlateValue(1, 0));
		std::cout << board << "\n\n";

		std::vector<Move_Command> movesFirst = board.GetPlateValue(0, 1)->GenerateLegitMoves(board.GetBoardRef());
		std::vector<Move_Command> movesSecond = board.GetPlateValue(0, 3)->GenerateLegitMoves(board.GetBoardRef());
		while (1);
	}
}