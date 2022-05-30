#pragma once
#include "Layer.h"

namespace Pale::AI_Module {
	class Fully_Connected_Layer : public Layer {

	public:
		Fully_Connected_Layer() = delete;
		Fully_Connected_Layer(unsigned short int layerId, unsigned short int layerNeurons, unsigned short int inputSize, std::function<double(double)> activationFunction, std::function<double(double)> dActivationFunction, bool inputLayer = false, bool outputLayer = true);
		~Fully_Connected_Layer() = default;

		std::vector<Math::Matrix> FeedForward(const std::vector<Math::Matrix>& previousLayer) override;
		std::vector<Math::Matrix> BackPropagation(const std::vector<Math::Matrix>& previousLayer) override { return { Math::Matrix() }; } // TODO: Needs to be implemented!

	private:
		unsigned short int _inputSize, _layerNeurons;
		Math::Matrix _weightsMatrix;
		std::function<double(double)> _activationFunction, _dActivationFunction;
	};
} // namespace Pale::AI_Module