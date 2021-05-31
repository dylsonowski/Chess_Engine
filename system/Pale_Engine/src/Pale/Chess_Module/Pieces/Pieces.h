#pragma once
#include "../../Engine_Macros.h"

namespace Pale {
	namespace Chess_Logic {
		enum class OWNERS {
			NONE = 0,
			WHITE,
			BLACK
		};

		class Special_Strategy;
		class Pieces {

		public:
			Pieces() = delete;
			virtual ~Pieces() = default;

			virtual void SpecialLogic() = 0;
			void UpdatePosition(std::pair<unsigned int, unsigned int> newPosition) { _positionCords = newPosition; }

			virtual bool MoveLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const = 0;
			inline std::pair<unsigned int, unsigned int> GetPosition() const { return _positionCords; }
			inline OWNERS GetOwner() const { return _owner; }
			inline int GetValue() const { return _value; }
			inline std::string GetName() const { return _name; }

		protected:
			Pieces(OWNERS owner, unsigned int limitOfCopies) : _value(0), _owner(owner), _limitOfCopies(limitOfCopies) {}

			int _value;
			std::string _name;
			unsigned int _limitOfCopies; //Limit for 1 player.
			std::pair<unsigned int, unsigned int> _positionCords;
			OWNERS _owner;
			std::shared_ptr<Special_Strategy> _specialMove;
		};

		//--- Methods allowing to decide if move is legal or not ---//
		static bool KingIsChecked(std::vector<std::vector<std::shared_ptr<Pieces>>>& board, OWNERS whichTurn) {
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

				if (!kingFound)
					throw PaleEngineException("Exception happened!", 'e', "Pieces.cpp", 241, "KingIsChecked", MOVE_COMMAND__NO_KING_ON_BOARD);

				//--- Check by rook or queen on the north ---//
				for (int it = 0; it < board.size(); it++) {
					if (kingPos.first - it < 0 || check)
						break;

					if (whichTurn == OWNERS::BLACK) {
						if (board.at(kingPos.first - it).at(kingPos.second)->GetValue() == 2 || board.at(kingPos.first - it).at(kingPos.second)->GetValue() == 5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first - it).at(kingPos.second)->GetOwner() == OWNERS::BLACK)
							break;
					}
					else {
						if (board.at(kingPos.first - it).at(kingPos.second)->GetValue() == -2 || board.at(kingPos.first - it).at(kingPos.second)->GetValue() == -5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first - it).at(kingPos.second)->GetOwner() == OWNERS::WHITE)
							break;
					}
				}

				//--- Check by rook or queen on the south ---//
				for (int it = 0; it < board.size(); it++) {
					if (kingPos.first + it >= board.size() || check)
						break;

					if (whichTurn == OWNERS::BLACK) {
						if (board.at(kingPos.first + it).at(kingPos.second)->GetValue() == 2 || board.at(kingPos.first + it).at(kingPos.second)->GetValue() == 5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first + it).at(kingPos.second)->GetOwner() == OWNERS::BLACK)
							break;
					}
					else {
						if (board.at(kingPos.first + it).at(kingPos.second)->GetValue() == -2 || board.at(kingPos.first + it).at(kingPos.second)->GetValue() == -5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first + it).at(kingPos.second)->GetOwner() == OWNERS::WHITE)
							break;
					}
				}

				//--- Check by rook or queen on the east ---//
				for (int it = 0; it < board.size(); it++) {
					if (kingPos.second + it >= board.size() || check)
						break;

					if (whichTurn == OWNERS::BLACK) {
						if (board.at(kingPos.first).at(kingPos.second + it)->GetValue() == 2 || board.at(kingPos.first).at(kingPos.second + it)->GetValue() == 5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first).at(kingPos.second + it)->GetOwner() == OWNERS::BLACK)
							break;
					}
					else {
						if (board.at(kingPos.first).at(kingPos.second + it)->GetValue() == -2 || board.at(kingPos.first).at(kingPos.second + it)->GetValue() == -5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first).at(kingPos.second + it)->GetOwner() == OWNERS::WHITE)
							break;
					}
				}

				//--- Check by rook or queen on the west ---//
				for (int it = 0; it < board.size(); it++) {
					if (kingPos.second - it < 0 || check)
						break;

					if (whichTurn == OWNERS::BLACK) {
						if (board.at(kingPos.first).at(kingPos.second - it)->GetValue() == 2 || board.at(kingPos.first).at(kingPos.second - it)->GetValue() == 5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first).at(kingPos.second - it)->GetOwner() == OWNERS::BLACK)
							break;
					}
					else {
						if (board.at(kingPos.first).at(kingPos.second - it)->GetValue() == -2 || board.at(kingPos.first).at(kingPos.second - it)->GetValue() == -5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first).at(kingPos.second - it)->GetOwner() == OWNERS::WHITE)
							break;
					}
				}

				//--- Check by bishop or queen on the north - west ---//
				for (int it = 0; it < board.size(); it++) {
					if (kingPos.first - it < 0 || kingPos.second - it < 0 || check)
						break;

					if (whichTurn == OWNERS::BLACK) {
						if (board.at(kingPos.first - it).at(kingPos.second - it)->GetValue() == 4 || board.at(kingPos.first - it).at(kingPos.second - it)->GetValue() == 5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first - it).at(kingPos.second - it)->GetOwner() == OWNERS::BLACK)
							break;
					}
					else {
						if (board.at(kingPos.first - it).at(kingPos.second - it)->GetValue() == -4 || board.at(kingPos.first - it).at(kingPos.second - it)->GetValue() == -5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first - it).at(kingPos.second - it)->GetOwner() == OWNERS::WHITE)
							break;
					}
				}

				//--- Check by bishop or queen on the north - east ---//
				for (int it = 0; it < board.size(); it++) {
					if (kingPos.first - it < 0 || kingPos.second + it >= board.size() || check)
						break;

					if (whichTurn == OWNERS::BLACK) {
						if (board.at(kingPos.first - it).at(kingPos.second + it)->GetValue() == 4 || board.at(kingPos.first - it).at(kingPos.second + it)->GetValue() == 5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first - it).at(kingPos.second + it)->GetOwner() == OWNERS::BLACK)
							break;
					}
					else {
						if (board.at(kingPos.first - it).at(kingPos.second + it)->GetValue() == -4 || board.at(kingPos.first - it).at(kingPos.second + it)->GetValue() == -5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first - it).at(kingPos.second + it)->GetOwner() == OWNERS::WHITE)
							break;
					}
				}

				//--- Check by bishop or queen on the south - east ---//
				for (int it = 0; it < board.size(); it++) {
					if (kingPos.first + it >= board.size() || kingPos.second + it >= board.size() || check)
						break;

					if (whichTurn == OWNERS::BLACK) {
						if (board.at(kingPos.first + it).at(kingPos.second + it)->GetValue() == 4 || board.at(kingPos.first + it).at(kingPos.second + it)->GetValue() == 5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first + it).at(kingPos.second + it)->GetOwner() == OWNERS::BLACK)
							break;
					}
					else {
						if (board.at(kingPos.first + it).at(kingPos.second + it)->GetValue() == -4 || board.at(kingPos.first + it).at(kingPos.second + it)->GetValue() == -5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first + it).at(kingPos.second + it)->GetOwner() == OWNERS::WHITE)
							break;
					}
				}

				//--- Check by bishop or queen on the south - west ---//
				for (int it = 0; it < board.size(); it++) {
					if (kingPos.first + it >= board.size() || kingPos.second - it < 0 || check)
						break;

					if (whichTurn == OWNERS::BLACK) {
						if (board.at(kingPos.first + it).at(kingPos.second - it)->GetValue() == 4 || board.at(kingPos.first + it).at(kingPos.second - it)->GetValue() == 5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first + it).at(kingPos.second - it)->GetOwner() == OWNERS::BLACK)
							break;
					}
					else {
						if (board.at(kingPos.first + it).at(kingPos.second - it)->GetValue() == -4 || board.at(kingPos.first + it).at(kingPos.second - it)->GetValue() == -5) {
							check = true;
							break;
						}
						else if (board.at(kingPos.first + it).at(kingPos.second - it)->GetOwner() == OWNERS::WHITE)
							break;
					}
				}

				//--- Check by knight ---//
				if (whichTurn == OWNERS::BLACK) {
					if ((kingPos.first - 2 >= 0 && kingPos.second - 1 >= 0 && board.at(kingPos.first - 2).at(kingPos.second - 1)->GetValue() == 3) ||
						(kingPos.first - 2 >= 0 && kingPos.second + 1 < board.size() && board.at(kingPos.first - 2).at(kingPos.second + 1)->GetValue() == 3) ||
						(kingPos.first - 1 >= 0 && kingPos.second + 2 < board.size() && board.at(kingPos.first - 1).at(kingPos.second + 2)->GetValue() == 3) ||
						(kingPos.first + 1 < board.size() && kingPos.second + 2 < board.size() && board.at(kingPos.first + 1).at(kingPos.second + 2)->GetValue() == 3) ||
						(kingPos.first + 2 < board.size() && kingPos.second + 1 < board.size() && board.at(kingPos.first + 2).at(kingPos.second + 1)->GetValue() == 3) ||
						(kingPos.first + 2 < board.size() && kingPos.second - 1 >= 0 && board.at(kingPos.first + 2).at(kingPos.second - 1)->GetValue() == 3) ||
						(kingPos.first + 1 < board.size() && kingPos.second - 2 >= 0 && board.at(kingPos.first + 1).at(kingPos.second - 2)->GetValue() == 3) ||
						(kingPos.first - 1 >= 0 && kingPos.second - 2 >= 0 && board.at(kingPos.first - 1).at(kingPos.second - 2)->GetValue() == 3) || !check)
						check = true; //todo: out of range exception.
				}
				else {
					if ((kingPos.first - 2 >= 0 && kingPos.second - 1 >= 0 && board.at(kingPos.first - 2).at(kingPos.second - 1)->GetValue() == -3) ||
						(kingPos.first - 2 >= 0 && kingPos.second + 1 < board.size() && board.at(kingPos.first - 2).at(kingPos.second + 1)->GetValue() == -3) ||
						(kingPos.first - 1 >= 0 && kingPos.second + 2 < board.size() && board.at(kingPos.first - 1).at(kingPos.second + 2)->GetValue() == -3) ||
						(kingPos.first + 1 < board.size() && kingPos.second + 2 < board.size() && board.at(kingPos.first + 1).at(kingPos.second + 2)->GetValue() == -3) ||
						(kingPos.first + 2 < board.size() && kingPos.second + 1 < board.size() && board.at(kingPos.first + 2).at(kingPos.second + 1)->GetValue() == -3) ||
						(kingPos.first + 2 < board.size() && kingPos.second - 1 >= 0 && board.at(kingPos.first + 2).at(kingPos.second - 1)->GetValue() == -3) ||
						(kingPos.first + 1 < board.size() && kingPos.second - 2 >= 0 && board.at(kingPos.first + 1).at(kingPos.second - 2)->GetValue() == -3) ||
						(kingPos.first - 1 >= 0 && kingPos.second - 2 >= 0 && board.at(kingPos.first - 1).at(kingPos.second - 2)->GetValue() == 3) || !check)
						check = true;
				}

				//--- Check by pawn ---//
				if (whichTurn == OWNERS::BLACK) {
					if ((kingPos.first + 1 < board.size() && kingPos.second - 1 >= 0 && board.at(kingPos.first + 1).at(kingPos.second - 1)->GetValue() == 1) ||
						(kingPos.first + 1 < board.size() && kingPos.second + 1 < board.size() && board.at(kingPos.first + 1).at(kingPos.second + 1)->GetValue() == 1) || !check)
						check = true;
				}
				else {
					if ((kingPos.first - 1 < board.size() && kingPos.second - 1 >= 0 && board.at(kingPos.first + 1).at(kingPos.second - 1)->GetValue() == -1) ||
						(kingPos.first - 1 < board.size() && kingPos.second + 1 < board.size() && board.at(kingPos.first + 1).at(kingPos.second + 1)->GetValue() == -1) || !check)
						check = true;
				}

				//--- Check by king ---//
				if (whichTurn == OWNERS::BLACK) {
					if ((kingPos.first - 1 >= 0 && kingPos.second - 1 >= 0 && board.at(kingPos.first - 1).at(kingPos.second - 1)->GetValue() == 7) ||
						(kingPos.first - 1 >= 0 && kingPos.second + 1 < board.size() && board.at(kingPos.first - 1).at(kingPos.second + 1)->GetValue() == 7) ||
						(kingPos.first + 1 < board.size() && kingPos.second + 1 < board.size() && board.at(kingPos.first + 1).at(kingPos.second + 1)->GetValue() == 7) ||
						(kingPos.first + 1 < board.size() && kingPos.second - 1 >= 0 && board.at(kingPos.first + 1).at(kingPos.second - 1)->GetValue() == 7) || !check)
						check = true;
				}
				else {
					if ((kingPos.first - 1 >= 0 && kingPos.second - 1 >= 0 && board.at(kingPos.first - 1).at(kingPos.second - 1)->GetValue() == -7) ||
						(kingPos.first - 1 >= 0 && kingPos.second + 1 < board.size() && board.at(kingPos.first - 1).at(kingPos.second + 1)->GetValue() == -7) ||
						(kingPos.first + 1 < board.size() && kingPos.second + 1 < board.size() && board.at(kingPos.first + 1).at(kingPos.second + 1)->GetValue() == -7) ||
						(kingPos.first + 1 < board.size() && kingPos.second - 1 >= 0 && board.at(kingPos.first + 1).at(kingPos.second - 1)->GetValue() == -7) || !check)
						check = true;
				}

				if (check)
					PALE_ENGINE_TRACE("King is under check.");

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

		static bool KingOnCheckPossition(std::pair<unsigned int, unsigned int> possition, std::vector<std::vector<std::shared_ptr<Pieces>>>& board, OWNERS whichTurn) {

		}

		//--- Structure of classes which define pieces special moves ---//
		class Special_Strategy {

		public:
			virtual ~Special_Strategy() = default;

			virtual void Execute() = 0;

		protected:
			Special_Strategy() = default;
		};

		class Promotion : public Special_Strategy {

		public:
			Promotion() = default; //todo: "= delete" later + custom Constructor.
			~Promotion() = default;

			void Execute() override { PALE_ENGINE_TRACE("Execution of Promotion maneuver."); }
		private:
			//todo: Finish up Promotion class.
		};

		class Castling : public Special_Strategy {

		public:
			Castling() = default; //todo: "= delete" later + custom Constructor.
			~Castling() = default;

			void Execute() override { PALE_ENGINE_TRACE("Execution of Castling maneuver."); }
		private:
			//todo: Finish up Castling class.
		};

		class En_Passant : public Special_Strategy {

		public:
			En_Passant() = default; //todo: "= delete" later + custom Constructor.
			~En_Passant() = default;

			void Execute() override { PALE_ENGINE_TRACE("Execution of En Passant maneuver."); }
		private:
			//todo: Finish up En Passant class.
		};

		class None : public Special_Strategy {

		public:
			None() = default;
			~None() = default;

			void Execute() override { PALE_ENGINE_TRACE("No special maneuver assigned."); }
		};
	}
}