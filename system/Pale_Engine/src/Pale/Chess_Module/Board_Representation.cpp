#include "palepch.h"
#include "Board_Representation.h"

namespace Pale::Chess_Logic {
	//--- Set of constructors defined by object type ---//
	template<>
	Board_Representation<std::shared_ptr<Pieces>>::Board_Representation(unsigned int row, unsigned int column, bool blankBoard)
		: _row(row), _column(column), _representationType(BOARD_TYPE::OBJECT_TYPE) {
		//--- Piece starting positions :== <piece name>.<black = first, white = second>.<copy number>.<row cord = first, column cord = second> ---//
		if (!blankBoard) {
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
		}
		else {
			for (int rowIt = 0; rowIt < row; rowIt++) { //Fill up board with blanks
				std::vector<std::shared_ptr<Pieces>> temp;
				for (int columnIt = 0; columnIt < column; columnIt++) {
					temp.emplace_back(std::make_shared<Blank>(rowIt, columnIt));
				}
				_board.emplace_back(temp);
			}
		}

		PALE_ENGINE_INFO("Board was successfully created! Board type: Object.");
	}

	template<>
	Board_Representation<int>::Board_Representation(unsigned int row, unsigned int column, bool blankBoard)
		: _row(row), _column(column), _representationType(BOARD_TYPE::INT_TYPE) {
		//--- Piece starting positions :== <piece name>.<black = first, white = second>.<copy number>.<row cord = first, column cord = second> ---//
		if (!blankBoard) {
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
		}
		else {
			for (int rowIt = 0; rowIt < row; rowIt++) { //Fill up board with blanks
				std::vector<int> temp;
				for (int columnIt = 0; columnIt < column; columnIt++) {
					temp.emplace_back(0);
				}
				_board.emplace_back(temp);
			}
		}

		PALE_ENGINE_INFO("Board was successfully created! Board type: Int.");
	}

	template<>
	Board_Representation<std::string>::Board_Representation(unsigned int row, unsigned int column, bool blankBoard)
		: _row(row), _column(column), _representationType(BOARD_TYPE::STRING_TYPE) {
		//--- Piece starting positions :== <piece name>.<black = first, white = second>.<copy number>.<row cord = first, column cord = second> ---//
		if (!blankBoard) {
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
		}
		else {
			for (int rowIt = 0; rowIt < row; rowIt++) { //Fill up board with blanks
				std::vector<std::string> temp;
				for (int columnIt = 0; columnIt < column; columnIt++) {
					temp.emplace_back("x");
				}
				_board.emplace_back(temp);
			}
		}

		PALE_ENGINE_INFO("Board was successfully created! Board type: String.");
	}

	//--- Set of ToString functions defined by object type ---//
	template<>
	std::string Board_Representation<std::shared_ptr<Pieces>>::ToString() const {
		std::stringstream ss;
		int yLable = 9;
		char xLabel = 'a';

		for (int rowIt = 0; rowIt < _row; rowIt++) {
			for (int columnIt = 0; columnIt < _column; columnIt++) {
				if (columnIt == 0)
					ss << "    --  ";
				else
					ss << " --  ";
			}

			ss << "\n";
			ss << yLable-- << " ";
			for (int columnIt = 0; columnIt < _column; columnIt++) {
				if (columnIt == _column - 1) {
					if (_board.at(rowIt).at(columnIt)->GetValue() == 0)
						ss << "|    |";
					else
						ss << "| " << _board.at(rowIt).at(columnIt)->GetName() << " |";
				}
				else {
					if (_board.at(rowIt).at(columnIt)->GetValue() == 0)
						ss << "|    ";
					else
						ss << "| " << _board.at(rowIt).at(columnIt)->GetName() << " ";
				}
			}

			ss << "\n";
			if (rowIt == _row - 1) {
				for (int columnIt = 0; columnIt < _column; columnIt++) {
					if (columnIt == 0)
						ss << "    --  ";
					else
						ss << " --  ";
				}
			}
		}

		ss << "\n  ";
		for (int columnIt = 0; columnIt < _column; columnIt++) {
			ss << "  " << xLabel << "  ";
			xLabel++;
		}
		return ss.str();
	}

	template<>
	std::string Board_Representation<int>::ToString() const {
		std::stringstream ss;
		int yLable = 9;
		char xLabel = 'a';

		for (int rowIt = 0; rowIt < _row; rowIt++) {
			for (int columnIt = 0; columnIt < _column; columnIt++) {
				if (columnIt == 0)
					ss << "    --  ";
				else
					ss << " --  ";
			}

			ss << "\n";
			ss << yLable-- << " ";
			for (int columnIt = 0; columnIt < _column; columnIt++) {
				if (columnIt == _column - 1) {
					if (_board.at(rowIt).at(columnIt) > 0)
						ss << "|  " << _board.at(rowIt).at(columnIt) << " |";
					else if (_board.at(rowIt).at(columnIt) == 0)
						ss << "|    |";
					else
						ss << "| " << _board.at(rowIt).at(columnIt) << " |";
				}
				else {
					if (_board.at(rowIt).at(columnIt) > 0)
						ss << "|  " << _board.at(rowIt).at(columnIt) << " ";
					else if (_board.at(rowIt).at(columnIt) == 0)
						ss << "|    ";
					else
						ss << "| " << _board.at(rowIt).at(columnIt) << " ";
				}
			}

			ss << "\n";
			if (rowIt == _row - 1) {
				for (int columnIt = 0; columnIt < _column; columnIt++) {
					if (columnIt == 0)
						ss << "    --  ";
					else
						ss << " --  ";
				}
			}
		}

		ss << "\n  ";
		for (int columnIt = 0; columnIt < _column; columnIt++) {
			ss << "  " << xLabel << "  ";
			xLabel++;
		}
		return ss.str();
	}

	template<>
	std::string Board_Representation<std::string>::ToString() const {
		std::stringstream ss;
		int yLable = 9;
		char xLabel = 'a';

		for (int rowIt = 0; rowIt < _row; rowIt++) {
			for (int columnIt = 0; columnIt < _column; columnIt++) {
				if (columnIt == 0)
					ss << "    --  ";
				else
					ss << " --  ";
			}

			ss << "\n";
			ss << yLable-- << " ";
			for (int columnIt = 0; columnIt < _column; columnIt++) {
				if (columnIt == _column - 1) {
					if (_board.at(rowIt).at(columnIt) == "x")
						ss << "|    |";
					else
						ss << "| " << _board.at(rowIt).at(columnIt) << " |";
				}
				else {
					if (_board.at(rowIt).at(columnIt) == "x")
						ss << "|    ";
					else
						ss << "| " << _board.at(rowIt).at(columnIt) << " ";
				}
			}

			ss << "\n";
			if (rowIt == _row - 1) {
				for (int columnIt = 0; columnIt < _column; columnIt++) {
					if (columnIt == 0)
						ss << "    --  ";
					else
						ss << " --  ";
				}
			}
		}

		ss << "\n  ";
		for (int columnIt = 0; columnIt < _column; columnIt++) {
				ss << "  " << xLabel << "  ";
			xLabel++;
		}
		return ss.str();
	}

	//--- Set of board converting functions defined by object type ---//
	//--- Object type board ---//
	template<>
	Board_Representation<std::shared_ptr<Pieces>> ConvertBoard(const Board_Representation<std::shared_ptr<Pieces>>& board) {
		Board_Representation<std::shared_ptr<Pieces>> newBoard(board.GetRowNumber(), board.GetColumnNumber(), true);
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
		Board_Representation<int> newBoard(board.GetRowNumber(), board.GetColumnNumber(), true);
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
		Board_Representation<std::string> newBoard(board.GetRowNumber(), board.GetColumnNumber(), true);
		for (int rowIt = 0; rowIt < board.GetRowNumber(); rowIt++) {
			for (int columnIt = 0; columnIt < board.GetColumnNumber(); columnIt++) {
				newBoard.SetPlateValue(rowIt, columnIt, board.GetPlateValue(rowIt, columnIt)->GetName());
			}
		}

		PALE_ENGINE_TRACE("Convertion on string type!");
		return newBoard;
	}

	//--- Int type board ---//
	template<>
	Board_Representation<std::shared_ptr<Pieces>> ConvertBoard(const Board_Representation<int>& board) {
		try {
			Board_Representation<std::shared_ptr<Pieces>> newBoard(board.GetRowNumber(), board.GetColumnNumber(), true);
			for (int rowIt = 0; rowIt < board.GetRowNumber(); rowIt++) {
				for (int columnIt = 0; columnIt < board.GetColumnNumber(); columnIt++) {
					OWNERS tempPieceOwner;
					std::pair<unsigned int, unsigned int> tempOldPosition = std::make_pair(rowIt, columnIt);

					if (board.GetPlateValue(rowIt, columnIt) < 0)
						tempPieceOwner = OWNERS::BLACK;
					else
						tempPieceOwner = OWNERS::WHITE;

					if (abs(board.GetPlateValue(rowIt, columnIt)) == 7) {
						newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<King>(tempPieceOwner, std::make_pair(rowIt, columnIt)));

						//--- If 'Int board' or 'String board' needs to be convert on object board it needs to set all require bools ---//
						if ((tempPieceOwner == OWNERS::BLACK && tempOldPosition != Piece_Starting_Positions::m_kingStartPos.first.at(0)) ||
							(tempPieceOwner == OWNERS::WHITE && tempOldPosition != Piece_Starting_Positions::m_kingStartPos.second.at(0)))
							std::static_pointer_cast<King>(newBoard.GetPlateValue(rowIt, columnIt))->SetKingFirstMove(true);
					}
					else if (abs(board.GetPlateValue(rowIt, columnIt)) == 5)
						newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<Queen>(tempPieceOwner, std::make_pair(rowIt, columnIt)));
					else if (abs(board.GetPlateValue(rowIt, columnIt)) == 4)
						newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<Bishop>(tempPieceOwner, std::make_pair(rowIt, columnIt)));
					else if (abs(board.GetPlateValue(rowIt, columnIt)) == 3)
						newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<Knight>(tempPieceOwner, std::make_pair(rowIt, columnIt)));
					else if (abs(board.GetPlateValue(rowIt, columnIt)) == 2) {
						newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<Rook>(tempPieceOwner, std::make_pair(rowIt, columnIt)));

						//--- If 'Int board' or 'String board' needs to be convert on object board it needs to set all require bools ---//
						if (tempPieceOwner == OWNERS::BLACK) {
							if ((columnIt == 0 && tempOldPosition != Piece_Starting_Positions::m_rookStartPos.first.at(0)) ||
								(columnIt > 0 && tempOldPosition != Piece_Starting_Positions::m_rookStartPos.first.at(1)))
								std::static_pointer_cast<Rook>(newBoard.GetPlateValue(rowIt, columnIt))->SetFirstMove(true);
						}
						else if (tempPieceOwner == OWNERS::WHITE) {
							if((columnIt == 0 && tempOldPosition != Piece_Starting_Positions::m_rookStartPos.second.at(0)) ||
								(columnIt > 0 && tempOldPosition != Piece_Starting_Positions::m_rookStartPos.second.at(1)))
								std::static_pointer_cast<Rook>(newBoard.GetPlateValue(rowIt, columnIt))->SetFirstMove(true);
						}								
					}
					else if (abs(board.GetPlateValue(rowIt, columnIt)) == 1) {
						newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<Pawn>(tempPieceOwner, std::make_pair(rowIt, columnIt)));

						//--- If 'Int board' or 'String board' needs to be convert on object board it needs to set all require bools ---//
						if ((tempPieceOwner == OWNERS::BLACK && tempOldPosition != Piece_Starting_Positions::m_pawnStartPos.first.at(columnIt)) ||
							(tempPieceOwner == OWNERS::WHITE && tempOldPosition != Piece_Starting_Positions::m_pawnStartPos.second.at(columnIt)))
							std::static_pointer_cast<Pawn>(newBoard.GetPlateValue(rowIt, columnIt))->SetFirstMove(true);
					}
					else if (abs(board.GetPlateValue(rowIt, columnIt)) == 0)
						newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<Blank>(rowIt, columnIt));
					else
						throw PaleEngineException("Exception happened!", 'w', "Board_representation.cpp", 399, "ConvertBoard", INVALID_PIECE_ID);		
				}
			}

			PALE_ENGINE_TRACE("Convertion on object type!");
			return newBoard;
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	template<>
	Board_Representation<int> ConvertBoard(const Board_Representation<int>& board) {
		Board_Representation<int> newBoard(board.GetRowNumber(), board.GetColumnNumber(), true);
		for (int rowIt = 0; rowIt < board.GetRowNumber(); rowIt++) {
			for (int columnIt = 0; columnIt < board.GetColumnNumber(); columnIt++) {
				newBoard.SetPlateValue(rowIt, columnIt, board.GetPlateValue(rowIt, columnIt));
			}
		}

		PALE_ENGINE_TRACE("No convertion made!");
		return newBoard;
	}

	template<>
	Board_Representation<std::string> ConvertBoard(const Board_Representation<int>& board) {
		try {
			Board_Representation<std::string> newBoard(board.GetRowNumber(), board.GetColumnNumber(), true);
			for (int rowIt = 0; rowIt < board.GetRowNumber(); rowIt++) {
				for (int columnIt = 0; columnIt < board.GetColumnNumber(); columnIt++) {
					if (abs(board.GetPlateValue(rowIt, columnIt)) == 7) {
						if (board.GetPlateValue(rowIt, columnIt) < 0)
							newBoard.SetPlateValue(rowIt, columnIt, "bK");
						else
							newBoard.SetPlateValue(rowIt, columnIt, "wK");
					}
					else if (abs(board.GetPlateValue(rowIt, columnIt)) == 5) {
						if (board.GetPlateValue(rowIt, columnIt) < 0)
							newBoard.SetPlateValue(rowIt, columnIt, "bQ");
						else
							newBoard.SetPlateValue(rowIt, columnIt, "wQ");
					}
					else if (abs(board.GetPlateValue(rowIt, columnIt)) == 4) {
						if (board.GetPlateValue(rowIt, columnIt) < 0)
							newBoard.SetPlateValue(rowIt, columnIt, "bB");
						else
							newBoard.SetPlateValue(rowIt, columnIt, "wB");
					}
					else if (abs(board.GetPlateValue(rowIt, columnIt)) == 3) {
						if (board.GetPlateValue(rowIt, columnIt) < 0)
							newBoard.SetPlateValue(rowIt, columnIt, "bN");
						else
							newBoard.SetPlateValue(rowIt, columnIt, "wN");
					}
					else if (abs(board.GetPlateValue(rowIt, columnIt)) == 2) {
						if (board.GetPlateValue(rowIt, columnIt) < 0)
							newBoard.SetPlateValue(rowIt, columnIt, "bR");
						else
							newBoard.SetPlateValue(rowIt, columnIt, "wR");
					}
					else if (abs(board.GetPlateValue(rowIt, columnIt)) == 1) {
						if (board.GetPlateValue(rowIt, columnIt) < 0)
							newBoard.SetPlateValue(rowIt, columnIt, "bP");
						else
							newBoard.SetPlateValue(rowIt, columnIt, "wP");
					}
					else if (abs(board.GetPlateValue(rowIt, columnIt)) == 0)
						newBoard.SetPlateValue(rowIt, columnIt, "x");
					else
						throw PaleEngineException("Exception happened!", 'w', "Board_representation.cpp", 399, "ConvertBoard", INVALID_PIECE_ID);
				}
			}

			PALE_ENGINE_TRACE("Convertion on string type!");
			return newBoard;
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	//--- String type board ---//
	template<>
	Board_Representation<std::shared_ptr<Pieces>> ConvertBoard(const Board_Representation<std::string>& board) {
		try {
			Board_Representation<std::shared_ptr<Pieces>> newBoard(board.GetRowNumber(), board.GetColumnNumber(), true);
			for (int rowIt = 0; rowIt < board.GetRowNumber(); rowIt++) {
				for (int columnIt = 0; columnIt < board.GetColumnNumber(); columnIt++) {
					OWNERS tempPieceOwner;
					std::pair<unsigned int, unsigned int> tempOldPosition = std::make_pair(rowIt, columnIt);

					if (board.GetPlateValue(rowIt, columnIt) == "x")
						newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<Blank>(rowIt, columnIt));
					else {
						if (board.GetPlateValue(rowIt, columnIt)[0] == 'b')
							tempPieceOwner = OWNERS::BLACK;
						else if (board.GetPlateValue(rowIt, columnIt)[0] == 'w')
							tempPieceOwner = OWNERS::WHITE;
						else
							throw PaleEngineException("Exception happened!", 'e', "Board_representation.cpp", 399, "ConvertBoard", FIGURE_BAD_OWNER);


						if (board.GetPlateValue(rowIt, columnIt)[1] == 'K') {
							newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<King>(tempPieceOwner, std::make_pair(rowIt, columnIt)));

							//--- If 'Int board' or 'String board' needs to be convert on object board it needs to set all require bools ---//
							if ((tempPieceOwner == OWNERS::BLACK && tempOldPosition != Piece_Starting_Positions::m_kingStartPos.first.at(0)) ||
								(tempPieceOwner == OWNERS::WHITE && tempOldPosition != Piece_Starting_Positions::m_kingStartPos.second.at(0)))
								std::static_pointer_cast<King>(newBoard.GetPlateValue(rowIt, columnIt))->SetKingFirstMove(true);
						}
						else if (board.GetPlateValue(rowIt, columnIt)[1] == 'Q')
							newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<Queen>(tempPieceOwner, std::make_pair(rowIt, columnIt)));
						else if (board.GetPlateValue(rowIt, columnIt)[1] == 'B')
							newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<Bishop>(tempPieceOwner, std::make_pair(rowIt, columnIt)));
						else if (board.GetPlateValue(rowIt, columnIt)[1] == 'N')
							newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<Knight>(tempPieceOwner, std::make_pair(rowIt, columnIt)));
						else if (board.GetPlateValue(rowIt, columnIt)[1] == 'R') {
							newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<Rook>(tempPieceOwner, std::make_pair(rowIt, columnIt)));

							//--- If 'Int board' or 'String board' needs to be convert on object board it needs to set all require bools ---//
							if (tempPieceOwner == OWNERS::BLACK) {
								if ((columnIt == 0 && tempOldPosition != Piece_Starting_Positions::m_rookStartPos.first.at(0)) ||
									(columnIt > 0 && tempOldPosition != Piece_Starting_Positions::m_rookStartPos.first.at(1)))
									std::static_pointer_cast<Rook>(newBoard.GetPlateValue(rowIt, columnIt))->SetFirstMove(true);
							}
							else if (tempPieceOwner == OWNERS::WHITE) {
								if ((columnIt == 0 && tempOldPosition != Piece_Starting_Positions::m_rookStartPos.second.at(0)) ||
									(columnIt > 0 && tempOldPosition != Piece_Starting_Positions::m_rookStartPos.second.at(1)))
									std::static_pointer_cast<Rook>(newBoard.GetPlateValue(rowIt, columnIt))->SetFirstMove(true);
							}
						}
						else if (board.GetPlateValue(rowIt, columnIt)[1] == 'P') {
							newBoard.SetPlateValue(rowIt, columnIt, std::make_shared<Pawn>(tempPieceOwner, std::make_pair(rowIt, columnIt)));

							//--- If 'Int board' or 'String board' needs to be convert on object board it needs to set all require bools ---//
							if ((tempPieceOwner == OWNERS::BLACK && tempOldPosition != Piece_Starting_Positions::m_pawnStartPos.first.at(columnIt)) ||
								(tempPieceOwner == OWNERS::WHITE && tempOldPosition != Piece_Starting_Positions::m_pawnStartPos.second.at(columnIt)))
								std::static_pointer_cast<Pawn>(newBoard.GetPlateValue(rowIt, columnIt))->SetFirstMove(true);
						}
						else
							throw PaleEngineException("Exception happened!", 'w', "Board_representation.cpp", 399, "ConvertBoard", INVALID_PIECE_ID);
					}
				}
			}

			PALE_ENGINE_TRACE("Convertion on object type!");
			return newBoard;
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	template<>
	Board_Representation<int> ConvertBoard(const Board_Representation<std::string>& board) {
		try {
			Board_Representation<int> newBoard(board.GetRowNumber(), board.GetColumnNumber(), true);
			for (int rowIt = 0; rowIt < board.GetRowNumber(); rowIt++) {
				for (int columnIt = 0; columnIt < board.GetColumnNumber(); columnIt++) {
					int value;
					if (board.GetPlateValue(rowIt, columnIt) == "x")
						value = 0;
					else {
						switch (board.GetPlateValue(rowIt, columnIt)[1]) {
						case 'K': { value = 7; break; }
						case 'Q': { value = 5; break; }
						case 'B': { value = 4; break; }
						case 'N': { value = 3; break; }
						case 'R': { value = 2; break; }
						case 'P': { value = 1; break; }
						default: { throw PaleEngineException("Exception happened!", 'w', "Board_representation.cpp", 399, "ConvertBoard", INVALID_PIECE_ID); break; }
						}

						if (board.GetPlateValue(rowIt, columnIt)[0] == 'b')
							value *= -1;
					}

					newBoard.SetPlateValue(rowIt, columnIt, value);
				}
			}

			PALE_ENGINE_TRACE("Convertion on int type!");
			return newBoard;
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	template<>
	Board_Representation<std::string> ConvertBoard(const Board_Representation<std::string>& board) {
		Board_Representation<std::string> newBoard(board.GetRowNumber(), board.GetColumnNumber(), true);
		for (int rowIt = 0; rowIt < board.GetRowNumber(); rowIt++) {
			for (int columnIt = 0; columnIt < board.GetColumnNumber(); columnIt++) {
				newBoard.SetPlateValue(rowIt, columnIt, board.GetPlateValue(rowIt, columnIt));
			}
		}

		PALE_ENGINE_TRACE("No convertion made!");
		return newBoard;
	}

	//--- Set of SetPlateValue functions defined by object type ---//
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
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
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
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
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
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}
}
