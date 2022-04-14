#pragma once
#include "Events/Application_Events.h"
#include "Events/Game_Events.h"


//--- Class responsible for communication between user interface and application core ---//
// IMPORTANT: Use concept of 'Observer' design pattern
namespace Pale::UI::Event_System {
	//--- Polymorphic class allowing to all kind of classes to be a 'subscriber' ---//
	class Base_Listener {

	public:
		explicit Base_Listener() = default;
		//--- Copying constructor and '=' operator are not necessary because it is unusable class ---//
		Base_Listener(const Base_Listener&) = delete;
		Base_Listener& operator=(const Base_Listener&) = delete;
		virtual ~Base_Listener() = default;
	};

	//--- Template class describing subscriber ---//
	template<typename T>
	class Listener : public Base_Listener {

	public:
		explicit Listener(const std::function<void(T&)>& callback) : _callback(callback) {}
		~Listener() = default;

		void ExecuteCallback(T& type) { _callback(type); }

	private:
		//--- Callback function pointer executed if specific event occur ---//
		std::function<void(T&)> _callback;
	};

	class Event_Emitter {

	public:
		//--- Template method responsible for finding subscribers which subscribe specific event end execute its callback function ---//
		template<typename T>
		void Emit(T& type) {
			auto it = _listeners.find(typeid(type));
			if (it != _listeners.end())
				static_cast<Listener<T>*>(it->second.get())->ExecuteCallback(type);
		}

		//--- Template method responsible for adding new subscriber with its respective event ---//
		template<typename T>
		Event_Emitter& On(const std::function<void(T&)>& callback) {
			_listeners[typeid(T)] = std::unique_ptr<Base_Listener>(new Listener<T>(callback));
			return *this;
		}

	private:
		//--- Structure containing all subscribers with its respective events that they want to listen for ---//
		std::map<std::type_index, std::unique_ptr<Base_Listener>> _listeners;
	};
}