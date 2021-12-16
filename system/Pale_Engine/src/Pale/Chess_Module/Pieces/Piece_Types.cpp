#include "palepch.h"
#include "Piece_Types.h"

namespace Pale::Chess_Logic {
	std::pair<std::vector<std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, unsigned int>>> Piece_Starting_Positions::m_kingStartPos = {
			{ {0, 4} },
			{ {7, 4} }
	};

	std::pair<std::vector<std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, unsigned int>>> Piece_Starting_Positions::m_queenStartPos = {
			{ {0, 3} },
			{ {7, 3} }
	};

	std::pair<std::vector<std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, unsigned int>>> Piece_Starting_Positions::m_bishopStartPos = {
			{ {0, 2}, {0, 5} },
			{ {7, 2}, {7, 5} }
	};

	std::pair<std::vector<std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, unsigned int>>> Piece_Starting_Positions::m_knightStartPos = {
			{ {0, 1}, {0, 6} },
			{ {7, 1}, {7, 6} }
	};

	std::pair<std::vector<std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, unsigned int>>> Piece_Starting_Positions::m_rookStartPos = {
			{ {0, 0}, {0, 7} },
			{ {7, 0}, {7, 7} }
	};

	std::pair<std::vector<std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, unsigned int>>> Piece_Starting_Positions::m_pawnStartPos = {
			{ {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7} },
			{ {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 7} }
	};

	//--- Function definitions for king class ---//
	King::King(OWNERS owner, unsigned int numberOfCopy) : Pieces(owner, 1), _check(false), _checkMate(false), _firstMove(true) {
		try {
			if (numberOfCopy > _limitOfCopies - 1)
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 39, "King", COPY_LIMIT_EXCEEDED);

			if (owner == OWNERS::BLACK) {
				_value = -7;
				_name = "bK";

				_positionCords = std::make_pair(Piece_Starting_Positions::m_kingStartPos.first.at(numberOfCopy).first,
					Piece_Starting_Positions::m_kingStartPos.first.at(numberOfCopy).second);
			}
			else if (owner == OWNERS::WHITE) {
				_value = 7;
				_name = "wK";

				_positionCords = std::make_pair(Piece_Starting_Positions::m_kingStartPos.second.at(numberOfCopy).first,
					Piece_Starting_Positions::m_kingStartPos.second.at(numberOfCopy).second);
			}
			else
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 56, "King", FIGURE_BAD_OWNER);
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	King::King(OWNERS owner, std::pair<unsigned int, unsigned int> startingPos) : Pieces(owner, 1), _check(false), _checkMate(false), _firstMove(true) {
		try {
			if (owner == OWNERS::BLACK) {
				_value = -7;
				_name = "bK";
			}
			else if (owner == OWNERS::WHITE) {
				_value = 7;
				_name = "wK";
			}
			else
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 83, "King", FIGURE_BAD_OWNER);

			_positionCords = startingPos;
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	bool King::MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
		bool canMove = true;
				
		//--- King cannot move more than 1 step ---//
		if ((abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) > 1 || 
			abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) > 1))
			canMove = false;

		//--- King piece cannot move on plate occupied by the same owners piece ---//
		if (board.at(endPos.first).at(endPos.second)->GetOwner() == _owner)
			canMove = false;

		//--- If king is under check ---//
		if (canMove) {
			std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
			tempBoard.at(endPos.first).at(endPos.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
			tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
			if (KingIsChecked(tempBoard, _owner))
				canMove = false;
		}

		if (!canMove)
			PALE_ENGINE_TRACE("King piece cannot move to this location.");
		else {
			if (_firstMove)
				_firstMove = false;
		}
		// TODO: Work but test needed!
		return canMove;
	}

	bool King::SpecialLogic(MOVE_TYPES type, std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board, std::optional<char> newPiece) { 
		// IMPORTANT: REMEMBER! To perform castling, SpecialLogic method have to return 'true' for both King and Rook performing manover.
		bool canPerform = true;
		if (endPos.second < _positionCords.second) { //Long castling
			//--- King can perform long castling if it didn't move in this game ---//
			if (!_firstMove)
				canPerform = false;

			//--- King can perform long castling if it is on proper position ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK && _positionCords != Piece_Starting_Positions::m_kingStartPos.first.at(0)) ||
					(_owner == OWNERS::WHITE && _positionCords != Piece_Starting_Positions::m_kingStartPos.second.at(0)))
					canPerform = false;
			}

			//--- King can perform long castling if left rook is on proper position ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK && board.at(Piece_Starting_Positions::m_rookStartPos.first.at(0).first).at(Piece_Starting_Positions::m_rookStartPos.first.at(0).first)->GetValue() != -2) ||
					(_owner == OWNERS::WHITE && board.at(Piece_Starting_Positions::m_rookStartPos.second.at(0).first).at(Piece_Starting_Positions::m_rookStartPos.second.at(0).first)->GetValue() != 2))
					canPerform = false;
			}

			//--- King can perform long castling if left rook also didn't move in this game ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK &&
					!std::static_pointer_cast<Rook>(board.at(Piece_Starting_Positions::m_rookStartPos.first.at(0).first).at(Piece_Starting_Positions::m_rookStartPos.first.at(0).second))->DidNotMove()) ||
					(_owner == OWNERS::WHITE &&
						!std::static_pointer_cast<Rook>(board.at(Piece_Starting_Positions::m_rookStartPos.second.at(0).first).at(Piece_Starting_Positions::m_rookStartPos.second.at(0).second))->DidNotMove()))
					canPerform = false;
			}

			//--- King can perform long castling if path between it and left rook is clear ---//
			if (canPerform) {
				if (_owner == OWNERS::BLACK) {
					std::pair<unsigned int, unsigned int> tempEndPos = std::make_pair(Piece_Starting_Positions::m_rookStartPos.first.at(0).first,
						Piece_Starting_Positions::m_rookStartPos.first.at(0).second + 1);
					if (!IsPathClear(_positionCords, tempEndPos, board))
						canPerform = false;
				}
				else {
					std::pair<unsigned int, unsigned int> tempEndPos = std::make_pair(Piece_Starting_Positions::m_rookStartPos.second.at(0).first,
						Piece_Starting_Positions::m_rookStartPos.second.at(0).second + 1);
					if (!IsPathClear(_positionCords, tempEndPos, board))
						canPerform = false;
				}
			}

			//--- King cannot perform long castling if it will put it under check ---//
			if (canPerform) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				std::shared_ptr<Castling> tempSpecialStrategy = std::make_shared<Castling>(_positionCords, Piece_Starting_Positions::m_rookStartPos.first.at(0), tempBoard);
				tempSpecialStrategy->Execute();
				if (KingIsChecked(tempBoard, _owner))
					canPerform = false;
			}

			if (!canPerform)
				PALE_ENGINE_TRACE("King piece cannot perform special move.");
			else {
				if (_firstMove)
					_firstMove = false;

				if (_owner == OWNERS::BLACK) //Black perform castling
					_specialMove = std::make_shared<Castling>(_positionCords, Piece_Starting_Positions::m_rookStartPos.first.at(0), board);
				else //White perform castling
					_specialMove = std::make_shared<Castling>(_positionCords, Piece_Starting_Positions::m_rookStartPos.second.at(0), board);
			}
		}
		else { //Short castling
			//--- King can perform short castling if it didn't move in this game ---//
			if (!_firstMove)
				canPerform = false;

			//--- King can perform short castling if it is on proper position ---//
			if ((_owner == OWNERS::BLACK && _positionCords != Piece_Starting_Positions::m_kingStartPos.first.at(0)) ||
				(_owner == OWNERS::WHITE && _positionCords != Piece_Starting_Positions::m_kingStartPos.second.at(0)))
				canPerform = false;

			//--- King can perform short castling if left rook didn't move in this game ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK && board.at(Piece_Starting_Positions::m_rookStartPos.first.at(1).first).at(Piece_Starting_Positions::m_rookStartPos.first.at(1).first)->GetValue() != -2) ||
					(_owner == OWNERS::WHITE && board.at(Piece_Starting_Positions::m_rookStartPos.second.at(1).first).at(Piece_Starting_Positions::m_rookStartPos.second.at(1).first)->GetValue() != 2))
					canPerform = false;
			}

			//--- King can perform long castling if rook also didn't move in this game ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK &&
					!std::static_pointer_cast<Rook>(board.at(Piece_Starting_Positions::m_rookStartPos.first.at(1).first).at(Piece_Starting_Positions::m_rookStartPos.first.at(1).second))->DidNotMove()) ||
					(_owner == OWNERS::WHITE &&
						!std::static_pointer_cast<Rook>(board.at(Piece_Starting_Positions::m_rookStartPos.second.at(1).first).at(Piece_Starting_Positions::m_rookStartPos.second.at(1).second))->DidNotMove()))
					canPerform = false;
			}

			//--- King can perform short castling if path between it and left rook is clear ---//
			if (canPerform) {
				if (_owner == OWNERS::BLACK) {
					std::pair<unsigned int, unsigned int> tempEndPos = std::make_pair(Piece_Starting_Positions::m_rookStartPos.first.at(1).first,
						Piece_Starting_Positions::m_rookStartPos.first.at(1).second - 1);
					if (!IsPathClear(_positionCords, tempEndPos, board))
						canPerform = false;
				}
				else {
					std::pair<unsigned int, unsigned int> tempEndPos = std::make_pair(Piece_Starting_Positions::m_rookStartPos.second.at(1).first,
						Piece_Starting_Positions::m_rookStartPos.second.at(1).second - 1);
					if (!IsPathClear(_positionCords, tempEndPos, board))
						canPerform = false;
				}
			}

			//--- King cannot perform short castling if it will put it under check ---//
			if (canPerform) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				std::shared_ptr<Castling> tempSpecialStrategy = std::make_shared<Castling>(_positionCords, Piece_Starting_Positions::m_rookStartPos.first.at(1), tempBoard);
				tempSpecialStrategy->Execute();
				if (KingIsChecked(tempBoard, _owner))
					canPerform = false;
			}

			if (!canPerform)
				PALE_ENGINE_TRACE("King piece cannot perform special move.");
			else {
				if (_firstMove)
					_firstMove = false;

				if (_owner == OWNERS::BLACK) //Black perform castling
					_specialMove = std::make_shared<Castling>(_positionCords, Piece_Starting_Positions::m_rookStartPos.first.at(1), board);
				else //White perform castling
					_specialMove = std::make_shared<Castling>(_positionCords, Piece_Starting_Positions::m_rookStartPos.second.at(1), board);
			}
		}
		// TODO: Check if work properly
		return canPerform;
	}
		
	std::vector<Move_Command> King::GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
		std::vector<Move_Command> tempMoveList;
		//--- Traditional moves ---//
		if (static_cast<int>(_positionCords.first - 1) >= 0) { //Move north
			if (board.at(_positionCords.first - 1).at(_positionCords.second)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first - 1).at(_positionCords.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if(board.at(_positionCords.first - 1).at(_positionCords.second)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, false, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second)));
				}
			}
		}

		if (static_cast<int>(_positionCords.first - 1) >= 0 && _positionCords.second + 1 < board.size()) { //Move north-east
			if (board.at(_positionCords.first - 1).at(_positionCords.second + 1)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first - 1).at(_positionCords.second + 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first - 1).at(_positionCords.second + 1)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second + 1)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, false, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second + 1)));
				}
			}
		}

		if (_positionCords.second + 1 < board.size()) { //Move east
			if (board.at(_positionCords.first).at(_positionCords.second + 1)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first).at(_positionCords.second + 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first).at(_positionCords.second + 1)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, true, _positionCords, std::make_pair(_positionCords.first, _positionCords.second + 1)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, false, _positionCords, std::make_pair(_positionCords.first, _positionCords.second + 1)));
				}
			}
		}

		if (_positionCords.first + 1 < board.size() && _positionCords.second + 1 < board.size()) { //Move souht-east
			if (board.at(_positionCords.first + 1).at(_positionCords.second + 1)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first + 1).at(_positionCords.second + 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first + 1).at(_positionCords.second + 1)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second + 1)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, false, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second + 1)));
				}
			}
		}

		if (_positionCords.first + 1 < board.size()) { //Move south
			if (board.at(_positionCords.first + 1).at(_positionCords.second)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first + 1).at(_positionCords.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first + 1).at(_positionCords.second)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, false, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second)));
				}
			}
		}

		if (_positionCords.first + 1 < board.size() && static_cast<int>(_positionCords.second - 1) >= 0) { //Move south-west
			if (board.at(_positionCords.first + 1).at(_positionCords.second - 1)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first + 1).at(_positionCords.second - 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first + 1).at(_positionCords.second - 1)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second - 1)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, false, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second - 1)));
				}
			}
		}

		if (static_cast<int>(_positionCords.second - 1) >= 0) { //Move west
			if (board.at(_positionCords.first).at(_positionCords.second - 1)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first).at(_positionCords.second - 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first).at(_positionCords.second - 1)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, true, _positionCords, std::make_pair(_positionCords.first, _positionCords.second - 1)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, false, _positionCords, std::make_pair(_positionCords.first, _positionCords.second - 1)));
				}
			}
		}

		if (static_cast<int>(_positionCords.first - 1) >= 0 && static_cast<int>(_positionCords.second - 1) >= 0) { //Move north-west
			if (board.at(_positionCords.first - 1).at(_positionCords.second - 1)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first - 1).at(_positionCords.second - 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first - 1).at(_positionCords.second - 1)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second - 1)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'K', _owner, false, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second - 1)));
				}
			}
		}

		//--- Special moves ---//
		if (static_cast<int>(_positionCords.second - 4) >= 0) { //Long castling
			if (SpecialLogic(MOVE_TYPES::CASTLING, std::make_pair(_positionCords.first, _positionCords.second - 2), board, std::optional<char>()) && 
				abs(board.at(_positionCords.first).at(_positionCords.second - 4)->GetValue()) == 2) {
				if (board.at(_positionCords.first).at(_positionCords.second - 4)->SpecialLogic(MOVE_TYPES::CASTLING, std::make_pair(_positionCords.first, _positionCords.second - 1), board, std::optional<char>()))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::CASTLING, 'K', _owner, false, _positionCords, std::make_pair(_positionCords.first, _positionCords.second - 2)));
			}
		}
			
		if (_positionCords.second + 3 < board.at(0).size()) { //Short castling
			if (SpecialLogic(MOVE_TYPES::CASTLING, std::make_pair(_positionCords.first, _positionCords.second + 2), board, std::optional<char>()) &&
				abs(board.at(_positionCords.first).at(_positionCords.second + 3)->GetValue()) == 2) {
				if (board.at(_positionCords.first).at(_positionCords.second + 3)->SpecialLogic(MOVE_TYPES::CASTLING, std::make_pair(_positionCords.first, _positionCords.second + 1), board, std::optional<char>()))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::CASTLING, 'K', _owner, false, _positionCords, std::make_pair(_positionCords.first, _positionCords.second + 2)));
			}
		}

		PALE_ENGINE_TRACE("Number of moves for piece on cordinats ({0}, {1}): {2}", _positionCords.first, _positionCords.second, tempMoveList.size());
		return tempMoveList;
	}

	//--- Function definitions for queen class ---//
	Queen::Queen(OWNERS owner, unsigned int numberOfCopy) : Pieces(owner, 1) {
		try {
			if (numberOfCopy > _limitOfCopies - 1)
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 66, "Queen", COPY_LIMIT_EXCEEDED);

			if (owner == OWNERS::BLACK) {
				_value = -5;
				_name = "bQ";

				_positionCords = std::make_pair(Piece_Starting_Positions::m_queenStartPos.first.at(numberOfCopy).first,
					Piece_Starting_Positions::m_queenStartPos.first.at(numberOfCopy).second);
			}
			else if (owner == OWNERS::WHITE) {
				_value = 5;
				_name = "wQ";

				_positionCords = std::make_pair(Piece_Starting_Positions::m_queenStartPos.second.at(numberOfCopy).first,
					Piece_Starting_Positions::m_queenStartPos.second.at(numberOfCopy).second);
			}
			else
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 83, "Queen", FIGURE_BAD_OWNER);

			_specialMove = std::make_shared<None>();
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if(exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	Queen::Queen(OWNERS owner, std::pair<unsigned int, unsigned int> startingPos) : Pieces(owner, 1) {
		try {
			if (owner == OWNERS::BLACK) {
				_value = -5;
				_name = "bQ";
			}
			else if (owner == OWNERS::WHITE) {
				_value = 5;
				_name = "wQ";
			}
			else
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 83, "Queen", FIGURE_BAD_OWNER);

			_positionCords = startingPos;
			_specialMove = std::make_shared<None>();
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	bool Queen::MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
		bool canMove = true;

		//--- Do not allow queene piece to move different than diagonal, horizontal or verticaly ---//
		if (abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) != abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) &&
			abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) > 0 && abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) > 0)
			canMove = false;

		//--- Path of queen move have to be clear ---//
		if (canMove) {
			if (!IsPathClear(_positionCords, endPos, board))
				canMove = false;
		}

		//--- Potential move cannot put king under check ---//
		if (canMove) {
			std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
			tempBoard.at(endPos.first).at(endPos.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
			tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
			if (KingIsChecked(tempBoard, _owner))
				canMove = false;
		}

		if (!canMove)
			PALE_ENGINE_TRACE("Queen piece cannot move to this location.");
		// TODO: Check if work properly
		return canMove;
	}

	std::vector<Move_Command> Queen::GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
		std::vector<Move_Command> tempMoveList;
		//--- Traditional moves ---//
		for (int it = 1; it < board.size(); it++) {
			if (static_cast<int>(_positionCords.first - it) >= 0) { //Move north
				if (board.at(_positionCords.first - it).at(_positionCords.second)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first - it).at(_positionCords.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first - it).at(_positionCords.second)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, true, _positionCords, std::make_pair(_positionCords.first - it, _positionCords.second)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, false, _positionCords, std::make_pair(_positionCords.first - it, _positionCords.second)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (static_cast<int>(_positionCords.first - it) >= 0 && _positionCords.second + it < board.size()) { //Move north-east
				if (board.at(_positionCords.first - it).at(_positionCords.second + it)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first - it).at(_positionCords.second + it) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first - it).at(_positionCords.second + it)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, true, _positionCords, std::make_pair(_positionCords.first - it, _positionCords.second + it)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, false, _positionCords, std::make_pair(_positionCords.first - it, _positionCords.second + it)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (_positionCords.second + it < board.size()) { //Move east
				if (board.at(_positionCords.first).at(_positionCords.second + it)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first).at(_positionCords.second + it) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first).at(_positionCords.second + it)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, true, _positionCords, std::make_pair(_positionCords.first, _positionCords.second + it)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, false, _positionCords, std::make_pair(_positionCords.first, _positionCords.second + it)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (_positionCords.first + it < board.size() && _positionCords.second + it < board.size()) { //Move south-east
				if (board.at(_positionCords.first + it).at(_positionCords.second + it)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first + it).at(_positionCords.second + it) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first + it).at(_positionCords.second + it)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, true, _positionCords, std::make_pair(_positionCords.first + it, _positionCords.second + it)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, false, _positionCords, std::make_pair(_positionCords.first + it, _positionCords.second + it)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (_positionCords.first + it < board.size()) { //Move south
				if (board.at(_positionCords.first + it).at(_positionCords.second)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first + it).at(_positionCords.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first + it).at(_positionCords.second)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, true, _positionCords, std::make_pair(_positionCords.first + it, _positionCords.second)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, false, _positionCords, std::make_pair(_positionCords.first + it, _positionCords.second)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (_positionCords.first + it < board.size() && static_cast<int>(_positionCords.second - it) >= 0) { //Move south-west
				if (board.at(_positionCords.first + it).at(_positionCords.second - it)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first + it).at(_positionCords.second - it) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first - it).at(_positionCords.second)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, true, _positionCords, std::make_pair(_positionCords.first + it, _positionCords.second - it)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, false, _positionCords, std::make_pair(_positionCords.first + it, _positionCords.second - it)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (static_cast<int>(_positionCords.second - it) >= 0) { //Move west
				if (board.at(_positionCords.first).at(_positionCords.second - it)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first).at(_positionCords.second - it) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first).at(_positionCords.second - it)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, true, _positionCords, std::make_pair(_positionCords.first, _positionCords.second - it)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, false, _positionCords, std::make_pair(_positionCords.first, _positionCords.second - it)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (static_cast<int>(_positionCords.first - it) >= 0 && static_cast<int>(_positionCords.second - it) >= 0) { //Move north-west
				if (board.at(_positionCords.first - it).at(_positionCords.second - it)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first - it).at(_positionCords.second - it) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first - it).at(_positionCords.second - it)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, true, _positionCords, std::make_pair(_positionCords.first - it, _positionCords.second - it)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'Q', _owner, false, _positionCords, std::make_pair(_positionCords.first - it, _positionCords.second - it)));
					}
				}
				else
					break;
			}
		}

		PALE_ENGINE_TRACE("Number of moves for piece on cordinats ({0}, {1}): {2}", _positionCords.first, _positionCords.second, tempMoveList.size());
		return tempMoveList;
	}

	//--- Function definitions for bishop class ---//
	Bishop::Bishop(OWNERS owner, unsigned int numberOfCopy) : Pieces(owner, 2) {
		try {
			if (numberOfCopy > _limitOfCopies - 1)
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 93, "Bishop", COPY_LIMIT_EXCEEDED);

			if (owner == OWNERS::BLACK) {
				_value = -4;
				_name = "bB";

				_positionCords = std::make_pair(Piece_Starting_Positions::m_bishopStartPos.first.at(numberOfCopy).first,
					Piece_Starting_Positions::m_bishopStartPos.first.at(numberOfCopy).second);
			}
			else if (owner == OWNERS::WHITE) {
				_value = 4;
				_name = "wB";

				_positionCords = std::make_pair(Piece_Starting_Positions::m_bishopStartPos.second.at(numberOfCopy).first,
					Piece_Starting_Positions::m_bishopStartPos.second.at(numberOfCopy).second);
			}
			else
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 110, "Bishop", FIGURE_BAD_OWNER);

			_specialMove = std::make_shared<None>();
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	Bishop::Bishop(OWNERS owner, std::pair<unsigned int, unsigned int> startingPos) : Pieces(owner, 2) {
		try {
			if (owner == OWNERS::BLACK) {
				_value = -4;
				_name = "bB";
			}
			else if (owner == OWNERS::WHITE) {
				_value = 4;
				_name = "wB";
			}
			else
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 110, "Bishop", FIGURE_BAD_OWNER);

			_positionCords = startingPos;
			_specialMove = std::make_shared<None>();
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	bool Bishop::MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
		bool canMove = true;

		//--- Do not allow bishop piece to move different than diagonal ---//
		if (abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) != abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)))
			canMove = false;

		//--- Path of bishop move have to be clear ---//
		if (canMove) {
			if (!IsPathClear(_positionCords, endPos, board))
				canMove = false;
		}

		//--- Potential move cannot put king under check ---//
		if (canMove) {
			std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
			tempBoard.at(endPos.first).at(endPos.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
			tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
			if (KingIsChecked(tempBoard, _owner))
				canMove = false;
		}

		if(!canMove)
			PALE_ENGINE_TRACE("Bishop piece cannot move to this location.");
		// TODO: Check if work properly
		return canMove;
	}

	std::vector<Move_Command> Bishop::GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
		std::vector<Move_Command> tempMoveList;
		//--- Traditional moves ---//
		for (int it = 1; it < board.size(); it++) {
			if (static_cast<int>(_positionCords.first - it) >= 0 && _positionCords.second + it < board.size()) { //Move north-east
				if (board.at(_positionCords.first - it).at(_positionCords.second + it)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first - it).at(_positionCords.second + it) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first - it).at(_positionCords.second + it)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'B', _owner, true, _positionCords, std::make_pair(_positionCords.first - it, _positionCords.second + it)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'B', _owner, false, _positionCords, std::make_pair(_positionCords.first - it, _positionCords.second + it)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (_positionCords.first + it < board.size() && _positionCords.second + it < board.size()) { //Move south-east
				if (board.at(_positionCords.first + it).at(_positionCords.second + it)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first + it).at(_positionCords.second + it) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first + it).at(_positionCords.second + it)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'B', _owner, true, _positionCords, std::make_pair(_positionCords.first + it, _positionCords.second + it)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'B', _owner, false, _positionCords, std::make_pair(_positionCords.first + it, _positionCords.second + it)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (_positionCords.first + it < board.size() && static_cast<int>(_positionCords.second - it) >= 0) { //Move south-west
				if (board.at(_positionCords.first + it).at(_positionCords.second - it)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first + it).at(_positionCords.second - it) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first + it).at(_positionCords.second - it)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'B', _owner, true, _positionCords, std::make_pair(_positionCords.first + it, _positionCords.second - it)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'B', _owner, false, _positionCords, std::make_pair(_positionCords.first + it, _positionCords.second - it)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (static_cast<int>(_positionCords.first - it) >= 0 && static_cast<int>(_positionCords.second - it) >= 0) { //Move north-west
				if (board.at(_positionCords.first - it).at(_positionCords.second - it)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first - it).at(_positionCords.second - it) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first - it).at(_positionCords.second - it)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'B', _owner, true, _positionCords, std::make_pair(_positionCords.first - it, _positionCords.second - it)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'B', _owner, false, _positionCords, std::make_pair(_positionCords.first - it, _positionCords.second - it)));
					}
				}
				else
					break;
			}
		}

		PALE_ENGINE_TRACE("Number of moves for piece on cordinats ({0}, {1}): {2}", _positionCords.first, _positionCords.second, tempMoveList.size());
		return tempMoveList;
	}

	//--- Function definitions for knight class ---//
	Knight::Knight(OWNERS owner, unsigned int numberOfCopy) : Pieces(owner, 2) {
		try {
			if (numberOfCopy > _limitOfCopies - 1)
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 120, "Knight", COPY_LIMIT_EXCEEDED);

			if (owner == OWNERS::BLACK) {
				_value = -3;
				_name = "bN";

				_positionCords = std::make_pair(Piece_Starting_Positions::m_knightStartPos.first.at(numberOfCopy).first,
					Piece_Starting_Positions::m_knightStartPos.first.at(numberOfCopy).second);
			}
			else if (owner == OWNERS::WHITE) {
				_value = 3;
				_name = "wN";

				_positionCords = std::make_pair(Piece_Starting_Positions::m_knightStartPos.second.at(numberOfCopy).first,
					Piece_Starting_Positions::m_knightStartPos.second.at(numberOfCopy).second);
			}
			else
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 138, "Knight", FIGURE_BAD_OWNER);

			_specialMove = std::make_shared<None>();
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	Knight::Knight(OWNERS owner, std::pair<unsigned int, unsigned int> startingPos) : Pieces(owner, 2) {
		try {
			if (owner == OWNERS::BLACK) {
				_value = -3;
				_name = "bN";
			}
			else if (owner == OWNERS::WHITE) {
				_value = 3;
				_name = "wN";
			}
			else
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 138, "Knight", FIGURE_BAD_OWNER);

			_positionCords = startingPos;
			_specialMove = std::make_shared<None>();
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	bool Knight::MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
		bool canMove = true;

		//--- Do not allow knight piece to move different than 2 linear + 1 diagonaly ---//
		if (abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) > 2 || abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) > 2 ||
			abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) < 1 || abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) < 1 ||
			abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) == abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)))
			canMove = false;

		//--- Knight piece cannot move on plate occupied by the same owners piece ---//
		if (board.at(endPos.first).at(endPos.second)->GetOwner() == _owner)
			canMove = false;
			
		//--- Potential move cannot put king under check ---//
		if (canMove) {
			std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
			tempBoard.at(endPos.first).at(endPos.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
			tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
			if (KingIsChecked(tempBoard, _owner))
				canMove = false;
		}

		if (!canMove)
			PALE_ENGINE_TRACE("Knight piece cannot move to this location.");
		// TODO: Check if work properly
		return canMove;
	}

	std::vector<Move_Command> Knight::GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
		std::vector<Move_Command> tempMoveList;
		//--- Traditional moves ---//
		if (static_cast<int>(_positionCords.first - 2) >= 0 && static_cast<int>(_positionCords.second - 1) >= 0) { //Move 2 up + 1 left
			if (board.at(_positionCords.first - 2).at(_positionCords.second - 1)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first - 2).at(_positionCords.second - 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first - 2).at(_positionCords.second - 1)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, true, _positionCords, std::make_pair(_positionCords.first - 2, _positionCords.second - 1)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, false, _positionCords, std::make_pair(_positionCords.first - 2, _positionCords.second - 1)));
				}
			}
		}

		if (static_cast<int>(_positionCords.first - 2) >= 0 && _positionCords.second + 1 < board.size()) { //Move 2 up + 1 right
			if (board.at(_positionCords.first - 2).at(_positionCords.second + 1)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first - 2).at(_positionCords.second + 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first - 2).at(_positionCords.second + 1)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, true, _positionCords, std::make_pair(_positionCords.first - 2, _positionCords.second + 1)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, false, _positionCords, std::make_pair(_positionCords.first - 2, _positionCords.second + 1)));
				}
			}
		}

		if (static_cast<int>(_positionCords.first - 1) >= 0 && _positionCords.second + 2 < board.size()) { //Move 1 up + 2 right
			if (board.at(_positionCords.first - 1).at(_positionCords.second + 2)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first - 1).at(_positionCords.second + 2) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first - 1).at(_positionCords.second + 2)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second + 2)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, false, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second + 2)));
				}
			}
		}

		if (_positionCords.first + 1 < board.size() && _positionCords.second + 2 < board.size()) { //Move 1 down + 2 right
			if (board.at(_positionCords.first + 1).at(_positionCords.second + 2)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first + 1).at(_positionCords.second + 2) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first + 1).at(_positionCords.second + 2)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second + 2)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, false, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second + 2)));
				}
			}
		}

		if (_positionCords.first + 2 < board.size() && _positionCords.second + 1 < board.size()) { //Move 2 down + 1 right
			if (board.at(_positionCords.first + 2).at(_positionCords.second + 1)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first + 2).at(_positionCords.second + 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first + 2).at(_positionCords.second + 1)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, true, _positionCords, std::make_pair(_positionCords.first + 2, _positionCords.second + 1)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, false, _positionCords, std::make_pair(_positionCords.first + 2, _positionCords.second + 1)));
				}
			}
		}

		if (_positionCords.first + 2 < board.size() && static_cast<int>(_positionCords.second - 1) >= 0) { //Move 2 down + 1 left
			if (board.at(_positionCords.first + 2).at(_positionCords.second - 1)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first + 2).at(_positionCords.second - 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first + 2).at(_positionCords.second - 1)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, true, _positionCords, std::make_pair(_positionCords.first + 2, _positionCords.second - 1)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, false, _positionCords, std::make_pair(_positionCords.first + 2, _positionCords.second - 1)));
				}
			}
		}

		if (_positionCords.first + 1 < board.size() && static_cast<int>(_positionCords.second - 2) >= 0) { //Move 1 down + 2 left
			if (board.at(_positionCords.first + 1).at(_positionCords.second - 2)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first + 1).at(_positionCords.second - 2) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first + 1).at(_positionCords.second - 2)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second - 2)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, false, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second - 2)));
				}
			}
		}

		if (static_cast<int>(_positionCords.first - 1) >= 0 && static_cast<int>(_positionCords.second - 2) >= 0) { //Move 1 up + 2 left
			if (board.at(_positionCords.first - 1).at(_positionCords.second - 2)->GetOwner() != _owner) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first - 1).at(_positionCords.second - 2) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner)) {
					if (board.at(_positionCords.first - 1).at(_positionCords.second - 2)->GetOwner() != OWNERS::NONE)
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second - 2)));
					else
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, false, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second - 2)));
				}
			}
		}

		PALE_ENGINE_TRACE("Number of moves for piece on cordinats ({0}, {1}): {2}", _positionCords.first, _positionCords.second, tempMoveList.size());
		return tempMoveList;
	}

	//--- Function definitions for rook class ---//
	Rook::Rook(OWNERS owner, unsigned int numberOfCopy) : Pieces(owner, 2), _firstMove(true) {
		try {
			if (numberOfCopy > _limitOfCopies - 1)
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 147, "Rook", COPY_LIMIT_EXCEEDED);

			if (owner == OWNERS::BLACK) {
				_value = -2;
				_name = "bR";

				_positionCords = std::make_pair(Piece_Starting_Positions::m_rookStartPos.first.at(numberOfCopy).first,
					Piece_Starting_Positions::m_rookStartPos.first.at(numberOfCopy).second);
			}
			else if (owner == OWNERS::WHITE) {
				_value = 2;
				_name = "wR";

				_positionCords = std::make_pair(Piece_Starting_Positions::m_rookStartPos.second.at(numberOfCopy).first,
					Piece_Starting_Positions::m_rookStartPos.second.at(numberOfCopy).second);
			}
			else
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 164, "Rook", FIGURE_BAD_OWNER);
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	Rook::Rook(OWNERS owner, std::pair<unsigned int, unsigned int> startingPos) : Pieces(owner, 2), _firstMove(true) {
		try {
			if (owner == OWNERS::BLACK) {
				_value = -2;
				_name = "bR";
			}
			else if (owner == OWNERS::WHITE) {
				_value = 2;
				_name = "wR";
			}
			else
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 164, "Rook", FIGURE_BAD_OWNER);

			_positionCords = startingPos;
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	bool Rook::MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
		bool canMove = true;

		//--- Do not allow rook piece to move different than linear ---//
		if (abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) > 0 && abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) > 0)
			canMove = false;

		//--- Path of rook move have to be clear ---//
		if (canMove) {
			if (!IsPathClear(_positionCords, endPos, board))
				canMove = false;
		}

		//--- Potential move cannot put king under check ---//
		if (canMove) {
			std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
			tempBoard.at(endPos.first).at(endPos.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
			tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
			if (KingIsChecked(tempBoard, _owner))
				canMove = false;
		}

		if (!canMove)
			PALE_ENGINE_TRACE("Rook piece cannot move to this location.");
		else {
			if (_firstMove)
				_firstMove = false;
		}
		// TODO: Check if work properly
		return canMove;
	}

	bool Rook::SpecialLogic(MOVE_TYPES type, std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board, std::optional<char> newPiece) {
		// IMPORTANT: REMEMBER! To perform castling, SpecialLogic method have to return 'true' for both King and Rook performing manover
		bool canPerform = true;
		if (endPos.second < _positionCords.second) { //Long castling
			//--- Rook can perform long castling only if it didn't move in this game ---//
			if (!_firstMove)
				canPerform = false;

			//--- Rook can perform long castling if king is on proper position ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK && board.at(Piece_Starting_Positions::m_kingStartPos.first.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.first.at(0).second)->GetValue() != -7) ||
					(_owner == OWNERS::WHITE && board.at(Piece_Starting_Positions::m_kingStartPos.first.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.second.at(0).second)->GetValue() != 7))
					canPerform = false;
			}

			//--- Rook can perform long castling if it is on proper position ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK && _positionCords != Piece_Starting_Positions::m_rookStartPos.first.at(0)) ||
					(_owner == OWNERS::WHITE && _positionCords != Piece_Starting_Positions::m_rookStartPos.second.at(0)))
					canPerform = false;
			}

			//--- Rook can perform long castling if king didn't move in this game ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK &&
					!std::static_pointer_cast<King>(board.at(Piece_Starting_Positions::m_kingStartPos.first.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.first.at(0).second))->DidNotMove()) ||
					(_owner == OWNERS::WHITE &&
						!std::static_pointer_cast<King>(board.at(Piece_Starting_Positions::m_kingStartPos.second.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.second.at(0).second))->DidNotMove()))
					canPerform = false;
			}

			//--- Rook can perform long castling if path between it and right king is clear ---//
			if (canPerform) {
				if (_owner == OWNERS::BLACK) {
					std::pair<unsigned int, unsigned int> tempEndPos = std::make_pair(Piece_Starting_Positions::m_kingStartPos.first.at(0).first,
						Piece_Starting_Positions::m_kingStartPos.first.at(0).second - 1);
					if (!IsPathClear(_positionCords, tempEndPos, board))
						canPerform = false;
				}
				else {
					std::pair<unsigned int, unsigned int> tempEndPos = std::make_pair(Piece_Starting_Positions::m_kingStartPos.second.at(0).first,
						Piece_Starting_Positions::m_kingStartPos.second.at(0).second - 1);
					if (!IsPathClear(_positionCords, tempEndPos, board))
						canPerform = false;
				}
			}

			//--- Rook cannot perform short castling if it will put king under check ---//
			if (canPerform) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				std::shared_ptr<Castling> tempSpecialStrategy = std::make_shared<Castling>(_positionCords, Piece_Starting_Positions::m_rookStartPos.first.at(0), tempBoard);
				tempSpecialStrategy->Execute();
				if (KingIsChecked(tempBoard, _owner))
					canPerform = false;
			}

			if (!canPerform)
				PALE_ENGINE_TRACE("Rook piece cannot perform special move.");
			else {
				if (_firstMove)
					_firstMove = false;

				if (_owner == OWNERS::BLACK) //Black perform castling
					_specialMove = std::make_shared<Castling>(Piece_Starting_Positions::m_kingStartPos.first.at(0), _positionCords, board);
				else //White perform castling
					_specialMove = std::make_shared<Castling>(Piece_Starting_Positions::m_kingStartPos.second.at(0), _positionCords, board);
			}
		}
		else { //Short castling
			//--- Rook can perform short castling only if it didn't move in this game ---//
			if (!_firstMove)
				canPerform = false;

			//--- Rook can perform short castling if king is on proper position ---//
			if ((_owner == OWNERS::BLACK && board.at(Piece_Starting_Positions::m_kingStartPos.first.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.first.at(0).second)->GetValue() != -7) ||
				(_owner == OWNERS::WHITE && board.at(Piece_Starting_Positions::m_kingStartPos.first.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.second.at(0).second)->GetValue() != 7))
				canPerform = false;

			//--- Rook can perform short castling if it is on proper position ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK && _positionCords != Piece_Starting_Positions::m_rookStartPos.first.at(1)) ||
					(_owner == OWNERS::WHITE && _positionCords != Piece_Starting_Positions::m_rookStartPos.second.at(1)))
					canPerform = false;
			}

			//--- Rook can perform long castling if king didn't move in this game ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK &&
					!std::static_pointer_cast<King>(board.at(Piece_Starting_Positions::m_kingStartPos.first.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.first.at(0).second))->DidNotMove()) ||
					(_owner == OWNERS::WHITE &&
						!std::static_pointer_cast<King>(board.at(Piece_Starting_Positions::m_kingStartPos.second.at(0).first).at(Piece_Starting_Positions::m_kingStartPos.second.at(0).second))->DidNotMove()))
					canPerform = false;
			}

			//--- Rook can perform short castling if path between it and right king is clear ---//
			if (canPerform) {
				if (_owner == OWNERS::BLACK) {
					std::pair<unsigned int, unsigned int> tempEndPos = std::make_pair(Piece_Starting_Positions::m_kingStartPos.first.at(0).first,
						Piece_Starting_Positions::m_kingStartPos.first.at(0).second + 1);
					if (!IsPathClear(_positionCords, tempEndPos, board))
						canPerform = false;
				}
				else {
					std::pair<unsigned int, unsigned int> tempEndPos = std::make_pair(Piece_Starting_Positions::m_kingStartPos.second.at(0).first,
						Piece_Starting_Positions::m_kingStartPos.second.at(0).second + 1);
					if (!IsPathClear(_positionCords, tempEndPos, board))
						canPerform = false;
				}
			}

			//--- Rook cannot perform short castling if it will put king under check ---//
			if (canPerform) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				std::shared_ptr<Castling> tempSpecialStrategy = std::make_shared<Castling>(_positionCords, Piece_Starting_Positions::m_rookStartPos.first.at(1), tempBoard);
				tempSpecialStrategy->Execute();
				if (KingIsChecked(tempBoard, _owner))
					canPerform = false;
			}

			if (!canPerform)
				PALE_ENGINE_TRACE("Rook piece cannot perform special move.");
			else {
				if (_firstMove)
					_firstMove = false;

				if (_owner == OWNERS::BLACK) //Black perform castling
					_specialMove = std::make_shared<Castling>(Piece_Starting_Positions::m_kingStartPos.first.at(0), _positionCords, board);
				else //White perform castling
					_specialMove = std::make_shared<Castling>(Piece_Starting_Positions::m_kingStartPos.second.at(0), _positionCords, board);
			}
		}
		// TODO: Check if work properly
		return canPerform;
	}

	std::vector<Move_Command> Rook::GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
		std::vector<Move_Command> tempMoveList;
		//--- Traditional moves ---//
		for (int it = 1; it < board.size(); it++) { 
			if (static_cast<int>(_positionCords.first - it) >= 0) { //Move north
				if (board.at(_positionCords.first - it).at(_positionCords.second)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first - it).at(_positionCords.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first - it).at(_positionCords.second)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, true, _positionCords, std::make_pair(_positionCords.first - it, _positionCords.second)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'R', _owner, false, _positionCords, std::make_pair(_positionCords.first - it, _positionCords.second)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (_positionCords.second + it < board.size()) { //Move east
				if (board.at(_positionCords.first).at(_positionCords.second + it)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first).at(_positionCords.second + it) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first).at(_positionCords.second + it)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, true, _positionCords, std::make_pair(_positionCords.first, _positionCords.second + it)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'R', _owner, false, _positionCords, std::make_pair(_positionCords.first, _positionCords.second + it)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (_positionCords.first + it < board.size()) { //Move south
				if (board.at(_positionCords.first + it).at(_positionCords.second)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first + it).at(_positionCords.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first + it).at(_positionCords.second)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, true, _positionCords, std::make_pair(_positionCords.first + it, _positionCords.second)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'R', _owner, false, _positionCords, std::make_pair(_positionCords.first + it, _positionCords.second)));
					}
				}
				else
					break;
			}
		}

		for (int it = 1; it < board.size(); it++) {
			if (static_cast<int>(_positionCords.second - it) >= 0) { //Move west
				if (board.at(_positionCords.first).at(_positionCords.second - it)->GetOwner() != _owner) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first).at(_positionCords.second - it) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner)) {
						if (board.at(_positionCords.first).at(_positionCords.second - it)->GetOwner() != OWNERS::NONE) {
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'N', _owner, true, _positionCords, std::make_pair(_positionCords.first, _positionCords.second - it)));
							break;
						}
						else
							tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'R', _owner, false, _positionCords, std::make_pair(_positionCords.first, _positionCords.second - it)));
					}
				}
				else
					break;
			}
		}

		//--- Special moves ---//
		if (_positionCords.second + 4 < board.size()) { //Long castling
			if (SpecialLogic(MOVE_TYPES::CASTLING, std::make_pair(_positionCords.first, _positionCords.second + 3), board, std::optional<char>()) &&
				abs(board.at(_positionCords.first).at(_positionCords.second + 4)->GetValue()) == 7) {
				if (board.at(_positionCords.first).at(_positionCords.second + 4)->SpecialLogic(MOVE_TYPES::CASTLING, std::make_pair(_positionCords.first, _positionCords.second + 2), board, std::optional<char>()))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::CASTLING, 'R', _owner, false, _positionCords, std::make_pair(_positionCords.first, _positionCords.second + 3)));
			}
		}

		if (static_cast<int>(_positionCords.second - 3) >= 0) { //Short castling
			if (SpecialLogic(MOVE_TYPES::CASTLING, std::make_pair(_positionCords.first, _positionCords.second - 2), board, std::optional<char>()) &&
				abs(board.at(_positionCords.first).at(_positionCords.second - 3)->GetValue()) == 7) {
				if (board.at(_positionCords.first).at(_positionCords.second - 3)->SpecialLogic(MOVE_TYPES::CASTLING, std::make_pair(_positionCords.first, _positionCords.second - 1), board, std::optional<char>()))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::CASTLING, 'R', _owner, false, _positionCords, std::make_pair(_positionCords.first, _positionCords.second - 2)));
			}
		}

		PALE_ENGINE_TRACE("Number of moves for piece on cordinats ({0}, {1}): {2}", _positionCords.first, _positionCords.second, tempMoveList.size());
		return tempMoveList;
	}

	//--- Function definitions for pawn class ---//
	Pawn::Pawn(OWNERS owner, unsigned int numberOfCopy) : Pieces(owner, 8), _firstMove(true), _movedByTwo(false) {
		try {
			if (numberOfCopy > _limitOfCopies - 1)
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 174, "Pawn", COPY_LIMIT_EXCEEDED);

			if (owner == OWNERS::BLACK) {
				_value = -1;
				_name = "bP";

				_positionCords = std::make_pair(Piece_Starting_Positions::m_pawnStartPos.first.at(numberOfCopy).first,
					Piece_Starting_Positions::m_pawnStartPos.first.at(numberOfCopy).second);
			}
			else if (owner == OWNERS::WHITE) {
				_value = 1;
				_name = "wP";

				_positionCords = std::make_pair(Piece_Starting_Positions::m_pawnStartPos.second.at(numberOfCopy).first,
					Piece_Starting_Positions::m_pawnStartPos.second.at(numberOfCopy).second);
			}
			else
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 191, "Pawn", FIGURE_BAD_OWNER);
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	Pawn::Pawn(OWNERS owner, std::pair<unsigned int, unsigned int> startingPos) : Pieces(owner, 8), _firstMove(true), _movedByTwo(false) {
		try {
			if (owner == OWNERS::BLACK) {
				_value = -1;
				_name = "bP";
			}
			else if (owner == OWNERS::WHITE) {
				_value = 1;
				_name = "wP";
			}
			else
				throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 164, "Pawn", FIGURE_BAD_OWNER);

			_positionCords = startingPos;
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	bool Pawn::MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
		bool canMove = true;

		//--- If pawn do not have first move it cannot move more than 1 plate ---//
		if (!_firstMove && abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) > 1)
			canMove = false;

		//-- Change on x cord can happened only if pawn want to capture enemy piece ---//
		if (abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) > 0 &&
			abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) != abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)))
			canMove = false;

		//-- Pawn can move diagonaly only to capture other piece and that have to be ENEMY piece ---//
		if (abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) == abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) &&
			(board.at(endPos.first).at(endPos.second)->GetOwner() == _owner || board.at(endPos.first).at(endPos.second)->GetOwner() == OWNERS::NONE))
			canMove = false;

		//--- Pawn piece can move only in direction of enemy edge of the board ---//
		if ((_owner == OWNERS::BLACK && static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first) > 0) ||
			(_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first) < 0))
			canMove = false;

		//--- Pawn piece cannot move on occupied plate ---//
		if (board.at(endPos.first).at(endPos.second)->GetOwner() != OWNERS::NONE)
			canMove = false;

		//--- Potential move cannot put king under check ---//
		if (canMove) {
			std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
			tempBoard.at(endPos.first).at(endPos.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
			tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
			if (KingIsChecked(tempBoard, _owner))
				canMove = false;
		}

		if (!canMove)
			PALE_ENGINE_TRACE("Pawn piece cannot move to this location.");
		else {
			if (abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) > 1 && _firstMove)
				_movedByTwo = true;
			else if (!_firstMove)
				_movedByTwo = false;

			if (_firstMove)
				_firstMove = false;
		}
		// TODO: Check if work properly
		return canMove;
	}

	bool Pawn::SpecialLogic(MOVE_TYPES type, std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board, std::optional<char> newPiece) {
		bool canPerform = true;
		if (type == MOVE_TYPES::PROMOTION) {
			//--- Pawn can move only by 1 block ---//
			if (abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) > 1)
				canPerform = false;

			//--- Promotion can be perform only if pawn reach oposit end of the board ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK && endPos.first != board.size() - 1) || (_owner == OWNERS::WHITE && endPos.first != 0))
					canPerform = false;
			}

			//--- To perform promotion target block cannot be occupied by same color piece ---//
			if (canPerform) {
				if (board.at(endPos.first).at(endPos.second)->GetOwner() == _owner)
					canPerform = false;
			}

			//--- Diagonal promotion can be perform only if pawn capture another piece ---//
			if (canPerform) {
				if (abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) == abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) &&
					board.at(endPos.first).at(endPos.second)->GetOwner() == OWNERS::NONE)
					canPerform = false;
			}
				
			//--- To perform promotion target LINEAR block needs to be empty ---//
			if (canPerform) {
				if (abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) == 0 && board.at(endPos.first).at(endPos.second)->GetValue() != 0)
					canPerform = false;
			}

			//--- Promotion cannot be performe if this action will put king under check ---//
			if (canPerform) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				std::shared_ptr<Promotion> tempSpecialStrategy = std::make_shared<Promotion>(_positionCords, endPos, tempBoard, newPiece.value());
				tempSpecialStrategy->Execute();
				if (KingIsChecked(tempBoard, _owner))
					canPerform = false;
			}

			if (!canPerform)
				PALE_ENGINE_TRACE("Pawn piece cannot perform special move.");
			else
				_specialMove = std::make_shared<Promotion>(_positionCords, endPos, board, newPiece.value());
		}
		else if (type == MOVE_TYPES::EN_PASSANT) {
			//--- En passant can be performe only if pawn made capture move ---//
			if (abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) != abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)))
				canPerform = false;

			//--- Pawn move cannot be longer than 1 block ---//
			if (canPerform) {
				if (abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) > 1 || abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) > 1)
					canPerform = false;
			}

			//--- Only enemy pawn can be capture by en passant ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK && board.at(endPos.first - 1).at(endPos.second)->GetValue() != 1) ||
					(_owner == OWNERS::WHITE && board.at(endPos.first + 1).at(endPos.second)->GetValue() != -1))
					canPerform = false;
			}

			//--- To perform en passant target block needs to be empty ---//
			if (canPerform) {
				if (board.at(endPos.first).at(endPos.second)->GetValue() != 0)
					canPerform = false;
			}

			//--- If capturing pawn did not moved by 2 blocks en passant cannot be perform ---//
			if (canPerform) {
				if ((_owner == OWNERS::BLACK && !std::static_pointer_cast<Pawn>(board.at(endPos.first - 1).at(endPos.second))->MovedByTwo()) ||
					(_owner == OWNERS::WHITE && !std::static_pointer_cast<Pawn>(board.at(endPos.first + 1).at(endPos.second))->MovedByTwo()))
					canPerform = false;
			}

			//--- En passant cannot be performe if this action will put king under check ---//
			if (canPerform) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				std::shared_ptr<En_Passant> tempSpecialStrategy = std::make_shared<En_Passant>(_positionCords, endPos, tempBoard, _owner);
				tempSpecialStrategy->Execute();
				if (KingIsChecked(tempBoard, _owner))
					canPerform = false;
			}

			if (!canPerform)
				PALE_ENGINE_TRACE("Pawn piece cannot perform special move.");
			else
				_specialMove = std::make_shared<En_Passant>(_positionCords, endPos, board, _owner);
		}
		// TODO: Check if work properly
		return canPerform;
	}

	std::vector<Move_Command> Pawn::GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
		std::vector<Move_Command> tempMoveList;
		//--- Traditional moves ---//
		//--- Non capturing moves ---//
		if (_owner == OWNERS::BLACK && _positionCords.first + 2 < board.size()) { //First move by 2 steps (BLACK)
			if (_firstMove && board.at(_positionCords.first + 2).at(_positionCords.second)->GetOwner() == OWNERS::NONE && 
				board.at(_positionCords.first + 1).at(_positionCords.second)->GetOwner() == OWNERS::NONE) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first + 2).at(_positionCords.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'P', _owner, false, _positionCords, std::make_pair(_positionCords.first + 2, _positionCords.second)));
			}
		}
		else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 2) >= 0) { //First move by 2 steps (WHITE)
			if (_firstMove && board.at(_positionCords.first - 2).at(_positionCords.second)->GetOwner() == OWNERS::NONE &&
				board.at(_positionCords.first - 1).at(_positionCords.second)->GetOwner() == OWNERS::NONE) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first - 2).at(_positionCords.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'P', _owner, false, _positionCords, std::make_pair(_positionCords.first - 2, _positionCords.second)));
			}
		}

		if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Traditional move by 1 steps (BLACK)
			if (board.at(_positionCords.first + 1).at(_positionCords.second)->GetOwner() == OWNERS::NONE) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first + 1).at(_positionCords.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'P', _owner, false, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second)));
			}
		}
		else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Traditional move by 1 steps (WHITE)
			if (board.at(_positionCords.first - 1).at(_positionCords.second)->GetOwner() == OWNERS::NONE) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				tempBoard.at(_positionCords.first - 1).at(_positionCords.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
				if (!KingIsChecked(tempBoard, _owner))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'P', _owner, false, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second)));
			}
		}

		//--- Capturing moves ---//
		if (_positionCords.second + 1 < board.size()) {
			if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Right capturing move (BLACK)
				if (board.at(_positionCords.first + 1).at(_positionCords.second + 1)->GetOwner() != _owner && 
					board.at(_positionCords.first + 1).at(_positionCords.second + 1)->GetOwner() != OWNERS::NONE) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first + 1).at(_positionCords.second + 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner))
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second + 1)));
				}
			}
			else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Right capturing move (WHITE)
				if (board.at(_positionCords.first - 1).at(_positionCords.second + 1)->GetOwner() != _owner &&
					board.at(_positionCords.first - 1).at(_positionCords.second + 1)->GetOwner() != OWNERS::NONE) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first - 1).at(_positionCords.second + 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner))
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second + 1)));
				}
			}
		}
		else if (static_cast<int>(_positionCords.second - 1) >= 0) {
			if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Left capturing move (BLACK)
				if (board.at(_positionCords.first + 1).at(_positionCords.second - 1)->GetOwner() != _owner &&
					board.at(_positionCords.first + 1).at(_positionCords.second - 1)->GetOwner() != OWNERS::NONE) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first + 1).at(_positionCords.second + 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner))
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second - 1)));
				}
			}
			else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Left capturing move (WHITE)
				if (board.at(_positionCords.first - 1).at(_positionCords.second - 1)->GetOwner() != _owner &&
					board.at(_positionCords.first - 1).at(_positionCords.second - 1)->GetOwner() != OWNERS::NONE) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					tempBoard.at(_positionCords.first - 1).at(_positionCords.second + 1) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = std::make_shared<Blank>(_positionCords.first, _positionCords.second);
					if (!KingIsChecked(tempBoard, _owner))
						tempMoveList.emplace_back(Move_Command(MOVE_TYPES::BASIC, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second - 1)));
				}
			}
		}

		//--- Special moves ---//
		//--- Traditional promotion ---//
		if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Traditional promotion in queen (BLACK)
			if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first + 1, _positionCords.second), board, 'Q'))
				tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, false, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second)));
		}
		else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Traditional promotion in queen (WHITE)
			if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first - 1, _positionCords.second), board, 'Q'))
				tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, false, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second)));
		}

		if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Traditional promotion in bishop (BLACK)
			if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first + 1, _positionCords.second), board, 'B'))
				tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, false, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second)));
		}
		else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Traditional promotion in bishop (WHITE)
			if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first - 1, _positionCords.second), board, 'B'))
				tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, false, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second)));
		}

		if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Traditional promotion in knight (BLACK)
			if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first + 1, _positionCords.second), board, 'N'))
				tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, false, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second)));
		}
		else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Traditional promotion in knight (WHITE)
			if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first - 1, _positionCords.second), board, 'N'))
				tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, false, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second)));
		}

		if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Traditional promotion in rook (BLACK)
			if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first + 1, _positionCords.second), board, 'R'))
				tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, false, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second)));
		}
		else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Traditional promotion in rook (WHITE)
			if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first - 1, _positionCords.second), board, 'R'))
				tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, false, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second)));
		}

		//--- Capturing promotion ---//
		if (_positionCords.second + 1 < board.size()) {
			if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Right capturing promotion in queen (BLACK)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first + 1, _positionCords.second + 1), board, 'Q'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second + 1)));
			}
			else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Right capturing promotion in queen (WHITE)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first - 1, _positionCords.second + 1), board, 'Q'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second + 1)));
			}

			if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Right capturing promotion in bishop (BLACK)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first + 1, _positionCords.second + 1), board, 'B'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second + 1)));
			}
			else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Right capturing promotion in bishop (WHITE)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first - 1, _positionCords.second + 1), board, 'B'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second + 1)));
			}

			if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Right capturing promotion in knight (BLACK)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first + 1, _positionCords.second + 1), board, 'N'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second + 1)));
			}
			else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Right capturing promotion in knight (WHITE)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first - 1, _positionCords.second + 1), board, 'N'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second + 1)));
			}

			if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Right capturing promotion in rook (BLACK)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first + 1, _positionCords.second + 1), board, 'R'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second + 1)));
			}
			else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Right capturing promotion in rook (WHITE)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first - 1, _positionCords.second + 1), board, 'R'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second + 1)));
			}
		}

		if (static_cast<int>(_positionCords.second - 1) >= 0) {
			if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Left capturing promotion in queen (BLACK)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first + 1, _positionCords.second - 1), board, 'Q'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second - 1)));
			}
			else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Left capturing promotion in queen (WHITE)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first - 1, _positionCords.second - 1), board, 'Q'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second - 1)));
			}

			if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Left capturing promotion in bishop (BLACK)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first + 1, _positionCords.second - 1), board, 'B'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second - 1)));
			}
			else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Left capturing promotion in bishop (WHITE)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first - 1, _positionCords.second - 1), board, 'B'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second - 1)));
			}

			if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Left capturing promotion in knight (BLACK)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first + 1, _positionCords.second - 1), board, 'N'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second - 1)));
			}
			else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Left capturing promotion in knight (WHITE)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first - 1, _positionCords.second - 1), board, 'N'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second - 1)));
			}

			if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Left capturing promotion in rook (BLACK)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first + 1, _positionCords.second - 1), board, 'R'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second - 1)));
			}
			else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Left capturing promotion in rook (WHITE)
				if (SpecialLogic(MOVE_TYPES::PROMOTION, std::make_pair(_positionCords.first - 1, _positionCords.second - 1), board, 'R'))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::PROMOTION, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second - 1)));
			}
		}

		//--- En passant ---//
		if (_positionCords.second + 1 < board.size()) {
			if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Right en passant (BLACK)
				if(SpecialLogic(MOVE_TYPES::EN_PASSANT, std::make_pair(_positionCords.first + 1, _positionCords.second + 1), board, std::optional<char>()))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::EN_PASSANT, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second + 1)));
			}
			else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Right en passant (WHITE)
				if (SpecialLogic(MOVE_TYPES::EN_PASSANT, std::make_pair(_positionCords.first - 1, _positionCords.second + 1), board, std::optional<char>()))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::EN_PASSANT, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second + 1)));
			}
		}
		else if (static_cast<int>(_positionCords.second - 1) >= 0) {
			if (_owner == OWNERS::BLACK && _positionCords.first + 1 < board.size()) { //Left en passant (BLACK)
				if (SpecialLogic(MOVE_TYPES::EN_PASSANT, std::make_pair(_positionCords.first + 1, _positionCords.second - 1), board, std::optional<char>()))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::EN_PASSANT, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first + 1, _positionCords.second - 1)));
			}
			else if (_owner == OWNERS::WHITE && static_cast<int>(_positionCords.first - 1) >= 0) { //Left en passant (WHITE)
				if (SpecialLogic(MOVE_TYPES::EN_PASSANT, std::make_pair(_positionCords.first - 1, _positionCords.second - 1), board, std::optional<char>()))
					tempMoveList.emplace_back(Move_Command(MOVE_TYPES::EN_PASSANT, 'P', _owner, true, _positionCords, std::make_pair(_positionCords.first - 1, _positionCords.second - 1)));
			}
		}

		PALE_ENGINE_TRACE("Number of moves for piece on cordinats ({0}, {1}): {2}", _positionCords.first, _positionCords.second, tempMoveList.size());
		return tempMoveList;
	}
}