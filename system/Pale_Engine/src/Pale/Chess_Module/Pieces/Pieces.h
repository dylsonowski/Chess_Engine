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

			virtual bool MoveLogic(std::pair<unsigned int, unsigned int> startPos, std::pair<unsigned int, unsigned int> endPos, std::vector<std::vector<std::shared_ptr<Pieces>>>& board) const = 0;
			inline std::pair<unsigned int, unsigned int> GetPosition() const { return _positionCords; }
			inline OWNERS IsWhite() const { return _owner; }
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
		static bool KingIsChecked(std::vector<std::vector<std::shared_ptr<Pieces>>> board, OWNERS whichTurn);

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