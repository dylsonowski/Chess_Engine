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

		King::King(PIECE_OWNER owner, unsigned int numberOfCopy) : Pieces(owner, 1), _check(false), _checkMate(false) {
			try {
				if (numberOfCopy > _limitOfCopies - 1)
					throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 39, "King", COPY_LIMIT_EXCEEDED);

				if (owner == PIECE_OWNER::BLACK) {
					_value = -7;
					_name = "bK";

					_positionCords = std::make_pair(Piece_Starting_Positions::m_kingStartPos.first.at(numberOfCopy).first,
						Piece_Starting_Positions::m_kingStartPos.first.at(numberOfCopy).second);
				}
				else if (owner == PIECE_OWNER::WHITE) {
					_value = 7;
					_name = "wK";

					_positionCords = std::make_pair(Piece_Starting_Positions::m_kingStartPos.second.at(numberOfCopy).first,
						Piece_Starting_Positions::m_kingStartPos.second.at(numberOfCopy).second);
				}
				else
					throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 56, "King", FIGURE_BAD_OWNER);

				_specialMove = std::make_shared<Castling>(); //todo: Castling constructor will need arguments after implementing logic.
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
			}
		}

		Queen::Queen(PIECE_OWNER owner, unsigned int numberOfCopy) : Pieces(owner, 1) {
			try {
				if (numberOfCopy > _limitOfCopies - 1)
					throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 66, "Queen", COPY_LIMIT_EXCEEDED);

				if (owner == PIECE_OWNER::BLACK) {
					_value = -5;
					_name = "bQ";

					_positionCords = std::make_pair(Piece_Starting_Positions::m_queenStartPos.first.at(numberOfCopy).first,
						Piece_Starting_Positions::m_queenStartPos.first.at(numberOfCopy).second);
				}
				else if (owner == PIECE_OWNER::WHITE) {
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

		Bishop::Bishop(PIECE_OWNER owner, unsigned int numberOfCopy) : Pieces(owner, 2) {
			try {
				if (numberOfCopy > _limitOfCopies - 1)
					throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 93, "Bishop", COPY_LIMIT_EXCEEDED);

				if (owner == PIECE_OWNER::BLACK) {
					_value = -4;
					_name = "bB";

					_positionCords = std::make_pair(Piece_Starting_Positions::m_bishopStartPos.first.at(numberOfCopy).first,
						Piece_Starting_Positions::m_bishopStartPos.first.at(numberOfCopy).second);
				}
				else if (owner == PIECE_OWNER::WHITE) {
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

		Knight::Knight(PIECE_OWNER owner, unsigned int numberOfCopy) : Pieces(owner, 2) {
			try {
				if (numberOfCopy > _limitOfCopies - 1)
					throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 120, "Knight", COPY_LIMIT_EXCEEDED);

				if (owner == PIECE_OWNER::BLACK) {
					_value = -3;
					_name = "bN";

					_positionCords = std::make_pair(Piece_Starting_Positions::m_knightStartPos.first.at(numberOfCopy).first,
						Piece_Starting_Positions::m_knightStartPos.first.at(numberOfCopy).second);
				}
				else if (owner == PIECE_OWNER::WHITE) {
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

		Rook::Rook(PIECE_OWNER owner, unsigned int numberOfCopy) : Pieces(owner, 2) {
			try {
				if (numberOfCopy > _limitOfCopies - 1)
					throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 147, "Rook", COPY_LIMIT_EXCEEDED);

				if (owner == PIECE_OWNER::BLACK) {
					_value = -2;
					_name = "bR";

					_positionCords = std::make_pair(Piece_Starting_Positions::m_rookStartPos.first.at(numberOfCopy).first,
						Piece_Starting_Positions::m_rookStartPos.first.at(numberOfCopy).second);
				}
				else if (owner == PIECE_OWNER::WHITE) {
					_value = 2;
					_name = "wR";

					_positionCords = std::make_pair(Piece_Starting_Positions::m_rookStartPos.second.at(numberOfCopy).first,
						Piece_Starting_Positions::m_rookStartPos.second.at(numberOfCopy).second);
				}
				else
					throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 164, "Rook", FIGURE_BAD_OWNER);

				_specialMove = std::make_shared<Castling>(); //todo: Castling constructor will need arguments after implementing logic.
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
			}
		}

		Pawn::Pawn(PIECE_OWNER owner, unsigned int numberOfCopy) : Pieces(owner, 8), _firstMove(true) {
			try {
				if (numberOfCopy > _limitOfCopies - 1)
					throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 174, "Pawn", COPY_LIMIT_EXCEEDED);

				if (owner == PIECE_OWNER::BLACK) {
					_value = -1;
					_name = "bP";

					_positionCords = std::make_pair(Piece_Starting_Positions::m_pawnStartPos.first.at(numberOfCopy).first,
						Piece_Starting_Positions::m_pawnStartPos.first.at(numberOfCopy).second);
				}
				else if (owner == PIECE_OWNER::WHITE) {
					_value = 1;
					_name = "wP";

					_positionCords = std::make_pair(Piece_Starting_Positions::m_pawnStartPos.second.at(numberOfCopy).first,
						Piece_Starting_Positions::m_pawnStartPos.second.at(numberOfCopy).second);
				}
				else
					throw PaleEngineException("Exception happened!", 'e', "Piece_Types.cpp", 191, "Pawn", FIGURE_BAD_OWNER);

				_specialMove = std::make_shared<En_Passant>(); //todo: En Passant constructor will need arguments after implementing logic.
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