#include "palepch.h"
#include "Activation_Layer.h"

namespace Pale::AI_Module {
	Activation_Layer::Activation_Layer(unsigned short int layerId, std::function<double(double)> activationFunction, std::function<double(double)> dActivationFunction, std::pair<unsigned short int, unsigned short int> inputSize, unsigned short int inputDepth)
		: Layer(layerId, false, false),
		  _activationFunction(activationFunction),
		  _dActivationFunction(dActivationFunction) {
		_inputSize = { inputSize.first, inputSize.second, inputDepth };
		_outputSize = { inputSize.first, inputSize.second, inputDepth };

		PALE_ENGINE_INFO("Activation_Layer.cpp->Activation_Layer constructor [9]: New activation layer of id {0} has been created! Input size: {1}x{2}x{3}.", _layerId, inputSize.first, inputSize.second, inputDepth);
	}

	std::vector<Math::Matrix> Activation_Layer::FeedForward(const std::vector<Math::Matrix>& previousLayer) {
		assert(AssertionHandling(previousLayer.size() == _inputSize[2], "Activation_Layer.cpp->FeedForward() [13]: Assertion failed! Value = " + std::to_string(previousLayer.size()) + " should be equal: " + std::to_string(_inputSize[2]) + "!"));

		assert(AssertionHandling(previousLayer.at(0).GetMatrixSize() == std::make_pair(_inputSize[0], _inputSize[1]), "Activation_Layer.cpp->FeedForward() [15]: Assertion failed! Input of the activation layer of id " + std::to_string(_layerId) + " is incompatible!"));

		std::vector<Math::Matrix> tempOutput = previousLayer;
		for (auto previousLayerDepthIterator : tempOutput) {
			previousLayerDepthIterator.Map(_activationFunction);
		}

		PALE_ENGINE_INFO("Activation_Layer.cpp->FeedForward() [22]: Feed forward process for activation layer {0} has been performed successfully! Output dimensions: {1}x{2}x{3}.", _layerId, tempOutput.at(0).GetRowsNumber(), tempOutput.at(0).GetColumnsNumber(), tempOutput.size());

		return tempOutput;
	}

    std::vector<Math::Matrix> Activation_Layer::BackPropagation(const std::vector<Math::Matrix>& previousLayer) {
		assert(AssertionHandling(previousLayer.size() == _outputSize[2], "Activation_Layer.cpp->BackPropagation() [28]: Assertion failed! Value = " + std::to_string(previousLayer.size()) + " should be equal: " + std::to_string(_inputSize[2]) + "!"));

		assert(AssertionHandling(previousLayer.at(0).GetMatrixSize() == std::make_pair(_outputSize[0], _outputSize[1]), "Activation_Layer.cpp->BackPropagation() [30]: Assertion failed! Previous layer size given into activation layer of id " + std::to_string(_layerId) + " is incompatible!"));

		std::vector<Math::Matrix> tempOutput = previousLayer;
		for (auto previousLayerDepthIterator : tempOutput) {
			previousLayerDepthIterator.Map(_dActivationFunction);
		}

		PALE_ENGINE_INFO("Pooling_Layer.cpp->BackPropagation() [37]: Back propagation process for activation layer {0} has been performed successfully! Output dimensions: {1}x{2}x{3}.", _layerId, tempOutput.at(0).GetRowsNumber(), tempOutput.at(0).GetColumnsNumber(), tempOutput.size());

		return tempOutput;
	}
} // namespace Pale::AI_Module