#pragma once

#define BIT(x) (1 << x)

namespace Pale::UI::Event_System {
	enum class EVENT_TYPES {
		NONE = 0,
		APPLICATION_GAME_TREE_SETUP, APPLICATION_CLOSE,
		GAME_RESET_ENVIRONMENT, GAME_MAKE_MOVE
	};

	enum EVENT_CATEGORY {
		NONE = 0,
		APPLICATION_EVENT = BIT(1),
		GAME_EVENT = BIT(2)
	};

	#define EVENT_TYPE(type) static EVENT_TYPES GetStaticEventType() { return EVENT_TYPES::type; }\
																	virtual EVENT_TYPES GetEventType() const override { return GetStaticEventType(); }\
																	virtual const char* GetEventName() const override { return #type; }
			
	#define EVENT_CATEGORY(category) virtual int GetEventCategory() const override { return category; }

	class Event {

	public:
		virtual ~Event() = default;

		virtual EVENT_TYPES GetEventType() const = 0;
		virtual const char* GetEventName() const = 0;
		virtual int GetEventCategory() const = 0;
		virtual std::string ToString() const { return GetEventName(); }
		bool IsInCategory(EVENT_CATEGORY category) { return GetEventCategory() & category; }

		bool _isHandled = false;

	protected:
		Event() = default;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& event) {
		return os << event.ToString();
	}
}