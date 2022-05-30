#include "palepch.h"
#include "Fully_Connected_Layer.h"

namespace Pale::AI_Module {
	Fully_Connected_Layer::Fully_Connected_Layer(unsigned short int layerId, unsigned short int layerNeurons, unsigned short int inputSize, std::function<double(double)> activationFunction, std::function<double(double)> dActivationFunction, bool inputLayer, bool outputLayer)
		: Layer(layerId, inputLayer, outputLayer),
		  _layerNeurons(layerNeurons),
		  _inputSize(inputSize),
		  _activationFunction(activationFunction),
		  _dActivationFunction(dActivationFunction) {
		if (!inputLayer) {
			assert(AssertionHandling(layerNeurons != 0, "Fully_Connected_Layer.cpp->Fully_Connected_Layer constructor [12]: Assertion failed! Value = " + std::to_string(layerNeurons) + " should be equal: " + std::to_string(0) + "!"));

			_weightsMatrix = Math::Matrix(layerNeurons, inputSize, true);
		} else {
			_inputSize = 0;
			_activationFunction = nullptr;
			_dActivationFunction = nullptr;
		}
	}

	std::vector<Math::Matrix> Fully_Connected_Layer::FeedForward(const std::vector<Math::Matrix>& previousLayer) {
		assert(AssertionHandling(previousLayer.size() == 1, "Fully_Connected_Layer.cpp->FeedForward() [15]: Assertion failed! Value = " + std::to_string(previousLayer.size()) + " should be equal: " + std::to_string(1) + "!"));

		assert(AssertionHandling(previousLayer.at(0).GetMatrixSize() == std::make_pair(_inputSize, static_cast<unsigned short int>(1)), "Fully_Connected_Layer.cpp->FeedForward() [17]: Assertion failed! Input of the fully connected layer of id " + std::to_string(_layerId) + " is incompatible!"));

		return { ~_weightsMatrix * previousLayer.at(0) };
	}
} // namespace Pale::AI_Module