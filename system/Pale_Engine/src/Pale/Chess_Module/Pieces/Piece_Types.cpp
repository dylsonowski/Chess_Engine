#include "palepch.h"
#include "Piece_Types.h"

namespace Pale {
	namespace Chess_Logic {
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

		King::King(OWNERS owner, unsigned int numberOfCopy) : Pieces(owner, 1), _check(false), _checkMate(false) {
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

				_specialMove = std::make_shared<Castling>(); //todo: Implement castling
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
			}
		}

		bool King::MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const {
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
				if (KingIsChecked(board, _owner)) {
					std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
					std::shared_ptr<Pieces> tempPiece = tempBoard.at(endPos.first).at(endPos.second);
					tempBoard.at(endPos.first).at(endPos.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
					tempBoard.at(_positionCords.first).at(_positionCords.second) = tempPiece;
					if (KingIsChecked(tempBoard, _owner))
						canMove = false;
				}
			}

			if (!canMove)
				PALE_ENGINE_TRACE("King piece cannot move to this location.");
			//todo: Work but test needed!
			return canMove;
		}

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
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
			}
		}

		bool Queen::MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const {
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
				std::shared_ptr<Pieces> tempPiece = tempBoard.at(endPos.first).at(endPos.second);
				tempBoard.at(endPos.first).at(endPos.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = tempPiece;
				if (KingIsChecked(tempBoard, _owner))
					canMove = false;
			}

			if (!canMove)
				PALE_ENGINE_TRACE("Queen piece cannot move to this location.");
			//todo: Check if work properly
			return canMove;
		}

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
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
			}
		}

		bool Bishop::MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const {
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
				std::shared_ptr<Pieces> tempPiece = tempBoard.at(endPos.first).at(endPos.second);
				tempBoard.at(endPos.first).at(endPos.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = tempPiece;
				if (KingIsChecked(tempBoard, _owner))
					canMove = false;
			}

			if(!canMove)
				PALE_ENGINE_TRACE("Bishop piece cannot move to this location.");
			//todo: Check if work properly
			return canMove;
		}

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
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
			}
		}

		bool Knight::MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const {
			bool canMove = true;

			//--- Do not allow knight piece to move different than 2 linear + 1 diagonaly ---//
			if ((abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) != 2 && abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) != 1) ||
				(abs(static_cast<int>(_positionCords.first) - static_cast<int>(endPos.first)) != 1 && abs(static_cast<int>(_positionCords.second) - static_cast<int>(endPos.second)) != 2))
				canMove = false;

			//--- Knight piece cannot move on plate occupied by the same owners piece ---//
			if (board.at(endPos.first).at(endPos.second)->GetOwner() == _owner)
				canMove = false;
			
			//--- Potential move cannot put king under check ---//
			if (canMove) {
				std::vector<std::vector<std::shared_ptr<Pieces>>> tempBoard = board;
				std::shared_ptr<Pieces> tempPiece = tempBoard.at(endPos.first).at(endPos.second);
				tempBoard.at(endPos.first).at(endPos.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = tempPiece;
				if (KingIsChecked(tempBoard, _owner))
					canMove = false;
			}

			if (!canMove)
				PALE_ENGINE_TRACE("Knight piece cannot move to this location.");
			//todo: Check if work properly
			return canMove;
		}

		Rook::Rook(OWNERS owner, unsigned int numberOfCopy) : Pieces(owner, 2) {
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

				_specialMove = std::make_shared<Castling>(); //todo: Implement castling
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
			}
		}

		bool Rook::MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const {
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
				std::shared_ptr<Pieces> tempPiece = tempBoard.at(endPos.first).at(endPos.second);
				tempBoard.at(endPos.first).at(endPos.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = tempPiece;
				if (KingIsChecked(tempBoard, _owner))
					canMove = false;
			}

			if (!canMove)
				PALE_ENGINE_TRACE("Rook piece cannot move to this location.");
			//todo: Check if work properly
			return canMove;
		}

		Pawn::Pawn(OWNERS owner, unsigned int numberOfCopy) : Pieces(owner, 8), _firstMove(true) {
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

				_specialMove = std::make_shared<En_Passant>(); //todo: Implement en passant
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
			}
		}

		bool Pawn::MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const {
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
				std::shared_ptr<Pieces> tempPiece = tempBoard.at(endPos.first).at(endPos.second);
				tempBoard.at(endPos.first).at(endPos.second) = tempBoard.at(_positionCords.first).at(_positionCords.second);
				tempBoard.at(_positionCords.first).at(_positionCords.second) = tempPiece;
				if (KingIsChecked(tempBoard, _owner))
					canMove = false;
			}

			if (!canMove)
				PALE_ENGINE_TRACE("Pawn piece cannot move to this location.");
			//todo: Check if work properly
			return canMove;
		}
	}
}