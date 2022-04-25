#pragma once

namespace Pale::Math {
	//--- Activation functions ---//
	inline double SigmoidFunction(double value) { return 1 / (1 + exp(-value)); }

	inline double DSigmoigFunction(double value) { return value * (1 - value); }

	//--- Random number generating function ---//
	template<typename T>
	inline T GenerateRandomNumber(T minValue, T maxValue) {
		std::random_device device;
		std::default_random_engine engine(device());
		std::uniform_real_distribution<T> uniformDist(minValue, maxValue);
		return uniformDist(engine);
	}

	template<> inline int GenerateRandomNumber(int minValue, int maxValue) {
		std::random_device device;
		std::default_random_engine engine(device());
		std::uniform_int_distribution<int> uniformDist(minValue, maxValue);
		return uniformDist(engine);
	}

	//--- Basic math operations ---//
	inline unsigned short int PrecentageCalculation(double value, double maxValue) { return (value * 100) / maxValue; }
	inline unsigned short int ProportionCalculation(unsigned short int precentage, double maxValue) { return (precentage * maxValue) / 100; }
	template<typename T> inline T AbsoluteValueCalculation(T value) { return std::abs(value); }
}