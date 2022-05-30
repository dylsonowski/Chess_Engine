#include "palepch.h"
#include "Convolutional_Layer.h"

namespace Pale::AI_Module {
	Convolutional_Layer::Convolutional_Layer(unsigned short int layerId, bool inputLayer, std::pair<unsigned short int, unsigned short int> inputSize, unsigned short int inputDepth, unsigned short int kernelSize, unsigned short int numOfKernels, unsigned short int stride)
		: Layer(layerId, inputLayer, false),
		  _stride(stride) {
		for (int kernelsIterator = 0; kernelsIterator < numOfKernels; kernelsIterator++) {
			std::vector<Math::Matrix> tempKernel;
			for (int kernelDepthIterator = 0; kernelDepthIterator < inputDepth; kernelDepthIterator++) {
				tempKernel.emplace_back(Math::Matrix(kernelSize, kernelSize, true));
			}
			_kernels.emplace_back(tempKernel);
		}

		unsigned short int outputYSize = floor((inputSize.first - kernelSize) / stride) + 1;
		unsigned short int outputXSize = floor((inputSize.second - kernelSize) / stride) + 1;
		_inputSize = { inputSize.first, inputSize.second, inputDepth };
		_outputSize = { outputYSize, outputXSize, numOfKernels };

		for (int biasesIterator = 0; biasesIterator < numOfKernels; biasesIterator++) {
			_biases.emplace_back(Math::Matrix(outputYSize, outputXSize, true));
		}

		PALE_ENGINE_INFO("Convolutional_Layer.cpp->Convolutional_Layer constructor [25]: New convolutional layer of id {0} has been created! Input size: {1}x{2}. Input depth: {3}. Number of kernels: {4}. Kernel size: {5}. Stride of {6}. Output size: {7}x{8}.", layerId, inputSize.first, inputSize.second, inputDepth, numOfKernels, kernelSize, stride, outputYSize, outputXSize);
	}

	std::vector<Math::Matrix> Convolutional_Layer::FeedForward(const std::vector<Math::Matrix>& previousLayer) {
		assert(AssertionHandling(previousLayer.size() == _inputSize[2], "Convolutional_Layer.cpp->FeedForward() [27]: Assertion failed! Value = " + std::to_string(previousLayer.size()) + " should be equal: " + std::to_string(_inputSize[2]) + "!"));

		assert(AssertionHandling(previousLayer.at(0).GetMatrixSize() == std::make_pair(_inputSize[0], _inputSize[1]), "Convolutional_Layer.cpp->FeedForward() [29]: Assertion failed! Input of the convolutional layer of id " + std::to_string(_layerId) + " is incompatible!"));

		std::vector<Math::Matrix> tempOutput;
		for (int kernalsIterator = 0; kernalsIterator < _kernels.size(); kernalsIterator++) {
			Math::Matrix tempOutputMatrix(_outputSize[0], _outputSize[1], 0.f);
			for (int inputDepthIterator = 0; inputDepthIterator < previousLayer.size(); inputDepthIterator++) {
				Math::Matrix tempCrossCorrelationResult = Math::Matrix::CrossCorrelationOperation(previousLayer.at(inputDepthIterator), _kernels.at(kernalsIterator).at(inputDepthIterator));

				assert(AssertionHandling(tempCrossCorrelationResult.GetMatrixSize() == std::make_pair(_outputSize[0], _outputSize[1]), "Convolutional_Layer.cpp->FeedForward() [30]: Cross correlation output. " MATH__MATRICES_DIMENSIONS_INCORRECT));

				tempOutputMatrix += tempCrossCorrelationResult;
			}

			tempOutputMatrix += _biases.at(kernalsIterator);
			tempOutput.emplace_back(tempOutputMatrix);
		}

		PALE_ENGINE_INFO("Convolutional_Layer.cpp->FeedForward() [46]: Feed forward process for layer {0} has been performed successfully! Output dimensions: {1}x{2}x{3}.", _layerId, tempOutput.at(0).GetRowsNumber(), tempOutput.at(0).GetColumnsNumber(), tempOutput.size());

		return tempOutput;
	}
} // namespace Pale::AI_Module