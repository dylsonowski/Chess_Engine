#pragma once
#include "Layer.h"

namespace Pale::AI_Module {
	class Convolutional_Layer : public Layer {

	public:
		Convolutional_Layer() = delete;
		Convolutional_Layer(unsigned short int layerId, bool inputLayer, std::pair<unsigned short int, unsigned short int> inputSize, unsigned short int inputDepth = 3, unsigned short int kernelSize = 3, unsigned short int numOfKernels = 1, unsigned short int stride = 1);
		~Convolutional_Layer() = default;

		//--- Void functions ---//
		void FeedForward(const Math::Matrix& previousLayer) override;
		void BackPropagation(const Math::Matrix& previousLayer) override;

	private:
		unsigned short int _stride;
		std::pair<unsigned short int, unsigned short int> _outputSize;
		std::vector<Math::Matrix> _layer, _biases;
		std::vector<std::vector<Math::Matrix>> _kernels;
	};
}