#pragma once
#include "Pale/Engine_Macros.h"
#include "Pale/Chess_Module/Board_Representation.h"

namespace Pale {
	namespace UI {
		class User_Interface {

		public:
			User_Interface() = delete;
			virtual ~User_Interface() = default;

			virtual void Draw() const = 0; //TODO: Add necessary arguments!
			virtual void Update() = 0; //TODO: Add necessary arguments!

			void SetGameStatus(bool setCheck, bool value) {
				if (setCheck)
					_check = value;
				else
					_checkMate = value;
			}
			void UpdateBoardRepresentation(std::shared_ptr<Chess_Logic::Board_Representation<std::string>> boardRepresentation) {
				_boardRepresentation = boardRepresentation;
				_deathList = Chess_Logic::s_deathList;
			}

		protected:
			User_Interface(std::shared_ptr<Chess_Logic::Board_Representation<std::string>> boardRepresentation) : _check(false), _checkMate(false) { 
				UpdateBoardRepresentation(boardRepresentation); 
			}

			UI_TYPE type;
			std::shared_ptr<Chess_Logic::Board_Representation<std::string>> _boardRepresentation;
			std::vector<int> _deathList;
			bool _check, _checkMate;
		};
	}
}