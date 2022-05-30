#pragma once
#include "Layer.h"

namespace Pale::AI_Module {
	enum class POOLING_METHOD {
		MAX_POOLING = 0
	};

	class Pooling_Layer : public Layer {

	public:
		Pooling_Layer() = delete;
		Pooling_Layer(unsigned short int layerId, std::pair<unsigned short int, unsigned short int> inputSize, unsigned short int inputDepth = 3, unsigned short int windowSize = 2, POOLING_METHOD poolingMethods = POOLING_METHOD::MAX_POOLING, unsigned short int stride = 2);
		~Pooling_Layer() = default;

		std::vector<Math::Matrix> FeedForward(const std::vector<Math::Matrix>& previousLayer) override;
		std::vector<Math::Matrix> BackPropagation(const std::vector<Math::Matrix>& previousLayer) override;

	private:
		unsigned short int _stride, _poolingWindowSize;
		std::array<unsigned short int, 3> _inputSize, _outputSize;
		POOLING_METHOD _poolingMethod;
		std::vector<Math::Matrix> _inputPoolingMask;
	};
} // namespace Pale::AI_Module