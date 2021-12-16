#pragma once
#include "Pale/Chess_Module/Board_Representation.h"
#include "Event_System/Event_Emitter.h"

namespace Pale::UI {
	class User_Interface {

	public:
		User_Interface() = delete;
		virtual ~User_Interface() = default;

		virtual void Draw() = 0; // TODO: Add necessary arguments!
		virtual void Update(std::shared_ptr<Chess_Logic::Board_Representation<std::string>> boardRepresentation, APP_STATES applicationState, unsigned short int turn) = 0; // TODO: Add necessary arguments!

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

		//--- Instance of event emitter responsible for communication with application core according to hermetysation principal ---//
		Event_System::Event_Emitter _eventEmitter;

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