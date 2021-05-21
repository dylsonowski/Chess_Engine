#include "palepch.h"
#include "Application.h"

namespace Pale {
	void Application::Run()	{
		LOG_SYSTEM_INIT;
		Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>> board(8, 8, Pale::Chess_Logic::BOARD_TYPE::OBJECT_TYPE);
		//Pale::Chess_Logic::Board_Representation<int> intBoard = board;
		//Pale::Chess_Logic::Board_Representation<std::string> stringBoard = board;
		while (1);
	}
}