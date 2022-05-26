#include "palepch.h"
#include "Convolutional_Layer.h"

Pale::AI_Module::Convolutional_Layer::Convolutional_Layer(unsigned short int layerId, bool inputLayer, std::pair<unsigned short int, unsigned short int> inputSize, unsigned short int inputDepth, unsigned short int kernelSize, unsigned short int numOfKernels, unsigned short int stride) : Layer(layerId, inputLayer, false), _stride(stride) {
	for (int layerDepthIterator = 0; layerDepthIterator < inputDepth; layerDepthIterator++) {
		_layer.emplace_back(Math::Matrix(inputSize.first, inputSize.second, 0.f));
	}

	for (int kernelsIterator = 0; kernelsIterator < numOfKernels; kernelsIterator++) {
		std::vector<Math::Matrix> tempKernel;
		for (int kernelDepthIterator = 0; kernelDepthIterator < inputDepth; kernelDepthIterator++) {
			tempKernel.emplace_back(Math::Matrix(kernelSize, kernelSize, true));
		}
		_kernels.emplace_back(tempKernel);
	}

	unsigned short int outputYSize = floor((inputSize.first - kernelSize) / stride) + 1;
	unsigned short int outputXSize = floor((inputSize.second - kernelSize) / stride) + 1;
	_outputSize = std::make_pair(outputYSize, outputXSize);

	for (int biasesIterator = 0; biasesIterator < numOfKernels; biasesIterator++) {
		_biases.emplace_back(Math::Matrix(outputYSize, outputXSize, true));
	}

	PALE_ENGINE_INFO("Convolutional_Layer.cpp->Convolutional_Layer constructor [25]: New convolutional layer of id {0} has been created! Input size: {1}x{2}. Input depth: {3}. Number of kernels: {4}. Kernel size: {5}. Stride of {6}. Output size: {7}x{8}.", layerId, inputSize.first, inputSize.second, inputDepth, numOfKernels, kernelSize, stride, outputYSize, outputXSize);
}

void Pale::AI_Module::Convolutional_Layer::FeedForward(const Math::Matrix& previousLayer) {
	for()
}
