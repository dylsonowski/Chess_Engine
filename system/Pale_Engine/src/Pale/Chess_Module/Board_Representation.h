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

		static const MoveCommand& ProcessMoveCommand(const std::string move);

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
						throw PaleEngineException("Exception happened!", 'w', "Board_Representation.h", 39, "AddToDeathList", INVALID_PIECE_ID);
					else
						s_deathList.emplace_back(figure);
				}
				catch (PaleEngineException& exception) {
					if (exception.GetType() == 'e')
						PALE_ENGINE_ERROR("{0}[{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
					else if (exception.GetType() == 'w')
						PALE_ENGINE_WARN("{0}[{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

					std::cin.get();
				}
			}

			inline unsigned int GetRowNumber() const { return _row; }
			inline unsigned int GetColumnNumber() const { return _column; }
			inline BOARD_TYPE GetBoardType() const { return _representationType; }
			inline T GetPlateValue(unsigned int rowIt, unsigned int columnIt) const {
				try {
					if (rowIt < _row && columnIt < _column)
						return _board.at(rowIt).at(columnIt);
					else
						throw PaleEngineException("Exception happened!", 'e', "Board_Representation.h", 61, "GetPlateValue", VEC_OUT_OF_RANGE);
				}
				catch (PaleEngineException& exception) {
					if (exception.GetType() == 'e')
						PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
					else if (exception.GetType() == 'w')
						PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

					std::cin.get();
				}
			}
			inline static bool IsFigureDead(int figure) {
				if (std::find(s_deathList.begin(), s_deathList.end(), figure) != s_deathList.end())
					return true;
				else
					return false;
			}

			std::string ToString() const {
				std::stringstream ss;
				for (int rowIt = 0; rowIt < _row; rowIt++) {
					for (int columnIt = 0; columnIt < _column; columnIt++) {
						ss << " -- ";
					}

					ss << "\n";
					for (int columnIt = 0; columnIt < _column; columnIt++) {
						if (columnIt == _column - 1) {
							if(_board.at(rowIt).at(columnIt)->GetValue() == 0)
								ss << "|  " << _board.at(rowIt).at(columnIt)->GetName() << " |";
							else
								ss << "| " << _board.at(rowIt).at(columnIt)->GetName() << " |";
						}
						else {
							if (_board.at(rowIt).at(columnIt)->GetValue() == 0)
								ss << "|  " << _board.at(rowIt).at(columnIt)->GetName() << " ";
							else
								ss << "| " << _board.at(rowIt).at(columnIt)->GetName() << " ";
						}
					}

					ss << "\n";
					if (rowIt == _row - 1) {
						for (int columnIt = 0; columnIt < _column; columnIt++) {
							ss << " -- ";
						}
					}
				}
				return ss.str();
			}

		private:
			unsigned int _row, _column;
			std::vector<std::vector<T>> _board;
			BOARD_TYPE _representationType; //Type of data which is used for representing board situation.
			static std::vector<int> s_deathList; //List which holds captured pieces. 
		};

		//std::ostream& operator

		//--- Convertion functions ---//
		template<typename D, typename T>
		Board_Representation<D> ConvertBoard(const Board_Representation<T>& board);

		template<typename T>
		void Board_Representation<T>::SetPlateValue(unsigned int rowIt, unsigned int columnIt, T value);

		template<typename T>
		void Board_Representation<T>::MovePiece(std::pair<unsigned int, unsigned int> startPos, std::pair<unsigned int, unsigned int> endPos, Pieces& piece) {
			try {
				if (_board.at(startPos.first).at(startPos.second)->GetValue() != piece.GetValue())
					throw PaleEngineException("Exception happened!", 'w', "Board_Representation.h", 97, "MovePiece", INVALID_OCCUPATION);

				if (startPos.first < 0 || startPos.first >= _row || startPos.second < 0 || startPos.second >= _column ||
					endPos.first < 0 || endPos.first >= _row || endPos.second < 0 || endPos.second >= _column)
					throw PaleEngineException("Exception happened!", 'e', "Board_Representation.h", 101, "MovePiece", VEC_OUT_OF_RANGE);

				if (piece.MoveLogic(endPos, _board)) {
					PALE_ENGINE_INFO("Move was successfully made.");
					if (_board.at(endPos.first).at(endPos.second)->GetValue() != 0)
						_board.at(endPos.first).at(endPos.second) = std::make_shared<Blank>(endPos.first, endPos.second); //If capture set plate to blank

					_board.at(startPos.first).at(startPos.second).swap(_board.at(endPos.first).at(endPos.second)); //Swap contents of start and end plates.
					_board.at(startPos.first).at(startPos.second)->UpdatePosition(startPos); //Update positions for both plates.
					_board.at(endPos.first).at(endPos.second)->UpdatePosition(endPos);
				}
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());

				std::cin.get();
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
					throw PaleEngineException("Exception happened!", 'e', "Board_Representation.h", 132, "ProcessMoveCommand", MOVE_COMMAND__WRONG_OWNER);

				//Specifying type of the piece.
				if (std::toupper(move[1]) != 'K' && std::toupper(move[1]) != 'Q' && std::toupper(move[1]) != 'B'
					&& std::toupper(move[1]) != 'N' && std::toupper(move[1]) != 'R' && std::toupper(move[1]) != 'P')
					throw PaleEngineException("Exception happened!", 'e', "Board_Representation.h", 137, "ProcessMoveCommand", MOVE_COMMAND__WRONG_PIECE);
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
				default: throw PaleEngineException("Exception happened!", 'e', "Board_Representation.h", 151, "ProcessMoveCommand", MOVE_COMMAND__COORDINATE_OUT_OF_RANGE);	break;
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
				default: PaleEngineException("Exception happened!", 'e', "Board_Representation.h", 163, "ProcessMoveCommand", MOVE_COMMAND__COORDINATE_OUT_OF_RANGE); break;
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
				default: PaleEngineException("Exception happened!", 'e', "Board_Representation.h", 175, "ProcessMoveCommand", MOVE_COMMAND__COORDINATE_OUT_OF_RANGE); break;
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
				default: PaleEngineException("Exception happened!", 'e', "Board_Representation.h", 187, "ProcessMoveCommand", MOVE_COMMAND__COORDINATE_OUT_OF_RANGE); break;
				}

				processedMove.m_startPos = std::make_pair(startY, startX); //first cord := row, second cord := column
				processedMove.m_endPos = std::make_pair(endY, endX);
				PALE_ENGINE_INFO("Move was successfully processed.");
				return processedMove;
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