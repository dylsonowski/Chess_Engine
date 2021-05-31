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
			void ExecuteSpecialMove() override { _specialMove->Execute(); }

			bool SpecialLogic(MOVE_TYPES type, std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board, std::optional<char> newPiece) override;
			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override;
			std::vector<Move_Command> GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override;
			inline bool GetKingStatus(bool getCheck) const {
				if (getCheck)
					return _check;
				else
					return _checkMate;
			}
			inline bool DidNotMove() const { return _firstMove; }

		private:
			bool _check, _checkMate, _firstMove;
		};

		class Queen : public Pieces {

		public:
			Queen() = delete;
			Queen(OWNERS owner, unsigned int numberOfCopy);
			Queen(OWNERS owner, std::pair<unsigned int, unsigned int> startingPos);
			~Queen() = default;

			void ExecuteSpecialMove() override { _specialMove->Execute(); }

			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override;
			std::vector<Move_Command> GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override;
		};

		class Bishop : public Pieces {

		public:
			Bishop() = delete;
			Bishop(OWNERS owner, unsigned int numberOfCopy);
			Bishop(OWNERS owner, std::pair<unsigned int, unsigned int> startingPos);
			~Bishop() = default;

			void ExecuteSpecialMove() override { _specialMove->Execute(); }

			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override;
			std::vector<Move_Command> GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override;
		};

		class Knight : public Pieces {

		public:
			Knight() = delete;
			Knight(OWNERS owner, unsigned int numberOfCopy);
			Knight(OWNERS owner, std::pair<unsigned int, unsigned int> startingPos);
			~Knight() = default;

			void ExecuteSpecialMove() override { _specialMove->Execute(); }

			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override;
			std::vector<Move_Command> GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override;
		};

		class Rook : public Pieces {

		public:
			Rook() = delete;
			Rook(OWNERS owner, unsigned int numberOfCopy);
			Rook(OWNERS owner, std::pair<unsigned int, unsigned int> startingPos);
			~Rook() = default;

			void ExecuteSpecialMove() override { _specialMove->Execute(); }

			bool SpecialLogic(MOVE_TYPES type, std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board, std::optional<char> newPiece) override;
			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override;
			std::vector<Move_Command> GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override;
			inline bool DidNotMove() const { return _firstMove; }

		private:
			bool _firstMove;
		};

		class Pawn : public Pieces {

		public:
			Pawn() = delete;
			Pawn(OWNERS owner, unsigned int numberOfCopy);
			~Pawn() = default;

			void ExecuteSpecialMove() override { _specialMove->Execute(); }

			bool SpecialLogic(MOVE_TYPES type, std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board, std::optional<char> newPiece) override;
			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override;
			std::vector<Move_Command> GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override { return std::vector<Move_Command>(); }

			inline bool MovedByTwo() const { return _movedByTwo; }

		private:
			bool _firstMove, _movedByTwo;
		};

		class Blank : public Pieces {

		public:
			Blank() = delete;
			Blank(unsigned int rowCord, unsigned int columnCord) : Pieces(OWNERS::NONE, 64) {
				_value = 0;
				_name = "x";
				_positionCords = std::make_pair(rowCord, columnCord);
				_specialMove = std::make_shared<None>();
			}
			~Blank() = default;

			void ExecuteSpecialMove() override { _specialMove->Execute(); }

			bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override { return false; }
			std::vector<Move_Command> GenerateLegitMoves(std::vector<std::vector<std::shared_ptr<Pieces>>>& board) override { return {}; }
		};
	}
}