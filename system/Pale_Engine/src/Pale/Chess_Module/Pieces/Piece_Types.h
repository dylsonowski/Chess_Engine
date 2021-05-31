#pragma once
//#include "../../Engine_Macros.h" todo: Check if necessary (if not delete).
#include "Pieces.h"

namespace Pale {
	namespace Chess_Logic {
		const struct Piece_Starting_Positions {
			static std::pair<std::vector<std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, unsigned int>>> m_kingStartPos;
			static std::pair<std::vector<std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, unsigned int>>> m_queenStartPos;
			static std::pair<std::vector<std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, unsigned int>>> m_bishopStartPos;
			static std::pair<std::vector<std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, unsigned int>>> m_knightStartPos;
			static std::pair<std::vector<std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, unsigned int>>> m_rookStartPos;
			static std::pair<std::vector<std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, unsigned int>>> m_pawnStartPos;
		};

		class King : public Pieces {

		public:
			King() = delete;
			King(OWNERS owner, unsigned int numberOfCopy);
			~King() = default;

			void SetKingStatus(bool setCheck, bool value) {
				if (setCheck)
					_check = value;
				else
					_checkMate = value;
			}

			bool SpecialLogic() override { return false; } //todo: Implement special move logic.
			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const override;
			inline bool GetKingStatus(bool getCheck) const {
				if (getCheck)
					return _check;
				else
					return _checkMate;
			}

		private:
			bool _check, _checkMate;
		};

		class Queen : public Pieces {

		public:
			Queen() = delete;
			Queen(OWNERS owner, unsigned int numberOfCopy);
			~Queen() = default;

			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const override;
		};

		class Bishop : public Pieces {

		public:
			Bishop() = delete;
			Bishop(OWNERS owner, unsigned int numberOfCopy);
			~Bishop() = default;

			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const override;
		};

		class Knight : public Pieces {

		public:
			Knight() = delete;
			Knight(OWNERS owner, unsigned int numberOfCopy);
			~Knight() = default;

			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const override;
		};

		class Rook : public Pieces {

		public:
			Rook() = delete;
			Rook(OWNERS owner, unsigned int numberOfCopy);
			~Rook() = default;

			bool SpecialLogic() override { return false; } //todo: Implement special move logic.
			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const override;
		};

		class Pawn : public Pieces {

		public:
			Pawn() = delete;
			Pawn(OWNERS owner, unsigned int numberOfCopy);
			~Pawn() = default;

			bool SpecialLogic() override { return false; } //todo: Implement special move logic.
			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const override;

		private:
			bool _firstMove;
		};

		class Blank : public Pieces {

		public:
			Blank() = delete;
			Blank(unsigned int rowCord, unsigned int columnCord) : Pieces(OWNERS::NONE, 64) {
				_value = 0;
				_name = "x";
				_positionCords = std::make_pair(rowCord, columnCord);
			}
			~Blank() = default;

			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const override { return false; }
		};
	}
}