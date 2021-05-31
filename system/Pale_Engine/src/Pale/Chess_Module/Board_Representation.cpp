#include "palepch.h"
#include "Board_Representation.h"

namespace Pale {
	namespace Chess_Logic {
		template<>
		Board_Representation<std::shared_ptr<Pieces>>::Board_Representation(unsigned int row, unsigned int column)
			: _row(row), _column(column), _representationType(BOARD_TYPE::OBJECT_TYPE) {
			//--- Piece starting positions :== <piece name>.<black = first, white = second>.<copy number>.<row cord = first, column cord = second> ---//
			for (int rowIt = 0; rowIt < row; rowIt++) { //Fill up board with pawns and blanks
				std::vector<std::shared_ptr<Pieces>> temp;
				for (int columnIt = 0; columnIt < column; columnIt++) {
					if (rowIt == Piece_Starting_Positions::m_pawnStartPos.first.at(0).first) //If <pawn_start_cords>.<black>.X.<row cord>
						temp.emplace_back(std::make_shared<Pawn>(OWNERS::BLACK, columnIt));
					else if (rowIt == Piece_Starting_Positions::m_pawnStartPos.second.at(0).first) //If <pawn_start_cords>.<white>.X.<row cord>
						temp.emplace_back(std::make_shared<Pawn>(OWNERS::WHITE, columnIt));
					else
						temp.emplace_back(std::make_shared<Blank>(rowIt, columnIt)); //Else blank plate
				}
				_board.emplace_back(temp);
			}

			//--- Placing black rook ---//
			_board.at(Piece_Starting_Positions::m_rookStartPos.first.at(0).first).at(Piece_Starting_Positions::m_rookStartPos.first.at(0).second)
				= std::make_shared<Rook>(OWNERS::BLACK, 0);
			_board.at(Piece_Starting_Positions::m_rookStartPos.first.at(1).first).at(Piece_Starting_Positions::m_rookStartPos.first.at(1).second)
				= std::make_shared<Rook>(OWNERS::BLACK, 1);

			//--- Placing white rook ---//
			_board.at(Piece_Starting_Positions::m_rookStartPos.second.at(0).first).at(Piece_Starting_Positions::m_rookStartPos.second.at(0).second)
				= std::make_shared<Rook>(OWNERS::WHITE, 0);
			_board.at(Piece_Starting_Positions::m_rookStartPos.second.at(1).first).at(Piece_Starting_Positions::m_rookStartPos.second.at(1).second)
				= std::make_shared<Rook>(OWNERS::WHITE, 1);

			//--- Placing black knight ---//
			_board.at(Piece_Starting_Positions::m_knightStartPos.first.at(0).first).at(Piece_Starting_Positions::m_knightStartPos.first.at(0).second)
				= std::make_shared<Knight>(OWNERS::BLACK, 0);
			_board.at(Piece_Starting_Positions::m_knightStartPos.first.at(1).first).at(Piece_Starting_Positions::m_knightStartPos.first.at(1).second)
				= std::make_shared<Knight>(OWNERS::BLACK, 1);

			//--- Placing white knight ---//
			_board.at(Piece_Starting_Positions::m_knightStartPos.second.at(0).first).at(Piece_Starting_Positions::m_knightStartPos.second.at(0).second)
				= std::make_shared<Knight>(OWNERS::WHITE, 0);
			_board.at(Piece_Starting_Positions::m_knightStartPos.second.at(1).first).at(Piece_Starting_Positions::m_knightStartPos.second.at(1).second)
				= std::make_shared<Knight>(OWNERS::WHITE, 1);

			//--- Placing black bishop ---//
			_board.at(Piece_Starting_Positions::m_bishopStartPos.first.at(0).first).at(Piece_Starting_Positions::m_bishopStartPos.first.at(0).second)
				= std::make_shared<Bishop>(OWNERS::BLACK, 0);
			_board.at(Piece_Starting_Positions::m_bishopStartPos.first.at(1).first).at(Piece_Starting_Positions::m_bishopStartPos.first.at(1).second)
				= std::make_shared<Bishop>(OWNERS::BLACK, 1);

			//--- Placing white bishop ---//
			_board.at(Piece_Starting_Positions::m_bishopStartPos.second.at(0).first).at(Piece_Starting_Positions::m_bishopStartPos.second.at(0).second)
				= std::make_shared<Bishop>(OWNERS::WHITE, 0);
			_board.at(Piece_Starting_Positions::m_bishopStartPos.second.at(1).first).at(Piece_Starting_Positions::m_bishopStartPos.second.at(1).second)
				= std::make_shared<Bishop>(OWNERS::WHITE, 1);

			//--- Placing black queen ---//
			_board.at(Piece_Starting_Positions::m_queenStartPos.first.at(0).first).at(Piece_Starting_Positions::m_queenStartPos.first.at(0).second)
				= std::make_shared<Queen>(OWNERS::BLACK, 0);

			//--- Placing white queen ---//
			_board.at(Piece_Starting_Positions::m_queenStartPos.second.at(0).first).at(Piece_Starting_Positions::m_queenStartPos.second.at(0).second)
				= std::make_shared<Queen>(OWNERS::WHITE, 0);

			//--- Placing black king ---//
			_board.at(Piece_Starting_Positions::m_kingStartPos.first.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.first.at(0).second)
				= std::make_shared<King>(OWNERS::BLACK, 0);

			//--- Placing white king ---//
			_board.at(Piece_Starting_Positions::m_kingStartPos.second.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.second.at(0).second)
				= std::make_shared<King>(OWNERS::WHITE, 0);

			PALE_ENGINE_INFO("Board was successfully created! Board type: Object.");
		}

		template<>
		Board_Representation<int>::Board_Representation(unsigned int row, unsigned int column)
			: _row(row), _column(column), _representationType(BOARD_TYPE::INT_TYPE) {
			//--- Piece starting positions :== <piece name>.<black = first, white = second>.<copy number>.<row cord = first, column cord = second> ---//
			for (int rowIt = 0; rowIt < row; rowIt++) { //Fill up board with pawns and blanks
				std::vector<int> temp;
				for (int columnIt = 0; columnIt < column; columnIt++) {
					if (rowIt == Piece_Starting_Positions::m_pawnStartPos.first.at(0).first) //If <pawn_start_cords>.<black>.X.<row cord>
						temp.emplace_back(-1);
					else if (rowIt == Piece_Starting_Positions::m_pawnStartPos.second.at(0).first) //If <pawn_start_cords>.<white>.X.<row cord>
						temp.emplace_back(1);
					else
						temp.emplace_back(0); //Else blank plate
				}
				_board.emplace_back(temp);
			}

			//--- Placing black rooks ---//
			_board.at(Piece_Starting_Positions::m_rookStartPos.first.at(0).first).at(Piece_Starting_Positions::m_rookStartPos.first.at(0).second) =
				_board.at(Piece_Starting_Positions::m_rookStartPos.first.at(1).first).at(Piece_Starting_Positions::m_rookStartPos.first.at(1).second) = -2;
			//--- Placing black knights ---//
			_board.at(Piece_Starting_Positions::m_knightStartPos.first.at(0).first).at(Piece_Starting_Positions::m_knightStartPos.first.at(0).second) =
				_board.at(Piece_Starting_Positions::m_knightStartPos.first.at(1).first).at(Piece_Starting_Positions::m_knightStartPos.first.at(1).second) = -3;
			//--- Placing black bishops ---//
			_board.at(Piece_Starting_Positions::m_bishopStartPos.first.at(0).first).at(Piece_Starting_Positions::m_bishopStartPos.first.at(0).second) =
				_board.at(Piece_Starting_Positions::m_bishopStartPos.first.at(1).first).at(Piece_Starting_Positions::m_bishopStartPos.first.at(1).second) = -4;
			//--- Placing black queen ---//
			_board.at(Piece_Starting_Positions::m_queenStartPos.first.at(0).first).at(Piece_Starting_Positions::m_queenStartPos.first.at(0).second) = -5;
			//--- Placing black king ---//
			_board.at(Piece_Starting_Positions::m_kingStartPos.first.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.first.at(0).second) = -7;

			//--- Placing while rooks ---//
			_board.at(Piece_Starting_Positions::m_rookStartPos.second.at(0).first).at(Piece_Starting_Positions::m_rookStartPos.second.at(0).second) =
				_board.at(Piece_Starting_Positions::m_rookStartPos.second.at(1).first).at(Piece_Starting_Positions::m_rookStartPos.second.at(1).second) = 2;
			//--- Placing while knights ---//
			_board.at(Piece_Starting_Positions::m_knightStartPos.second.at(0).first).at(Piece_Starting_Positions::m_knightStartPos.second.at(0).second) =
				_board.at(Piece_Starting_Positions::m_knightStartPos.second.at(1).first).at(Piece_Starting_Positions::m_knightStartPos.second.at(1).second) = 3;
			//--- Placing white bishops ---//
			_board.at(Piece_Starting_Positions::m_bishopStartPos.second.at(0).first).at(Piece_Starting_Positions::m_bishopStartPos.second.at(0).second) =
				_board.at(Piece_Starting_Positions::m_bishopStartPos.second.at(1).first).at(Piece_Starting_Positions::m_bishopStartPos.second.at(1).second) = 4;
			//--- Placing white queen ---//
			_board.at(Piece_Starting_Positions::m_queenStartPos.second.at(0).first).at(Piece_Starting_Positions::m_queenStartPos.second.at(0).second) = 5;
			//--- Placing white king ---//
			_board.at(Piece_Starting_Positions::m_kingStartPos.second.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.second.at(0).second) = 7;

			PALE_ENGINE_INFO("Board was successfully created! Board type: Int.");
		}

		template<>
		Board_Representation<std::string>::Board_Representation(unsigned int row, unsigned int column)
			: _row(row), _column(column), _representationType(BOARD_TYPE::STRING_TYPE) {
			//--- Piece starting positions :== <piece name>.<black = first, white = second>.<copy number>.<row cord = first, column cord = second> ---//
			for (int rowIt = 0; rowIt < row; rowIt++) { //Fill up board with pawns and blanks
				std::vector<std::string> temp;
				for (int columnIt = 0; columnIt < column; columnIt++) {
					if (rowIt == Piece_Starting_Positions::m_pawnStartPos.first.at(0).first) //If <pawn_start_cords>.<black>.X.<row cord>
						temp.emplace_back("bP");
					else if (rowIt == Piece_Starting_Positions::m_pawnStartPos.second.at(0).first) //If <pawn_start_cords>.<white>.X.<row cord>
						temp.emplace_back("wP");
					else
						temp.emplace_back("x"); //Else blank plate
				}
				_board.emplace_back(temp);
			}

			//--- Placing black rooks ---//
			_board.at(Piece_Starting_Positions::m_rookStartPos.first.at(0).first).at(Piece_Starting_Positions::m_rookStartPos.first.at(0).second) =
				_board.at(Piece_Starting_Positions::m_rookStartPos.first.at(1).first).at(Piece_Starting_Positions::m_rookStartPos.first.at(1).second) = "bR";
			//--- Placing black knights ---//
			_board.at(Piece_Starting_Positions::m_knightStartPos.first.at(0).first).at(Piece_Starting_Positions::m_knightStartPos.first.at(0).second) =
				_board.at(Piece_Starting_Positions::m_knightStartPos.first.at(1).first).at(Piece_Starting_Positions::m_knightStartPos.first.at(1).second) = "bN";
			//--- Placing black bishops ---//
			_board.at(Piece_Starting_Positions::m_bishopStartPos.first.at(0).first).at(Piece_Starting_Positions::m_bishopStartPos.first.at(0).second) =
				_board.at(Piece_Starting_Positions::m_bishopStartPos.first.at(1).first).at(Piece_Starting_Positions::m_bishopStartPos.first.at(1).second) = "bB";
			//--- Placing black queen ---//
			_board.at(Piece_Starting_Positions::m_queenStartPos.first.at(0).first).at(Piece_Starting_Positions::m_queenStartPos.first.at(0).second) = "bQ";
			//--- Placing black king ---//
			_board.at(Piece_Starting_Positions::m_kingStartPos.first.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.first.at(0).second) = "bK";

			//--- Placing while rooks ---//
			_board.at(Piece_Starting_Positions::m_rookStartPos.second.at(0).first).at(Piece_Starting_Positions::m_rookStartPos.second.at(0).second) =
				_board.at(Piece_Starting_Positions::m_rookStartPos.second.at(1).first).at(Piece_Starting_Positions::m_rookStartPos.second.at(1).second) = "wR";
			//--- Placing while knights ---//
			_board.at(Piece_Starting_Positions::m_knightStartPos.second.at(0).first).at(Piece_Starting_Positions::m_knightStartPos.second.at(0).second) =
				_board.at(Piece_Starting_Positions::m_knightStartPos.second.at(1).first).at(Piece_Starting_Positions::m_knightStartPos.second.at(1).second) = "wN";
			//--- Placing white bishops ---//
			_board.at(Piece_Starting_Positions::m_bishopStartPos.second.at(0).first).at(Piece_Starting_Positions::m_bishopStartPos.second.at(0).second) =
				_board.at(Piece_Starting_Positions::m_bishopStartPos.second.at(1).first).at(Piece_Starting_Positions::m_bishopStartPos.second.at(1).second) = "wB";
			//--- Placing white queen ---//
			_board.at(Piece_Starting_Positions::m_queenStartPos.second.at(0).first).at(Piece_Starting_Positions::m_queenStartPos.second.at(0).second) = "wQ";
			//--- Placing white king ---//
			_board.at(Piece_Starting_Positions::m_kingStartPos.second.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.second.at(0).second) = "wK";

			PALE_ENGINE_INFO("Board was successfully created! Board type: String.");
		}

		template<>
		Board_Representation<std::shared_ptr<Pieces>> ConvertBoard(const Board_Representation<std::shared_ptr<Pieces>>& board) {
			Board_Representation<std::shared_ptr<Pieces>> newBoard(board.GetRowNumber(), board.GetColumnNumber());
			for (int rowIt = 0; rowIt < board.GetRowNumber(); rowIt++) {
				for (int columnIt = 0; columnIt < board.GetColumnNumber(); columnIt++) {
					newBoard.SetPlateValue(rowIt, columnIt, board.GetPlateValue(rowIt, columnIt));
				}
			}
			PALE_ENGINE_TRACE("No convertion made!");
			return newBoard;
		}

		template<>
		Board_Representation<int> ConvertBoard(const Board_Representation<std::shared_ptr<Pieces>>& board) {
			Board_Representation<int> newBoard(board.GetRowNumber(), board.GetColumnNumber());
			for (int rowIt = 0; rowIt < board.GetRowNumber(); rowIt++) {
				for (int columnIt = 0; columnIt < board.GetColumnNumber(); columnIt++) {
					newBoard.SetPlateValue(rowIt, columnIt, board.GetPlateValue(rowIt, columnIt)->GetValue());
				}
			}
			PALE_ENGINE_TRACE("Convertion on int type!");
			return newBoard;
		}

		template<>
		Board_Representation<std::string> ConvertBoard(const Board_Representation<std::shared_ptr<Pieces>>& board) {
			Board_Representation<std::string> newBoard(board.GetRowNumber(), board.GetColumnNumber());
			for (int rowIt = 0; rowIt < board.GetRowNumber(); rowIt++) {
				for (int columnIt = 0; columnIt < board.GetColumnNumber(); columnIt++) {
					newBoard.SetPlateValue(rowIt, columnIt, board.GetPlateValue(rowIt, columnIt)->GetName());
				}
			}
			PALE_ENGINE_TRACE("Convertion on string type!");
			return newBoard;
		}

		template<>
		void Board_Representation<std::shared_ptr<Pieces>>::SetPlateValue(unsigned int rowIt, unsigned int columnIt, std::shared_ptr<Pieces> value) {
			try {
				if (rowIt >= _row || columnIt >= _column)
					throw PaleEngineException("Exception happened!", 'e', "Board_Representation.cpp", 214, "SetPlateValue", VEC_OUT_OF_RANGE);

				if (_representationType != BOARD_TYPE::OBJECT_TYPE && _representationType != BOARD_TYPE::BASE)
					throw PaleEngineException("Exception happened!", 'e', "Board_Representation.cpp", 217, "SetPlateValue", BAD_TYPE_INSERTION);

				_board.at(rowIt).at(columnIt) = value;
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
			}
		}

		template<>
		void Board_Representation<int>::SetPlateValue(unsigned int rowIt, unsigned int columnIt, int value) {
			try {
				if (rowIt >= _row || columnIt >= _column)
					throw PaleEngineException("Exception happened!", 'e', "Board_Representation.cpp", 235, "SetPlateValue", VEC_OUT_OF_RANGE);

				if (_representationType != BOARD_TYPE::INT_TYPE)
					throw PaleEngineException("Exception happened!", 'e', "Board_Representation.cpp", 238, "SetPlateValue", BAD_TYPE_INSERTION);

				if (value > 7 || value == 6 || value < -7 || value == -6)
					throw PaleEngineException("Exception happened!", 'w', "Board_representation.cpp", 241, "SetPlateValue", INVALID_PIECE_ID);

				_board.at(rowIt).at(columnIt) = value;
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
			}
		}

		template<>
		void Board_Representation<std::string>::SetPlateValue(unsigned int rowIt, unsigned int columnIt, std::string value) {
			try {
				if (rowIt >= _row || columnIt >= _column)
					throw PaleEngineException("Exception happened!", 'e', "Board_Representation.cpp", 259, "SetPlateValue", VEC_OUT_OF_RANGE);

				if (_representationType != BOARD_TYPE::STRING_TYPE)
					throw PaleEngineException("Exception happened!", 'e', "Board_Representation.cpp", 262, "SetPlateValue", BAD_TYPE_INSERTION);

				if (value != "bP" && value != "bR" && value != "bN" && value != "bB" && value != "bQ" && value != "bK"
					&& value != "wP" && value != "wR" && value != "wN" && value != "wB" && value != "wQ" && value != "wK" && value != "x")
					throw PaleEngineException("Exception happened!", 'w', "Board_representation.cpp", 266, "SetPlateValue", INVALID_PIECE_ID);

				_board.at(rowIt).at(columnIt) = value;
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
			}
		}
	}
}
