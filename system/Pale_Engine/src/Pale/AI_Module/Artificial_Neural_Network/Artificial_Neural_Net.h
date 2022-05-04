#pragma once
#include "Layer.h"

namespace Pale::AI_Module {
	struct Data_Set {
		std::vector<std::pair<std::vector<double>, std::vector<double>>> m_data;
		bool m_testSet;
		unsigned short int m_dataSetSize;

		Data_Set() = delete;
		Data_Set(const std::vector<std::vector<double>>& inputData, const std::vector<std::vector<double>>& targetData, bool testSet = false) : m_testSet(testSet) {
			try {
				if (inputData.size() != targetData.size())
					throw PaleEngineException("Exception happened!", 'e', "Artificial_Neural_Net.h", 14, "Data_Set constructor", NN__INVALID_DATA_SIZE);

				for (int dataIterator = 0; dataIterator < inputData.size(); dataIterator++) {
					m_data.emplace_back(std::make_pair(inputData.at(dataIterator), targetData.at(dataIterator)));
				}
				m_dataSetSize = inputData.size();

				PALE_ENGINE_INFO("Artificial_Neural_Net.h->Data_Set constructor [21]: New Data_Set object has been created! Data set size: {0}. Test set: {1}.", m_dataSetSize, m_testSet);
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}

		Data_Set ShuffleDataSet() {
			std::random_device randomDevice;
			std::mt19937 generator(randomDevice());
			std::shuffle(m_data.begin(), m_data.end(), generator);

			PALE_ENGINE_TRACE("Artificial_Neural_Net.h->ShuffleDataSet() [36]: Data set has been shuffled!");

			return *this;
		}
	};

	class Artificial_Neural_Net {

	public:
		Artificial_Neural_Net() = delete;
		Artificial_Neural_Net(std::vector<unsigned int> topology, std::string name, double learningRate = 0.01);
		~Artificial_Neural_Net() = default;

		//--- Void functions ---//
		void Train(const Data_Set& trainSet, const Data_Set& testSet, unsigned short int epochs, bool printErrors = false, float acceptanceCriteriaPerNeuron = 0.2f, bool pauseTraining = false);
		void SaveWeights(std::optional<std::string> fileName, std::string weightsFilePath = "./Saved Weights/", bool extractionLogs = false) const;
		void LoadWeights(std::string fileName, std::string weightsFilePath = "./Saved Weights/", bool importLogs = false);

		//--- Returning functions ---//
		std::string ToString() const;

	private:
		void FeedForward(const std::vector<double>& inputData);
		void BackPropagation(const std::vector<double>& targetData);
		const std::vector<double> Predict(const std::vector<double>& inputData);

		std::string _networkName;
		std::vector<unsigned int> _topology;
		std::vector<std::shared_ptr<Layer>> _network;
		double _learningRate, _accumulatedOutputError;
	};

	inline std::ostream& operator<<(std::ostream& os, const Artificial_Neural_Net& data) {
		return os << data.ToString();
	}

	inline std::pair<Data_Set, Data_Set> SplitDataSet(const Data_Set& dataSet, unsigned short int splitDistribution) {
		std::vector<std::vector<double>> inputDataSet1, targetDataSet1, inputDataSet2, targetDataSet2;

		const unsigned short int dataSet1Size = Math::ProportionCalculation(splitDistribution, dataSet.m_dataSetSize);
		for (int dataSetIterator = 0; dataSetIterator < dataSet.m_dataSetSize; dataSetIterator++) {
			if (dataSetIterator < dataSet1Size) {
				inputDataSet1.emplace_back(dataSet.m_data.at(dataSetIterator).first);
				targetDataSet1.emplace_back(dataSet.m_data.at(dataSetIterator).second);
			}
			else {
				inputDataSet2.emplace_back(dataSet.m_data.at(dataSetIterator).first);
				targetDataSet2.emplace_back(dataSet.m_data.at(dataSetIterator).second);
			}
		}

		PALE_ENGINE_INFO("Artificial_Neural_Net.h->SplitDataSet() [83]: Data set has been splinted! Data set distribution: {0}/{1}. Data sets sizes: {2}/{3}.", splitDistribution, 1 - splitDistribution, inputDataSet1.size(), inputDataSet2.size());

		return std::make_pair(Data_Set(inputDataSet1, targetDataSet1), Data_Set(inputDataSet2, targetDataSet2, true));
	}
}