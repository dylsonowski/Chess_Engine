#pragma once
#include "../../Math/Matrix.h"

namespace Pale::AI_Module {
	class Layer {

	public:
		Layer() = delete;
		virtual ~Layer() = default;

		//--- Getters & Setters ---//
		inline unsigned short int GetLayerId() const { return _layerId; }
		inline bool IsInputLayer() const { return _inputLayer; }
		inline bool IsOutputLayer() const { return _outputLayer; }

		//--- Void functions ---//
		virtual std::vector<Math::Matrix> FeedForward(const std::vector<Math::Matrix>& previousLayer) = 0;
		virtual std::vector<Math::Matrix> BackPropagation(const std::vector<Math::Matrix>& previousLayer) = 0;

	protected:
		Layer(unsigned short int layerId, bool inputLayer, bool outputLayer): _layerId(layerId),
																			  _inputLayer(inputLayer),
																			  _outputLayer(outputLayer) {}

		unsigned short int _layerId;
		bool _inputLayer, _outputLayer;
	};
} // namespace Pale::AI_Module