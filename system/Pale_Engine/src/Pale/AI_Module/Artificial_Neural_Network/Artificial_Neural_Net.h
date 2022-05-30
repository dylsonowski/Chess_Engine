#pragma once
#include "Artificial_Layer.h"
#include "../Data_Set.h"

namespace Pale::AI_Module {
	class Artificial_Neural_Net {

	public:
		Artificial_Neural_Net() = delete;
		Artificial_Neural_Net(std::vector<unsigned int> topology, std::string name, double learningRate = 0.01);
		~Artificial_Neural_Net() = default;

		//--- Void functions ---//
		void Train(const Data_Set& trainSet, const Data_Set& testSet, unsigned short int epochs, bool printErrors = false, float acceptanceCriteriaPerNeuron = 0.2f, bool pauseTraining = false);
		void SaveWeights(std::optional<std::string> fileName, std::string weightsFilePath = "./weights", bool extractionLogs = false) const;
		void LoadWeights(std::string fileName, std::string weightsFilePath = "./weights", bool importLogs = false);

		//--- Returning functions ---//
		std::string ToString() const;

	private:
		void FeedForward(const std::vector<double>& inputData);
		void BackPropagation(const std::vector<double>& targetData);
		const std::vector<double> Predict(const std::vector<double>& inputData);

		std::string _networkName;
		std::vector<unsigned int> _topology;
		std::vector<std::shared_ptr<Artificial_Layer>> _network;
		double _learningRate, _accumulatedOutputError;
	};

	inline std::ostream& operator<<(std::ostream& os, const Artificial_Neural_Net& data) {
		return os << data.ToString();
	}
}