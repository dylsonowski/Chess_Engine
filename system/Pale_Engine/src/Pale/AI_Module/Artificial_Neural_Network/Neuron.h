#pragma once

namespace Pale::AI_Module {
	class Neuron {

	public:
		Neuron() = delete;
		Neuron(unsigned short int neuronId, unsigned short int layerId, double initialValue, bool inputNeuron, unsigned int inputWeightsNumber);
		~Neuron() = default;

		//--- Getters & Setters ---//
		inline double GetNeuronValue() const { return _neuronValue; }
		inline double GetWeight(unsigned short int iterator) {
			try {
				if (iterator >= _weights.size())
					throw PaleEngineException("Exception happened!", 'e', "Neuron.h", 15, "GetWeight()", VEC_OUT_OF_RANGE);

				return _weights.at(iterator);
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
		inline unsigned short int GetInputWeightsNumber() const { return _weights.size(); }
		inline void SetInputWeight(unsigned short int weightId, double value) { _weights.at(weightId) = value; }
		inline void SetNeuronValue(double value) { _neuronValue = value; }

		//--- Void functions ---//
		void CalculateNeuronValue(const Math::Matrix& previousLayer, double bias);
		inline void ApplyActivationFunction(std::function<double(double)> activationFunction) { _neuronValue = activationFunction(_neuronValue); }

	private:
		void InitializeInputWeights(unsigned int inputWeightsNumber);

		unsigned short int _layerId, _neuronId;
		double _neuronValue;
		std::vector<double> _weights;
	};
}