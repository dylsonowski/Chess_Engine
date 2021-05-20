#pragma once
#include "../Engine_Macros.h"
#include "Pieces/Piece_Types.h"
//#include "../Log_System.h" todo: Check if necessary (if not delete).

namespace Pale {
	namespace Chess_Logic {
		//--- Enum with types of board representation. ---//
		enum class BOARD_TYPE {
			BASE = 0,
			INT_TYPE,
			STRING_TYPE,
			OBJECT_TYPE
		};

		//--- Structure containing data passed by move command ---//
		struct MoveCommand {
			std::string m_piece;
			PIECE_OWNER m_owner;
			std::pair<unsigned int, unsigned int> m_startPos, m_endPos;
		};

		static MoveCommand ProcessMoveCommand(std::string move); //todo: Complete definition of this function.

		//--- Base board representation template class (T :== int, string or Piece) ---//
		template<typename T>
		class Board_Representation {

		public:
			Board_Representation() = delete;
			Board_Representation(unsigned int row, unsigned int column);
			~Board_Representation() = default;

			void SetPlateValue(unsigned int rowIt, unsigned int columnIt, T value);
			void MovePiece(std::pair<unsigned int, unsigned int> startPos, std::pair<unsigned int, unsigned int> endPos, Pieces& piece);
			static void AddToDeathList(int figure) { 
				try {
					if (figure > 7 || figure == 6 || figure < -7 || figure == -6)
						throw INVALID_PIECE_ID;
					else
						s_deathList.emplace_back(figure);
				}
				catch (std::string errorMessage) { PALE_ENGINE_WARN(errorMessage); std::cin.get(); }
			}

			inline unsigned int GetRowNumbers() const { return _row; }
			inline unsigned int GetColumnNumber() const { return _column; }
			inline BOARD_TYPE GetBoardType() const { return _representationType; }
			inline T GetPlateValue(unsigned int rowIt, unsigned int columnIt) const {
				try {
					if (rowIt < _row && columnIt < _column)
						return _board.at(rowIt).at(columnIt);
					else
						throw VEC_OUT_OF_RANGE;
				}
				catch (std::string errorMessage) { PALE_ENGINE_ERROR(errorMessage); std::cin.get(); }
			}
			inline static bool IsFigureDead(int figure) {
				if (std::find(s_deathList.begin(), s_deathList.end(), figure) != s_deathList.end())
					return true;
				else
					return false;
			}

		private:
			unsigned int _row, _column;
			std::vector<std::vector<T>> _board;
			BOARD_TYPE _representationType; //Type of data which is used for representing board situation.
			static std::vector<int> s_deathList; //List which holds captured pieces. 
		};

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

			PALE_ENGINE_INFO("Board was successfully created.");
		}

		template<typename T>
		void Board_Representation<T>::SetPlateValue(unsigned int rowIt, unsigned int columnIt, T value) { //todo: Different! Only object will move.
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

					if (value != "bP" && value != "bR" && value != "bN" && value != "bB" && value != "bQ" && value != "bK"
						&& value != "wP" && value != "wR" && value != "wN" && value != "wB" && value != "wQ" && value != "wK" && value != "x")
						throw std::make_pair('w', INVALID_PIECE_ID);

					_board.at(rowIt).at(columnIt) = value;
					break;

				default:
					if (typeid(value) != typeid(std::shared_ptr<Pawn>) && typeid(value) != typeid(std::shared_ptr<Rook>) && typeid(value) != typeid(std::shared_ptr<Knight>)
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
		void Board_Representation<T>::MovePiece(std::pair<unsigned int, unsigned int> startPos, std::pair<unsigned int, unsigned int> endPos, Pieces& piece) {
			try {
				if (_board.at(startPos.first).at(startPos.second)->GetValue() != piece.GetValue())
					throw std::make_pair('w', INVALID_OCCUPATION);

				if (startPos.first < 0 || startPos.first >= _row || startPos.second < 0 || startPos.second >= _column ||
					endPos.first < 0 || endPos.first >= _row || endPos.second < 0 || endPos.second >= _column)
					throw std::make_pair('e', VEC_OUT_OF_RANGE);

				if (piece.MoveLogic(endPos, _board)) {
					PALE_ENGINE_INFO("Move was successfully made.");
					if (_board.at(endPos.first).at(endPos.second)->GetValue() != 0)
						_board.at(endPos.first).at(endPos.second) = std::make_shared<Blank>(endPos.first, endPos.second); //If capture set plate to blank

					_board.at(startPos.first).at(startPos.second).swap(_board.at(endPos.first).at(endPos.second)); //Swap contents of start and end plates.
					_board.at(startPos.first).at(startPos.second)->UpdatePosition(startPos); //Update positions for both plates.
					_board.at(endPos.first).at(endPos.second)->UpdatePosition(endPos);
				}
			}
			catch (std::pair<char, std::string> errorMessage) {
				if (errorMessage.first == 'w')
					PALE_ENGINE_WARN(errorMessage.second);
				else if (errorMessage.first == 'e')
					PALE_ENGINE_ERROR(errorMessage.second);
			}
		}
	}
}