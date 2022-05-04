#pragma once
#include "Neuron.h"
#include "../Math/Matrix.h"

namespace Pale::AI_Module {
	class Layer	{

	public:
		Layer() = delete;
		Layer(unsigned short int layerId, unsigned short int neuronsNumber, double biasValue, bool outputLayer, std::optional<unsigned short int> previousLayerSize);
		~Layer() = default;

		//--- Getters & Setters ---//
		inline unsigned short int GetLayerId() const { return _layerId; }
		inline double GetNeuronValue(unsigned short int neuronId) const { 
			try {
				if (neuronId >= _layer.size())
					throw PaleEngineException("Exception happened!", 'e', "Layer.h", 16, "GetNeuronValue()", VEC_OUT_OF_RANGE);

				return _layer.at(neuronId)->GetNeuronValue();
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
		inline unsigned short int GetLayerSize() const { return _layer.size(); }
		inline bool IsInputLayer() const { return _inputLayer; }
		inline bool IsOutputLayer() const { return _outputLayer; }
		inline double GetBiasValue(unsigned short int biasId) const { return _biases.at(biasId); }
		inline void SetNeuronValue(unsigned short int neuronId, double value) { 
			try {
				if (neuronId >= _layer.size())
					throw PaleEngineException("Exception happened!", 'e', "Layer.h", 33, "SetNeuronValue()", VEC_OUT_OF_RANGE);

				_layer.at(neuronId)->SetNeuronValue(value);
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
		inline void SetNeuronWeight(unsigned short int neuronId, unsigned short int weightId, double value) { 
			try {
				if (neuronId >= _layer.size())
					throw PaleEngineException("Exception happened!", 'e', "Layer.h", 47, "SetNeuronWeight()", VEC_OUT_OF_RANGE);

				if (weightId >= _layer.at(neuronId)->GetInputWeightsNumber())
					throw PaleEngineException("Exception happened!", 'e', "Layer.h", 50, "SetNeuronWeight()", VEC_OUT_OF_RANGE);

				_layer.at(neuronId)->SetInputWeight(weightId, value);
			}
			catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
		inline void SetBiasValue(unsigned short int biasId, double value) { _biases.at(biasId) = value; }
		inline std::vector<double> GetNeuronInputWeights(unsigned short int neuronId) const {
			std::vector<double> neuronInputWeights;
			for (int neuronInputWeightsIterator = 0; neuronInputWeightsIterator < _layer.at(neuronId)->GetInputWeightsNumber(); neuronInputWeightsIterator++) {
				neuronInputWeights.emplace_back(_layer.at(neuronId)->GetWeight(neuronInputWeightsIterator));
			}

			return neuronInputWeights;
		}

		//--- Void functions ---//
		void RecalculateLayerValues(std::optional<Math::Matrix> previousLayer);
		void UpdateLayerWeights(const Math::Matrix& deltaWeightsMatrix);
		void UpdateLayerBiases(const Math::Matrix& deltaBiasesMatrix);
		void LoadNeuronInputWeights(unsigned short int neuronId, const std::vector<double>& weightsValues);

		//--- Returning functions ---//
		Math::Matrix ConvertToMatrix() const;
		std::string ToString(bool horizontalView = true) const;
		Math::Matrix GenerateWeightsMatrix() const;

	private:
		unsigned short int _layerId;
		bool _inputLayer, _outputLayer;
		std::vector<std::shared_ptr<Neuron>> _layer;
		std::vector<double> _biases;
	};
}