#pragma once
#include "../Engine_Macros.h"
#include <vector>
#include <string>
//#include "../Log_System.h"

namespace Pale {
	namespace Chess_Logic {
		enum class BOARD_TYPE {
			BASE = 0,
			INT_TYPE,
			STRING_TYPE,
			OBJECT_TYPE
		};

		template<typename T>
		class PALE_API Board_Representation {

		public:
			Board_Representation() = delete;
			Board_Representation(unsigned int row = 8, unsigned int column = 8, BOARD_TYPE boardType = BOARD_TYPE::BASE);
			~Board_Representation() = default;

			void SetPlateValue(T value);

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
				catch (std::string errorMessage) { PALE_ENGINE_ERROR(errorMessage); }
			}

		private:
			unsigned int _row, _column;
			std::vector<std::vector<T>> _board;
			BOARD_TYPE _representationType;
			static std::vector<int> _deathList;
		};
	}
}