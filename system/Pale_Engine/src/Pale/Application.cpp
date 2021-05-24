#include "palepch.h"
#include "Application.h"

namespace Pale {
	void Application::Run()	{
		LOG_SYSTEM_INIT;
		Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>> board(8, 8);
		Pale::Chess_Logic::Board_Representation<int> intBoard = Pale::Chess_Logic::ConvertBoard<int>(board);
		Pale::Chess_Logic::Board_Representation<std::string> stringBoard = Pale::Chess_Logic::ConvertBoard<std::string>(board);
		while (1);
	}
}