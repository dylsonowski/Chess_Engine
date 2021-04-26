#include "palepch.h"
/*#include "Board_Representation.h"

namespace Pale {
	namespace Chess_Logic {
		//template class Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>>;
		//template class Board_Representation<int>;
		//template class Board_Representation<std::string>;

		template<typename T>
		Board_Representation<T>::Board_Representation(unsigned int row, unsigned int column) //todo: Make it diiferent way (first representation = object, object define value and start cords)
			: _row(row), _column(column), _representationType(BOARD_TYPE::BASE) {
			//--- Piece starting positions :== <piece name>.<black = first, white = second>.<copy number>.<row cord = first, column cord = second> ---//
			for (int rowIt = 0; rowIt < row; rowIt++) {
				std::vector<std::shared_ptr<Pieces>> temp;
				for (int columnIt = 0; columnIt < column; columnIt++) {
					if (rowIt == Piece_Starting_Positions::m_pawnStartPos.first.at(0).first) //If <pawn_start_cords>.<black>.X.<row cord>
						temp.emplace_back(std::make_shared<Pawn>(PIECE_OWNER::BLACK, columnIt));
					else if (rowIt == Piece_Starting_Positions::m_pawnStartPos.second.at(0).first) //If <pawn_start_cords>.<white>.X.<row cord>
						temp.emplace_back(std::make_shared<Pawn>(PIECE_OWNER::WHITE, columnIt));
					else
						temp.emplace_back(std::make_shared<Blank>(rowIt, columnIt));
				}
				_board.emplace_back(temp);
			}

			//--- Placing black rook ---//
			_board.at(Piece_Starting_Positions::m_rookStartPos.first.at(0).first).at(Piece_Starting_Positions::m_rookStartPos.first.at(0).second) 
				= std::make_shared<Rook>(PIECE_OWNER::BLACK, 0);
			_board.at(Piece_Starting_Positions::m_rookStartPos.first.at(1).first).at(Piece_Starting_Positions::m_rookStartPos.first.at(1).second) 
				= std::make_shared<Rook>(PIECE_OWNER::BLACK, 1);

			//--- Placing white rook ---//
			_board.at(Piece_Starting_Positions::m_rookStartPos.second.at(0).first).at(Piece_Starting_Positions::m_rookStartPos.second.at(0).second)
				= std::make_shared<Rook>(PIECE_OWNER::WHITE, 0);
			_board.at(Piece_Starting_Positions::m_rookStartPos.second.at(1).first).at(Piece_Starting_Positions::m_rookStartPos.second.at(1).second)
				= std::make_shared<Rook>(PIECE_OWNER::WHITE, 1);

			//--- Placing black knight ---//
			_board.at(Piece_Starting_Positions::m_knightStartPos.first.at(0).first).at(Piece_Starting_Positions::m_knightStartPos.first.at(0).second)
				= std::make_shared<Knight>(PIECE_OWNER::BLACK, 0);
			_board.at(Piece_Starting_Positions::m_knightStartPos.first.at(1).first).at(Piece_Starting_Positions::m_knightStartPos.first.at(1).second)
				= std::make_shared<Knight>(PIECE_OWNER::BLACK, 1);

			//--- Placing white knight ---//
			_board.at(Piece_Starting_Positions::m_knightStartPos.second.at(0).first).at(Piece_Starting_Positions::m_knightStartPos.second.at(0).second)
				= std::make_shared<Knight>(PIECE_OWNER::WHITE, 0);
			_board.at(Piece_Starting_Positions::m_knightStartPos.second.at(1).first).at(Piece_Starting_Positions::m_knightStartPos.second.at(1).second)
				= std::make_shared<Knight>(PIECE_OWNER::WHITE, 1);

			//--- Placing black bishop ---//
			_board.at(Piece_Starting_Positions::m_bishopStartPos.first.at(0).first).at(Piece_Starting_Positions::m_bishopStartPos.first.at(0).second)
				= std::make_shared<Bishop>(PIECE_OWNER::BLACK, 0);
			_board.at(Piece_Starting_Positions::m_bishopStartPos.first.at(1).first).at(Piece_Starting_Positions::m_bishopStartPos.first.at(1).second)
				= std::make_shared<Bishop>(PIECE_OWNER::BLACK, 1);

			//--- Placing white bishop ---//
			_board.at(Piece_Starting_Positions::m_bishopStartPos.second.at(0).first).at(Piece_Starting_Positions::m_bishopStartPos.second.at(0).second)
				= std::make_shared<Bishop>(PIECE_OWNER::WHITE, 0);
			_board.at(Piece_Starting_Positions::m_bishopStartPos.second.at(1).first).at(Piece_Starting_Positions::m_bishopStartPos.second.at(1).second)
				= std::make_shared<Bishop>(PIECE_OWNER::WHITE, 1);

			//--- Placing black queen ---//
			_board.at(Piece_Starting_Positions::m_queenStartPos.first.at(0).first).at(Piece_Starting_Positions::m_queenStartPos.first.at(0).second)
				= std::make_shared<Queen>(PIECE_OWNER::BLACK, 0);

			//--- Placing white queen ---//
			_board.at(Piece_Starting_Positions::m_queenStartPos.second.at(0).first).at(Piece_Starting_Positions::m_queenStartPos.second.at(0).second)
				= std::make_shared<Queen>(PIECE_OWNER::WHITE, 0);

			//--- Placing black king ---//
			_board.at(Piece_Starting_Positions::m_kingStartPos.first.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.first.at(0).second)
				= std::make_shared<King>(PIECE_OWNER::BLACK, 0);

			//--- Placing white king ---//
			_board.at(Piece_Starting_Positions::m_kingStartPos.second.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.second.at(0).second)
				= std::make_shared<King>(PIECE_OWNER::WHITE, 0);
		}

		template<typename T>
		inline void Board_Representation<T>::SetPlateValue(unsigned int rowIt, unsigned int columnIt, T value) {
			try {
				if (rowIt >= _row || columnIt >= _column)
					throw std::make_pair('e', VEC_OUT_OF_RANGE);

				switch (_representationType) {
				case BOARD_TYPE::INT_TYPE:
					if (typeid(value) != typeid(int))
						throw std::make_pair('e', BAD_TYPE_INSERTION);

					if (value > 7 || value == 6 || value < -7 || value == -6)
						throw std::make_pair('w', INVALID_PIECE_ID);

					_board.at(rowIt).at(columnIt) = value;
					break;

				case BOARD_TYPE::STRING_TYPE:
					if (typeid(value) != typeid(std::string))
						throw std::make_pair('e', BAD_TYPE_INSERTION);

					if(value != "bP" && value != "bR" && value != "bN" && value != "bB" && value != "bQ" && value != "bK" 
						&& value != "wP" && value != "wR" && value != "wN" && value != "wB" && value != "wQ" && value != "wK" && value != "x")
						throw std::make_pair('w', INVALID_PIECE_ID);

					_board.at(rowIt).at(columnIt) = value;
					break;

				default:
					if(typeid(value) != typeid(std::shared_ptr<Pawn>) && typeid(value) != typeid(std::shared_ptr<Rook>) && typeid(value) != typeid(std::shared_ptr<Knight>) 
						&& typeid(value) != typeid(std::shared_ptr<Bishop>) && typeid(value) != typeid(std::shared_ptr<Queen>) && typeid(value) != typeid(std::shared_ptr<King>) 
						&& typeid(value) != typeid(std::shared_ptr<Blank>))
						throw std::make_pair('e', BAD_TYPE_INSERTION);

					_board.at(rowIt).at(columnIt) = value;
					break;
				}
			}
			catch (std::pair<char, std::string> eventMessage) { 
				if (eventMessage.first == 'e')
					PALE_ENGINE_ERROR(eventMessage.second);
				else if (eventMessage.first == 'w')
					PALE_ENGINE_WARN(eventMessage.second);

				std::cin.get();
			}
		}

		template<typename T>
		inline T Board_Representation<T>::GetPlateValue(unsigned int rowIt, unsigned int columnIt) const {
			try {
				if (rowIt < _row && columnIt < _column)
					return _board.at(rowIt).at(columnIt);
				else
					throw VEC_OUT_OF_RANGE;
			}
			catch (std::string errorMessage) { PALE_ENGINE_ERROR(errorMessage); std::cin.get(); }
		}
	}
}*/