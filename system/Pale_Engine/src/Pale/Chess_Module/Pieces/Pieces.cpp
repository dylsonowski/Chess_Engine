#include "palepch.h"
#include "Pieces.h"

bool Pale::Chess_Logic::KingIsChecked(std::vector<std::vector<std::shared_ptr<Pieces>>> board, OWNERS whichTurn) {
	try {
		bool check = false, kingFound = false;
		std::pair<unsigned int, unsigned int> kingPos;
		for (int rowIt = 0; rowIt < board.size(); rowIt++) {
			for (int columnIt = 0; columnIt < board.at(0).size(); columnIt++) {
				if (whichTurn == OWNERS::BLACK && board.at(rowIt).at(columnIt)->GetValue() == -7) {
					kingPos = std::make_pair(rowIt, columnIt);
					kingFound = true;
				}
				else if (whichTurn == OWNERS::WHITE && board.at(rowIt).at(columnIt)->GetValue() == 7) {
					kingPos = std::make_pair(rowIt, columnIt);
					kingFound = true;
				}
			}
		}
		return check;
	}
	catch (PaleEngineException& exception) {
		if (exception.GetType() == 'e')
			PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		else if (exception.GetType() == 'w')
			PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

		std::cin.get();
	}
}
