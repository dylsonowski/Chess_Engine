#include "palepch.h"
#include "Artificial_Neuron.h"
#include "../Math/Matrix.h"
#include "../Math/Basic_Math.h"

namespace Pale::AI_Module {
	Artificial_Neuron::Artificial_Neuron(unsigned short int neuronId, unsigned short int layerId, double initialValue, bool inputNeuron, unsigned int inputWeightsNumber) : _neuronId(neuronId), _layerId(layerId), _neuronValue(initialValue) {
		if (!inputNeuron && inputWeightsNumber != 0)
			InitializeInputWeights(inputWeightsNumber);

		PALE_ENGINE_INFO("Neuron.cpp->Neuron constructor [10]: New neuron has been created! Neuron ID: {0}. Layer ID {1}. Neuron value: {2}. Input neuron: {3}. Size of input weights: {4}.", neuronId, layerId, _neuronValue, inputNeuron, _weights.size());
	}

	void Artificial_Neuron::CalculateNeuronValue(const Math::Matrix& previousLayer, double bias) {
		try {
			if (_weights.size() != previousLayer.GetRowsNumber())
				throw PaleEngineException("Exception happened!", 'e', "Neuron.cpp", 16, "CalculateNeuronValue()", NN__PREVIOUS_LAYER_INVALID_SIZE);

			Math::Matrix inputWeightsMatrix(_weights.size(), 1, _weights), previousLayerMatrix = previousLayer;
			double tempNeuronValue = Math::Matrix::HadamardMultiplication(inputWeightsMatrix, previousLayerMatrix, true).GetValue(0, 0);
			_neuronValue = tempNeuronValue + bias;
			ApplyActivationFunction(Math::SigmoidFunction);

			PALE_ENGINE_INFO("Neuron.cpp->CalculateNeuronValue() [23]: Value of neuron: {0} from layer: {1} has been recalculated! Value: {2}. Activated value {3}.", _neuronId, _layerId, tempNeuronValue + bias, _neuronValue);
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
		}
	}

	void Artificial_Neuron::InitializeInputWeights(unsigned int inputWeightsNumber) {
		for (int weightsIterator = 0; weightsIterator < inputWeightsNumber; weightsIterator++) {
			_weights.emplace_back(Math::GenerateRandomNumber<double>(-1, 1));
		}
	}
}