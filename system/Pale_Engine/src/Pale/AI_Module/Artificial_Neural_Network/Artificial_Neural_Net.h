#pragma once
#include "Layer.h"

namespace Pale::AI_Module {
	struct Data_Set {
		std::vector<std::vector<double>> m_inputData, m_targetData;
		bool m_testSet;
		unsigned short int m_dataSetSize;

		Data_Set() = delete;
		Data_Set(const std::vector<std::vector<double>>& inputData, const std::vector<std::vector<double>>& targetData, bool testSet = false) : m_inputData(inputData), m_targetData(targetData), m_testSet(testSet) {
			try {
				if (inputData.size() != targetData.size())
					throw PaleEngineException("Exception happened!", 'e', "Artificial_Neural_Net.cpp", 33, "Train()", NN__INVALID_DATA_SIZE);

				m_dataSetSize = inputData.size();
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
	};

	class Artificial_Neural_Net {

	public:
		Artificial_Neural_Net() = delete;
		Artificial_Neural_Net(std::vector<unsigned int> topology, std::string name, double learningRate = 0.01);
		~Artificial_Neural_Net() = default;

		//--- Void functions ---//
		void Train(const Data_Set& trainSet, const Data_Set& testSet, unsigned short int epochs, bool printErrors = false, unsigned short int acceptanceCriteria = 90);

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
}