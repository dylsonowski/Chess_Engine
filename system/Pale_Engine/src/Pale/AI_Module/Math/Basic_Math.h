#pragma once

namespace Pale::Math {
	//--- Activation functions ---//
	inline double SigmoidFunction(double value) { return 1 / (1 + exp(-value)); }

	inline double DSigmoigFunction(double value) { return value * (1 - value); }

	//--- Random number generating function ---//
	template<typename T>
	static T GenerateRandomNumber(T minValue, T maxValue) {
		std::random_device device;
		std::default_random_engine engine(device());
		std::uniform_real_distribution<T> uniformDist(minValue, maxValue);
		return uniformDist(engine);
	}
}