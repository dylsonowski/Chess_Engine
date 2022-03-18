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

		//--- Void functions ---//
		void RecalculateLayerValues();

		//--- Returning functions ---//
		Math::Matrix& ConvertToMatrix() const;

	private:
		unsigned short int _layerId;
		bool _inputLayer, _outputLayer;
		std::vector<std::shared_ptr<Neuron>> _layer;
		double _bias;
	};
}