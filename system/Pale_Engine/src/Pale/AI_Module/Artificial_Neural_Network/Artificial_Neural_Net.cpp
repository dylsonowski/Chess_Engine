#include "palepch.h"
#include "Artificial_Neural_Net.h"

namespace Pale::AI_Module {
	Artificial_Neural_Net::Artificial_Neural_Net(std::vector<unsigned int> topology, std::string name, double learningRate) : _networkName(name), _learningRate(learningRate), _accumulatedOutputError(0) {
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

			PALE_ENGINE_INFO("Artificial_Neural_Net.cpp->Artificial_Neural_Net constructor [20]: New instance of artificial neural network of name '{0}' has been created! Network size: {1}. Size of input layer: {2}. Size of output layer: {3}.", _networkName, _network.size(), _network.at(0)->GetLayerSize(), _network.at(_topology.size() - 1)->GetLayerSize());
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
		}
	}

	void Artificial_Neural_Net::Train(const Data_Set& trainSet, const Data_Set& testSet, unsigned short int epochs, bool printErrors, float acceptanceCriteriaPerNeuron, bool pauseTraining) {
		try {
			unsigned short int correctPredictions;
			for (int epochIterator = 0; epochIterator <= epochs; epochIterator++) {
				//--- Training ---//
				for (int trainingIterator = 0; trainingIterator < trainSet.m_dataSetSize; trainingIterator++) {
					FeedForward(trainSet.m_data.at(trainingIterator).first);
					BackPropagation(trainSet.m_data.at(trainingIterator).second);

					if (printErrors)
						std::cout << "Training execution " << trainingIterator << "/" << trainSet.m_dataSetSize << ". Output error: " << _accumulatedOutputError << ".\n";
				}

				//--- Testing ---//
				correctPredictions = 0;
				char continueTrainingAnswer;
				for (int testingIterator = 0; testingIterator < testSet.m_dataSetSize; testingIterator++) {
					std::vector<double> prediction = Predict(testSet.m_data.at(testingIterator).first);
					double predictionCorrectionPrecentage = 0;

					for (int predictionIterator = 0; predictionIterator < prediction.size(); predictionIterator++) {
						predictionCorrectionPrecentage += Math::AbsoluteValueCalculation<double>(prediction.at(predictionIterator) - testSet.m_data.at(testingIterator).second.at(predictionIterator));
					}

					predictionCorrectionPrecentage /= prediction.size();

					if(predictionCorrectionPrecentage <= acceptanceCriteriaPerNeuron * prediction.size())
						correctPredictions++;
				}

				auto var = Math::PrecentageCalculation(correctPredictions, testSet.m_dataSetSize);
				std::cout << "EPOCH: " << epochIterator << "/" << epochs << ". Accuracy: " << var << "%.";
				
				if (pauseTraining) {
					std::cout << "\nContinue training ? : ";
					std::cin >> continueTrainingAnswer;

					if (continueTrainingAnswer == 'N' || continueTrainingAnswer == 'n')
						break;
				}

				std::cout << "\n\n";
			}

			PALE_ENGINE_INFO("Artificial_Neural_Net.cpp->Train() [66]: Training process of network named '{0}' has been finished! Final model accuracy: {1}.", _networkName, correctPredictions);
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
		}
	}

	void Artificial_Neural_Net::SaveWeights(std::optional<std::string> fileName, std::string weightsFilePath, bool extractionLogs) const {
		try {
			nlohmann::json weightsFile;
			weightsFile["name"] = _networkName;
			for (int networkIterator = 1; networkIterator < _network.size(); networkIterator++) {
				for (int layerIterator = 0; layerIterator < _network.at(networkIterator)->GetLayerSize(); layerIterator++) {
					weightsFile["layer" + std::to_string(networkIterator)].push_back(_network.at(networkIterator)->GetNeuronInputWeights(layerIterator));
				}
			}

			if (extractionLogs)
				PALE_ENGINE_TRACE("Artificial_Neural_Net.cpp->SaveWeights() [96]: Json file with weights values has been created! Json content: {0}.", weightsFile.dump());

			std::string path;
			if (!fileName.has_value())
				path = weightsFilePath + "/" + _networkName + "(weights).json";
			else
				path = weightsFilePath + "/" + fileName.value() + ".json";

			std::ofstream outputFile(path);
			if (!outputFile.good())
				throw PaleEngineException("Exception happened!", 'e', "Artificial_Neural_Net.cpp", 89, "SaveWeights()", FILE_OPEN_ERROR);

			outputFile << std::setw(4) << weightsFile;
			outputFile.close();

			PALE_ENGINE_INFO("Artificial_Neural_Net.cpp->SaveWeights() [102]: Weights data has been saved inside file: {0}.", path);
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
		}
	}

	void Artificial_Neural_Net::LoadWeights(std::string fileName, std::string weightsFilePath, bool importLogs) {
		try {
			nlohmann::json weightsFile;
			std::string path = weightsFilePath + "/" + fileName + ".json";
			std::ifstream inputFile(path);

			if (!inputFile.good())
				throw PaleEngineException("Exception happened!", 'e', "Artificial_Neural_Net.cpp", 127, "LoadWeights()", FILE_OPEN_ERROR);

			inputFile >> weightsFile;
			inputFile.close();

			if(importLogs)
				PALE_ENGINE_TRACE("Artificial_Neural_Net.cpp->LoadWeights() [132]: Weights values has been imported! Json content: {0}.", weightsFile.dump());

			//--- Importing "networkName" value ---//
			_networkName = weightsFile["name"];
			assert(AssertionHandling(_networkName == weightsFile["name"].get<std::string>(), "Artificial_Neural_Net.cpp->LoadWeights() [135]: Assertion failed! Value = " + _networkName + " should be equal: " + weightsFile["name"].get<std::string>() + "!"));

			//--- Importing layers weights values ---//
			for (int networkIterator = 1; networkIterator < _network.size(); networkIterator++) {
				for (int layerIterator = 0; layerIterator < _network.at(networkIterator)->GetLayerSize(); layerIterator++) {
					std::vector<double> tempImportingWeightsVector = weightsFile["layer" + std::to_string(networkIterator)][layerIterator].get<std::vector<double>>();
					_network.at(networkIterator)->LoadNeuronInputWeights(layerIterator, tempImportingWeightsVector);
				}
			}

			PALE_ENGINE_INFO("Artificial_Neural_Net.cpp->LoadWeights() [146]: Weights values from file {0} has been loaded into network: {1}.", path, _networkName);
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

					Math::Matrix tempLayerErrorsMatrix = layerErrors;
					_accumulatedOutputError = tempLayerErrorsMatrix.Map(Math::AbsoluteValueCalculation<double>).AccumulateMatrixValues();
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
				_network.at(networkIterator)->UpdateLayerWeights(layerDeltaWeights);

				//--- Update biases values ---//
				_network.at(networkIterator)->UpdateLayerBiases(layerDeltaBiases);
			}

			PALE_ENGINE_TRACE("Artificial_Neural_Net.cpp->BackPropagation() [88]: Back propagation process has been finished!");
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
		}
	}

	const std::vector<double> Artificial_Neural_Net::Predict(const std::vector<double>& inputData) {
		FeedForward(inputData);
		std::vector<double> prediction;
		for (int outputLayerIterator = 0; outputLayerIterator < _network.back()->GetLayerSize(); outputLayerIterator++) {
			prediction.emplace_back(_network.back()->GetNeuronValue(outputLayerIterator));
		}

		return prediction;
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

		ss << "\n\nOutput error value: " << _accumulatedOutputError << ".";
		return ss.str();
	}
}