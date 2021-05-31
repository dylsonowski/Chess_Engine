#include "palepch.h"
#include "Pieces.h"

namespace Pale {
	namespace Chess_Logic {
		void Promotion::Execute() {
			/*switch (std::toupper(_newPiece)) {
			case 'Q':
				std::shared_ptr<Queen> newPiece = std::make_shared<Queen>(_boardRef.at(_startPos.first).at(_startPos.second)->GetOwner(), _endPos);
				_boardRef.at(_endPos.first).at(_endPos.second) = newPiece;
				break;

			case 'B':
				std::shared_ptr<Bishop> newPiece = std::make_shared<Bishop>(_boardRef.at(_startPos.first).at(_startPos.second)->GetOwner(), _endPos);
				_boardRef.at(_endPos.first).at(_endPos.second) = newPiece;
				break;

			case 'N':
				std::shared_ptr<Knight> newPiece = std::make_shared<Knight>(_boardRef.at(_startPos.first).at(_startPos.second)->GetOwner(), _endPos);
				_boardRef.at(_endPos.first).at(_endPos.second) = newPiece;
				break;

			case 'R':
				std::shared_ptr<Rook> newPiece = std::make_shared<Rook>(_boardRef.at(_startPos.first).at(_startPos.second)->GetOwner(), _endPos);
				_boardRef.at(_endPos.first).at(_endPos.second) = newPiece;
				break;
			}

			_boardRef.at(_startPos.first).at(_startPos.second) = std::make_shared<Blank>(_startPos.first, _startPos.second);*/
		}

		void Castling::Execute() {
			//_boardRef.at(_kingPos.first).at(_kingPos.second).swap(_boardRef.at(_rookPos.first).at(_rookPos.second));
		}

		void En_Passant::Execute() {
			/*_boardRef.at(_startPos.first).at(_startPos.second).swap(_boardRef.at(_endPos.first).at(_endPos.second));
			if (_whichTurn == OWNERS::BLACK) {
				Board_Representation<>AddToDeathList(_boardRef.at(_endPos.first - 1).at(_endPos.second)->GetValue());
				_boardRef.at(_endPos.first - 1).at(_endPos.second) = std::make_shared<Blank>(_endPos.first - 1, _endPos.second);
			}
			else {
				AddToDeathList(_boardRef.at(_endPos.first + 1).at(_endPos.second)->GetValue());
				_boardRef.at(_endPos.first + 1).at(_endPos.second) = std::make_shared<Blank>(_endPos.first + 1, _endPos.second);
			}*/
		}
	}
}