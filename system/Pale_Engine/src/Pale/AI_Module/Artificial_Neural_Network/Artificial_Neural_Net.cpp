#include "palepch.h"
#include "Artificial_Neural_Net.h"

namespace Pale::AI_Module {
	Artificial_Neural_Net::Artificial_Neural_Net(std::vector<unsigned int> topology, std::string name, double learningRate) : _networkName(name), _learningRate(learningRate) {
		try {
			if(topology.size() < 3)
				throw PaleEngineException("Exception happened!", 'e', "Artificial_Neural_Net.cpp", 8, "Artificial_Neural_Net constructor", NN__INCORRECT_TOPOLOGY_SIZE);

			_topology = topology;
			for (int networkIterator = 0; networkIterator < topology.size(); networkIterator++) {
				if (networkIterator == 0)
					_network.emplace_back(std::make_shared<Layer>(networkIterator, topology.at(networkIterator), 1, false, std::optional<unsigned short int>()));
				else if (networkIterator == topology.size() - 1)
					_network.emplace_back(std::make_shared<Layer>(networkIterator, topology.at(networkIterator), 1, true, topology.at(networkIterator - 1)));
				else
					_network.emplace_back(std::make_shared<Layer>(networkIterator, topology.at(networkIterator), 1, false, topology.at(networkIterator - 1)));
			}

			PALE_ENGINE_INFO("Artificial_Neural_Net.cpp->Artificial_Neural_Net constructor [20]: New instance of artificial neural network has been created! Network size: {0}. Size of input layer: {1}. Size of output layer: {2}.", _network.size(), _network.at(0)->GetLayerSize(), _network.at(_topology.size() - 1)->GetLayerSize());
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
		}
	}

	void Artificial_Neural_Net::FeedForward(const std::vector<double>& inputData) {
		for (int inputLayerIterator = 0; inputLayerIterator < _network.at(0)->GetLayerSize(); inputLayerIterator++) {
			_network.at(0)->SetNeuronValue(inputLayerIterator, inputData.at(inputLayerIterator));
		}

		PALE_ENGINE_TRACE("Artificial_Neural_Net.cpp->FeedForward() [35]: Values of input layer has been imported successfully.");

		int recalculatedNeurons = 0, recalculatedLayers = 0;
		for (int networkIterator = 0; networkIterator < _network.size(); networkIterator++) {
			if (_network.at(networkIterator)->IsInputLayer())
				_network.at(networkIterator)->RecalculateLayerValues(std::optional<Math::Matrix>());
			else {
				_network.at(networkIterator)->RecalculateLayerValues((_network.at(networkIterator - 1)->ConvertToMatrix()));
				recalculatedNeurons += _network.at(networkIterator)->GetLayerSize();
				recalculatedLayers++;
			}
		}

		PALE_ENGINE_TRACE("Artificial_Neural_Net.cpp->FeedForward() [48]: FeefForward process has been finished successfully! Number of recalculated layers: {0}. Number of recalculated neurons: {1}. Output of the network: {2}", recalculatedLayers, recalculatedNeurons, _network.at(_topology.size() - 1)->ToString());
	}

	void Artificial_Neural_Net::BackPropagation(const std::vector<double>& targetData) {
		try {
			if(targetData.size() != _network.back()->GetLayerSize())
				throw PaleEngineException("Exception happened!", 'e', "Artificial_Neural_Net.cpp", 54, "BackPropagation()", NN__INCORRECT_TARGET_SIZE);

			Math::Matrix layerErrors(_network.back()->GetLayerSize(), 1, false);
			for (int networkIterator = _network.size() - 1; networkIterator > 0; networkIterator--) {
				if (networkIterator == _network.size() - 1) {
					//--- Computing output error vector ---//
					Math::Matrix targerDataMatrix(targetData.size(), 1, targetData);
					layerErrors = targerDataMatrix - _network.at(networkIterator)->ConvertToMatrix();

					PALE_ENGINE_TRACE("Artificial_Neural_Net.cpp->BackPropagation() [63]: Output errors has been computed! Output errors: {0}", layerErrors.ToString(true));
				}
				else {
					//--- Computing error vector ---//
					layerErrors = (~_network.at(networkIterator + 1)->GenerateWeightsMatrix()) * layerErrors;

					PALE_ENGINE_TRACE("Artificial_Neural_Net.cpp->BackPropagation() [69]: Errors of layer {0} has been computed! Errors values: {1}", _network.at(networkIterator)->GetLayerId(), layerErrors.ToString(true));
				}

				//--- Computing delataWeights (learningRate * errors * gradient(derivative of sigmoid) * previousLayerValues) ---//
				Math::Matrix gradient = _network.at(networkIterator)->ConvertToMatrix().Map(Math::DSigmoigFunction);
				Math::Matrix layerDeltaWeights = Math::Matrix::HadamardMultiplication(layerErrors, gradient, false) * (~_network.at(networkIterator - 1)->ConvertToMatrix()) * _learningRate;

				PALE_ENGINE_TRACE("Artificial_Neural_Net.cpp->BackPropagation() [77]: Delta weights values for layer {0} have been computed! Delta weights values: {1}", _network.at(networkIterator)->GetLayerId(), layerDeltaWeights.ToString(true));

				Math::Matrix layerDeltaBiases = Math::Matrix::HadamardMultiplication(layerErrors, gradient, false) * _learningRate;

				PALE_ENGINE_TRACE("Artificial_Neural_Net.cpp->BackPropagation() [81]: Delta biases values for layer {0} have been computed! Delta biases values: {1}", _network.at(networkIterator)->GetLayerId(), layerDeltaBiases.ToString(true));

				//--- Update weights values ---//

			}

			////--- Computing error vector ---//
			//Math::Matrix targetDataMatrix(targetData.size(), 1, targetData), outputErrors = targetDataMatrix - _network.back()->ConvertToMatrix();

			//PALE_ENGINE_TRACE("Artificial_Neural_Net.cpp->BackPropagation() [59]: Output errors has been computed! Output errors: {0}", outputErrors.ToString(true));

			////--- Computing hidden layer error vector ---//
			//Math::Matrix layerWeightsMatrix = _network.back()->GenerateWeightsMatrix();
			//Math::Matrix hiddenLayerErrors = (~layerWeightsMatrix) * outputErrors;

			//PALE_ENGINE_TRACE("Artificial_Neural_Net.cpp->BackPropagation() [65]: Errors of layer {0} has been computed! Errors values: {1}", _network.at(_topology.size() - 2)->GetLayerId(), hiddenLayerErrors.ToString(true));

			////-- Computing delataWeights (learningRate * errors * gradient(derivative of sigmoid) * previousLayerValues) ---//
			//Math::Matrix gradient = _network.back()->ConvertToMatrix().Map(Math::DSigmoigFunction);
			//Math::Matrix deltaOutputWeights = outputErrors * gradient * (~_network.at(_topology.size() - 2)->ConvertToMatrix());
			//deltaOutputWeights *= _learningRate;

			//PALE_ENGINE_TRACE("Artificial_Neural_Net.cpp->BackPropagation() [72]: Delta weights values for layer {0} have been computed! Delta weights values: {1}", _network.back()->GetLayerId(), deltaOutputWeights.ToString(true));
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
		}
	}

	std::string Artificial_Neural_Net::ToString() const	{
		std::stringstream ss;
		ss << "//--- " << _networkName << " ---//\n\n";
		ss << "Network topology:\n";
		for (int topologyIterator = 0; topologyIterator < _topology.size(); topologyIterator++) {
			if (topologyIterator == 0)
				ss << "[" << _topology.at(topologyIterator) << " input] -> ";
			else if(topologyIterator == _topology.size() - 1)
				ss << "[" << _topology.at(topologyIterator) << " output]";
			else
				ss << "[" << _topology.at(topologyIterator) << " neurons] -> ";
		}

		ss << "\n\nNetwork input: ";
		for (int layerIterator = 0; layerIterator < _network.at(0)->GetLayerSize(); layerIterator++) {
			if (layerIterator == _network.at(0)->GetLayerSize() - 1)
				ss << _network.at(0)->GetNeuronValue(layerIterator) << ".";
			else
				ss << _network.at(0)->GetNeuronValue(layerIterator) << ", ";
		}

		ss << "\nNetwork output: ";
		for (int layerIterator = 0; layerIterator < _network.at(_topology.size() - 1)->GetLayerSize(); layerIterator++) {
			if (layerIterator == _network.at(_topology.size() - 1)->GetLayerSize() - 1)
				ss << _network.at(_topology.size() - 1)->GetNeuronValue(layerIterator) << ".";
			else
				ss << _network.at(_topology.size() - 1)->GetNeuronValue(layerIterator) << ", ";
		}

		return ss.str();
	}
}