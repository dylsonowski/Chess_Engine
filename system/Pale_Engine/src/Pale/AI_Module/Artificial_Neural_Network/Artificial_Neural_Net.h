#pragma once
#include "Layer.h"

namespace Pale::AI_Module {
	class Artificial_Neural_Net {

	public:
		Artificial_Neural_Net() = delete;
		Artificial_Neural_Net(std::vector<unsigned int> topology, std::string name, double learningRate = 0.01);
		~Artificial_Neural_Net() = default;

		//--- Void functions ---//
		void FeedForward(const std::vector<double>& inputData);
		void BackPropagation(const std::vector<double>& targetData);

		//--- Returning functions ---//
		std::string ToString() const;

	private:
		std::string _networkName;
		std::vector<unsigned int> _topology;
		std::vector<std::shared_ptr<Layer>> _network;
		double _learningRate;
	};

	inline std::ostream& operator<<(std::ostream& os, const Artificial_Neural_Net& data) {
		return os << data.ToString();
	}
}