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
			char m_piece;
			PIECE_OWNER m_owner;
			std::pair<unsigned int, unsigned int> m_startPos, m_endPos;
		};

		static const MoveCommand& ProcessMoveCommand(const std::string move); //todo: Complete definition of this function.

		//--- Base board representation template class (T :== int, string or Piece) ---//
		template<typename T>
		class Board_Representation {

		public:
			Board_Representation() = delete;
			Board_Representation(unsigned int row, unsigned int column, BOARD_TYPE type);
			~Board_Representation() = default;

			void SetPlateValue(unsigned int rowIt, unsigned int columnIt, T value);
			void SetBoardType(BOARD_TYPE type) { _representationType = type; }
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

			inline unsigned int GetRowNumber() const { return _row; }
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

			//--- Convertion functions ---//
			template<typename D>
			operator Board_Representation<D>() const {
				try {
					if (typeid(D) == typeid(T)) {
						Board_Representation<std::shared_ptr<Pieces>> newBoard(this->GetRowNumber(), this->GetColumnNumber());
						newBoard.SetBoardType(BOARD_TYPE::OBJECT_TYPE);
						for (int rowIt = 0; rowIt < this->GetRowNumber(); rowIt++) {
							for (int columnIt = 0; columnIt < this->GetColumnNumber(); columnIt++) {
								newBoard.SetPlateValue(rowIt, columnIt, this->GetPlateValue(rowIt, columnIt));
							}
						}
						PALE_ENGINE_TRACE("No convertion made!");
						return newBoard;
					}

					if (typeid(D) == typeid(int)) {
						Board_Representation<int> newBoard(this->GetRowNumber(), this->GetColumnNumber());
						newBoard.SetBoardType(BOARD_TYPE::INT_TYPE);
						for (int rowIt = 0; rowIt < this->GetRowNumber(); rowIt++) {
							for (int columnIt = 0; columnIt < this->GetColumnNumber(); columnIt++) {
								newBoard.SetPlateValue(rowIt, columnIt, this->GetPlateValue(rowIt, columnIt)->GetValue());
							}
						}
						PALE_ENGINE_TRACE("Convertion on int type!");
						return newBoard;
					}
					else if (typeid(D) == typeid(std::string)) {
						Board_Representation<std::string> newBoard(this->GetRowNumber(), this->GetColumnNumber());
						newBoard.SetBoardType(BOARD_TYPE::STRING_TYPE);
						for (int rowIt = 0; rowIt < this->GetRowNumber(); rowIt++) {
							for (int columnIt = 0; columnIt < this->GetColumnNumber(); columnIt++) {
								newBoard.SetPlateValue(rowIt, columnIt, this->GetPlateValue(rowIt, columnIt)->GetName());
							}
						}
						PALE_ENGINE_TRACE("Convertion on string type!");
						return newBoard;
					}
					else
						throw BAD_TYPE_CONVERTION;
				}
				catch (std::string errorMessage) { PALE_ENGINE_ERROR(errorMessage); std::cin.get(); }
			}

		private:
			unsigned int _row, _column;
			std::vector<std::vector<T>> _board;
			BOARD_TYPE _representationType; //Type of data which is used for representing board situation.
			static std::vector<int> s_deathList; //List which holds captured pieces. 
		};

		template<typename T>
		Board_Representation<T>::Board_Representation(unsigned int row, unsigned int column, BOARD_TYPE type) //todo: Have to choose type (vector<T> stay and fill regarding on a type
			: _row(row), _column(column), _representationType(type) {
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

			PALE_ENGINE_INFO("Board was successfully created! Board type: Object.");		
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

		const MoveCommand& ProcessMoveCommand(const std::string move) {
			try {
				MoveCommand processedMove;
				//Specifying owner of the piece.
				if (std::tolower(move[0]) == 'b')
					processedMove.m_owner = PIECE_OWNER::BLACK;
				else if (std::tolower(move[0]) == 'w')
					processedMove.m_owner = PIECE_OWNER::WHITE;
				else
					throw MOVE_COMMAND__WRONG_OWNER;

				//Specifying type of the piece.
				if (std::toupper(move[1]) != 'K' && std::toupper(move[1]) != 'Q' && std::toupper(move[1]) != 'B' 
					&& std::toupper(move[1]) != 'N' && std::toupper(move[1]) != 'R' && std::toupper(move[1]) != 'P')
					throw MOVE_COMMAND__WRONG_PIECE;
				else
					processedMove.m_piece = std::toupper(move[1]);

				int startX, startY, endX, endY;
				switch (move[2]) { //X cord for start position.
				case 'a': startX = 0;	break;
				case 'b': startX = 1;	break;
				case 'c': startX = 2;	break;
				case 'd': startX = 3;	break;
				case 'e': startX = 4;	break;
				case 'f': startX = 5;	break;
				case 'g': startX = 6;	break;
				case 'h': startX = 7;	break;
				default: throw MOVE_COMMAND__COORDINATE_OUT_OF_RANGE;	break;
				}

				switch (move[3]) { //Y cord for start position.
				case '1': startY = 7;	break;
				case '2': startY = 6;	break;
				case '3': startY = 5;	break;
				case '4': startY = 4;	break;
				case '5': startY = 3;	break;
				case '6': startY = 2;	break;
				case '7': startY = 1;	break;
				case '8': startY = 0;	break;
				default: throw MOVE_COMMAND__COORDINATE_OUT_OF_RANGE;	break;
				}

				switch (move[4]) { //X cord for end position.
				case 'a': endX = 0;	break;
				case 'b': endX = 1;	break;
				case 'c': endX = 2;	break;
				case 'd': endX = 3;	break;
				case 'e': endX = 4;	break;
				case 'f': endX = 5;	break;
				case 'g': endX = 6;	break;
				case 'h': endX = 7;	break;
				default: throw MOVE_COMMAND__COORDINATE_OUT_OF_RANGE;	break;
				}

				switch (move[5]) { //Y cord for end position.
				case '1': endY = 7;	break;
				case '2': endY = 6;	break;
				case '3': endY = 5;	break;
				case '4': endY = 4;	break;
				case '5': endY = 3;	break;
				case '6': endY = 2;	break;
				case '7': endY = 1;	break;
				case '8': endY = 0;	break;
				default: throw MOVE_COMMAND__COORDINATE_OUT_OF_RANGE;	break;
				}

				processedMove.m_startPos = std::make_pair(startY, startX); //first cord := row, second cord := column
				processedMove.m_endPos = std::make_pair(endY, endX);
				PALE_ENGINE_INFO("Move was successfully processed.");
				return processedMove; //todo: Check if work properly.
			}
			catch (std::string errorMessage) { PALE_ENGINE_ERROR(errorMessage); std::cin.get(); }
		}
	}
}