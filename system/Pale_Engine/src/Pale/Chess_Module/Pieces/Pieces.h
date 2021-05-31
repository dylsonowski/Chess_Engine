#pragma once
#include "../../Engine_Macros.h"

namespace Pale {
	namespace Chess_Logic {
		enum class OWNERS {
			NONE = 0,
			WHITE,
			BLACK
		};

		enum class MOVE_DIRECTION {
			NONE = 0,
			VERTICAL,
			HORIZONTAL,
			DIAGONAL
		};

		enum class MOVE_TYPES {
			BASIC = 0,
			PROMOTION,
			CASTLING,
			EN_PASSANT
		};

		class Special_Strategy;
		//extern class Queen;
		//extern class Bishop;
		//extern class Knight;
		//extern class Rook;
		//extern class Blank;
		class Pieces {

		public:
			Pieces() = delete;
			virtual ~Pieces() = default;

			void UpdatePosition(std::pair<unsigned int, unsigned int> newPosition) { _positionCords = newPosition; }
			virtual void ExecuteSpecialMove() = 0;

			virtual bool SpecialLogic(std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board, std::optional<char> newPiece) { return true; }
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
					throw PaleEngineException("Exception happened!", 'e', "Pieces.cpp", 65, "KingIsChecked", MOVE_COMMAND__NO_KING_ON_BOARD);

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
					if (!check) {
						if (static_cast<int>(kingPos.first - 2) >= 0 && static_cast<int>(kingPos.second - 1) >= 0) {
							if (board.at(kingPos.first - 2).at(kingPos.second - 1)->GetValue() == 3)
								check = true;
						}

						if (static_cast<int>(kingPos.first - 2) >= 0 && kingPos.second + 1 < board.size()) {
							if (board.at(kingPos.first - 2).at(kingPos.second + 1)->GetValue() == 3)
								check = true;
						}

						if (static_cast<int>(kingPos.first - 1) >= 0 && kingPos.second + 2 < board.size()) {
							if (board.at(kingPos.first - 1).at(kingPos.second + 2)->GetValue() == 3)
								check = true;
						}

						if (kingPos.first + 1 < board.size() && kingPos.second + 2 < board.size()) {
							if (board.at(kingPos.first + 1).at(kingPos.second + 2)->GetValue() == 3)
								check = true;
						}

						if (kingPos.first + 2 < board.size() && kingPos.second + 1 < board.size()) {
							if (board.at(kingPos.first + 2).at(kingPos.second + 1)->GetValue() == 3)
								check = true;
						}

						if (kingPos.first + 2 < board.size() && static_cast<int>(kingPos.second - 1) >= 0) {
							if (board.at(kingPos.first + 2).at(kingPos.second - 1)->GetValue() == 3)
								check = true;
						}

						if (kingPos.first + 1 < board.size() && static_cast<int>(kingPos.second - 2) >= 0) {
							if (board.at(kingPos.first + 1).at(kingPos.second - 2)->GetValue() == 3)
								check = true;
						}

						if (static_cast<int>(kingPos.first - 1) >= 0 && static_cast<int>(kingPos.second - 2) >= 0) {
							if (board.at(kingPos.first - 1).at(kingPos.second - 2)->GetValue() == 3)
								check = true;
						}
					}
				}
				else {
					if (!check) {
						if (static_cast<int>(kingPos.first - 2) >= 0 && static_cast<int>(kingPos.second - 1) >= 0) {
							if (board.at(kingPos.first - 2).at(kingPos.second - 1)->GetValue() == -3)
								check = true;
						}

						if (static_cast<int>(kingPos.first - 2) >= 0 && kingPos.second + 1 < board.size()) {
							if (board.at(kingPos.first - 2).at(kingPos.second + 1)->GetValue() == -3)
								check = true;
						}

						if (static_cast<int>(kingPos.first - 1) >= 0 && kingPos.second + 2 < board.size()) {
							if (board.at(kingPos.first - 1).at(kingPos.second + 2)->GetValue() == -3)
								check = true;
						}

						if (kingPos.first + 1 < board.size() && kingPos.second + 2 < board.size()) {
							if (board.at(kingPos.first + 1).at(kingPos.second + 2)->GetValue() == -3)
								check = true;
						}

						if (kingPos.first + 2 < board.size() && kingPos.second + 1 < board.size()) {
							if (board.at(kingPos.first + 2).at(kingPos.second + 1)->GetValue() == -3)
								check = true;
						}

						if (kingPos.first + 2 < board.size() && static_cast<int>(kingPos.second - 1) >= 0) {
							if (board.at(kingPos.first + 2).at(kingPos.second - 1)->GetValue() == -3)
								check = true;
						}

						if (kingPos.first + 1 < board.size() && static_cast<int>(kingPos.second - 2) >= 0) {
							if (board.at(kingPos.first + 1).at(kingPos.second - 2)->GetValue() == -3)
								check = true;
						}

						if (static_cast<int>(kingPos.first - 1) >= 0 && static_cast<int>(kingPos.second - 2) >= 0) {
							if (board.at(kingPos.first - 1).at(kingPos.second - 2)->GetValue() == -3)
								check = true;
						}
					}
				}

				//--- Check by pawn ---//
				if (whichTurn == OWNERS::BLACK) {
					if (!check) {
						if (kingPos.first + 1 < board.size() && static_cast<int>(kingPos.second - 1) >= 0) {
							if (board.at(kingPos.first + 1).at(kingPos.second - 1)->GetValue() == 1)
								check = true;
						}

						if (kingPos.first + 1 < board.size() && kingPos.second + 1 < board.size()) {
							if (board.at(kingPos.first + 1).at(kingPos.second + 1)->GetValue() == 1)
								check = true;
						}
					}
				}
				else {
					if (!check) {
						if (static_cast<int>(kingPos.first - 1) < board.size() && static_cast<int>(kingPos.second - 1) >= 0) {
							if (board.at(kingPos.first - 1).at(kingPos.second - 1)->GetValue() == -1)
								check = true;
						}

						if (static_cast<int>(kingPos.first - 1) < board.size() && kingPos.second + 1 < board.size()) {
							if (board.at(kingPos.first - 1).at(kingPos.second + 1)->GetValue() == -1)
								check = true;
						}
					}
				}

				//--- Check by king ---//
				if (whichTurn == OWNERS::BLACK) {
					if (!check) {
						if (static_cast<int>(kingPos.first - 1) >= 0) {
							if(board.at(kingPos.first - 1).at(kingPos.second)->GetValue() == 7)
								check = true;
						}

						if (static_cast<int>(kingPos.first - 1) >= 0 && kingPos.second + 1 < board.size()) {
							if(board.at(kingPos.first - 1).at(kingPos.second + 1)->GetValue() == 7)
								check = true;
						}

						if (kingPos.second + 1 < board.size()) {
							if (board.at(kingPos.first).at(kingPos.second + 1)->GetValue() == 7)
								check = true;
						}

						if (kingPos.first + 1 < board.size() && kingPos.second + 1 < board.size()) {
							if (board.at(kingPos.first + 1).at(kingPos.second + 1)->GetValue() == 7)
								check = true;
						}

						if (kingPos.first + 1 < board.size()) {
							if (board.at(kingPos.first + 1).at(kingPos.second)->GetValue() == 7)
								check = true;
						}

						if (kingPos.first + 1 < board.size() && static_cast<int>(kingPos.second - 1) >= 0) {
							if (board.at(kingPos.first + 1).at(kingPos.second - 1)->GetValue() == 7)
								check = true;
						}

						if (static_cast<int>(kingPos.second - 1) >= 0) {
							if (board.at(kingPos.first).at(kingPos.second - 1)->GetValue() == 7)
								check = true;
						}

						if (static_cast<int>(kingPos.first - 1) >= 0 && static_cast<int>(kingPos.second - 1) >= 0) {
							if (board.at(kingPos.first - 1).at(kingPos.second - 1)->GetValue() == 7)
								check = true;
						}
					}
				}
				else {
					if (!check) {
						if (static_cast<int>(kingPos.first - 1) >= 0) {
							if (board.at(kingPos.first - 1).at(kingPos.second)->GetValue() == -7)
								check = true;
						}

						if (static_cast<int>(kingPos.first - 1) >= 0 && kingPos.second + 1 < board.size()) {
							if (board.at(kingPos.first - 1).at(kingPos.second + 1)->GetValue() == -7)
								check = true;
						}

						if (kingPos.second + 1 < board.size()) {
							if (board.at(kingPos.first).at(kingPos.second + 1)->GetValue() == -7)
								check = true;
						}

						if (kingPos.first + 1 < board.size() && kingPos.second + 1 < board.size()) {
							if (board.at(kingPos.first + 1).at(kingPos.second + 1)->GetValue() == -7)
								check = true;
						}

						if (kingPos.first + 1 < board.size()) {
							if (board.at(kingPos.first + 1).at(kingPos.second)->GetValue() == -7)
								check = true;
						}

						if (kingPos.first + 1 < board.size() && static_cast<int>(kingPos.second - 1) >= 0) {
							if (board.at(kingPos.first + 1).at(kingPos.second - 1)->GetValue() == -7)
								check = true;
						}

						if (static_cast<int>(kingPos.second - 1) >= 0) {
							if (board.at(kingPos.first).at(kingPos.second - 1)->GetValue() == -7)
								check = true;
						}

						if (static_cast<int>(kingPos.first - 1) >= 0 && static_cast<int>(kingPos.second - 1) >= 0) {
							if (board.at(kingPos.first - 1).at(kingPos.second - 1)->GetValue() == -7)
								check = true;
						}
					}
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

		static bool IsPathClear(std::pair<unsigned int, unsigned int> startPos, std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) {
			try {
				bool pathClear = true;
				MOVE_DIRECTION moveDirection;

				if (abs(static_cast<int>(startPos.first) - static_cast<int>(endPos.first)) > 0 && abs(static_cast<int>(startPos.second) - static_cast<int>(endPos.second)) == 0)
					moveDirection = MOVE_DIRECTION::VERTICAL;
				else if (abs(static_cast<int>(startPos.first) - static_cast<int>(endPos.first)) == 0 && abs(static_cast<int>(startPos.second) - static_cast<int>(endPos.second)) > 0)
					moveDirection = MOVE_DIRECTION::HORIZONTAL;
				else if (abs(static_cast<int>(startPos.first) - static_cast<int>(endPos.first)) == abs(static_cast<int>(startPos.second) - static_cast<int>(endPos.second)))
					moveDirection = MOVE_DIRECTION::DIAGONAL;
				else
					throw PaleEngineException("Exception happened!", 'e', "Pieces.h", 482, "IsPathClear", MOVE_COMMAND__INVALID_MOVE_DIRECTION);

				switch (moveDirection) {
				case MOVE_DIRECTION::VERTICAL:
					//--- Move up ---//
					if (startPos.first > endPos.first) {
						for (int it = 1; it <= abs(static_cast<int>(startPos.first) - static_cast<int>(endPos.first)); it++) {
							if (board.at(startPos.first - it).at(startPos.second)->GetValue() != 0) {
								pathClear = false;
								break;
							}
						}
					}
					//--- Move down ---//
					else {
						for (int it = 1; it <= abs(static_cast<int>(startPos.first) - static_cast<int>(endPos.first)); it++) {
							if (board.at(startPos.first + it).at(startPos.second)->GetValue() != 0) {
								pathClear = false;
								break;
							}
						}
					}
					break;

				case MOVE_DIRECTION::HORIZONTAL:
					//--- Move left ---//
					if (startPos.second > endPos.second) {
						for (int it = 1; it <= abs(static_cast<int>(startPos.second) - static_cast<int>(endPos.second)); it++) {
							if (board.at(startPos.first).at(startPos.second - it) != 0) {
								pathClear = false;
								break;
							}
						}
					}
					//--- Move right ---//
					else {
						for (int it = 1; it <= abs(static_cast<int>(startPos.second) - static_cast<int>(endPos.second)); it++) {
							if (board.at(startPos.first).at(startPos.second + it) != 0) {
								pathClear = false;
								break;
							}
						}
					}
					break;

				case MOVE_DIRECTION::DIAGONAL:
					//--- Move up left ---//
					if (startPos.first > endPos.first && startPos.second > endPos.second) {
						for (int it = 1; it <= abs(static_cast<int>(startPos.first) - static_cast<int>(endPos.first)); it++) {
							if (board.at(startPos.first - it).at(startPos.second - it)) {
								pathClear = false;
								break;
							}
						}
					}
					//--- Move up right ---/
					else if (startPos.first > endPos.first && startPos.second < endPos.second) {
						for (int it = 1; it <= abs(static_cast<int>(startPos.first) - static_cast<int>(endPos.first)); it++) {
							if (board.at(startPos.first - it).at(startPos.second + it)) {
								pathClear = false;
								break;
							}
						}
					}
					//--- Move down right ---//
					else if (startPos.first < endPos.first && startPos.second < endPos.second) {
						for (int it = 1; it <= abs(static_cast<int>(startPos.first) - static_cast<int>(endPos.first)); it++) {
							if (board.at(startPos.first + it).at(startPos.second + it)) {
								pathClear = false;
								break;
							}
						}
					}
					//--- Move down left ---//
					else {
						for (int it = 1; it <= abs(static_cast<int>(startPos.first) - static_cast<int>(endPos.first)); it++) {
							if (board.at(startPos.first + it).at(startPos.second - it)) {
								pathClear = false;
								break;
							}
						}
					}
					break;
				}

				if (!pathClear)
					PALE_ENGINE_TRACE("Path is blocked by another piece.");

				return pathClear;
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
			}
		}	

		//--- Classes which define pieces special moves ---//
		class Special_Strategy {

		public:
			virtual ~Special_Strategy() = default;
			virtual void Execute() = 0;

		protected:
			Special_Strategy() = default;
		};

		class Promotion : public Special_Strategy {

		public:
			Promotion() = delete;
			Promotion(std::pair<unsigned int, unsigned int> startPos, std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board, char newPiece) 
				: Special_Strategy(), _boardRef(board), _startPos(startPos), _endPos(endPos), _newPiece(newPiece) {}
			~Promotion() = default;

			void Execute() override;

		private:
			std::vector<std::vector<std::shared_ptr<Pieces>>>& _boardRef;
			std::pair<unsigned int, unsigned int> _startPos, _endPos;
			char _newPiece;
		};

		class Castling : public Special_Strategy {

		public:
			Castling() = delete;
			Castling(std::pair<unsigned int, unsigned int> kingPos, std::pair<unsigned int, unsigned int> rookPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) 
				: Special_Strategy(), _kingPos(kingPos), _rookPos(rookPos), _boardRef(board) {}
			~Castling() = default;

			void Execute() override;

		private:
			std::vector<std::vector<std::shared_ptr<Pieces>>>& _boardRef;
			std::pair<unsigned int, unsigned int> _kingPos, _rookPos;
		};

		class En_Passant : public Special_Strategy {

		public:
			En_Passant() = delete;
			En_Passant(std::pair<unsigned int, unsigned int> startPos, std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board, OWNERS whichTurn) 
				: Special_Strategy(), _startPos(startPos), _endPos(endPos), _boardRef(board), _whichTurn(whichTurn) {}
			~En_Passant() = default;

			void Execute() override;

		private:
			std::pair<unsigned int, unsigned int> _startPos, _endPos;
			std::vector<std::vector<std::shared_ptr<Pieces>>>& _boardRef;
			OWNERS _whichTurn;
		};

		class None : public Special_Strategy {

		public:
			None() : Special_Strategy() {}
			~None() = default;

			void Execute() override { PALE_ENGINE_INFO("This piece do not have any special move to execute!"); }
		};
	}
}