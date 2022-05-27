#pragma once
#include "Layer.h"

namespace Pale::AI_Module {
	enum class POOLING_METHOD {
		MAX_POOLING
	};

	class Pooling_Layer : public Layer {

	public:
		Pooling_Layer() = delete;
		Pooling_Layer(unsigned short int layerId, std::pair<unsigned short int, unsigned short int> inputSize, unsigned short int inputDepth = 3, unsigned short int windowSize = 2, unsigned short int stride = 2): Layer(layerId, false, false) {}
		~Pooling_Layer() = default;

	private:
		unsigned short int _stride;
		std::array<unsigned short int, 3> _inputSize, _outputSize;
		Math::Matrix _poolingWindow;
		POOLING_METHOD _poolingMethod;
	};
}