#pragma once
#include "Layer.h"

namespace Pale::AI_Module {
    class Activation_Layer : public Layer {

        public:
            Activation_Layer() = delete;
            Activation_Layer(unsigned short int layerId, std::function<double(double)> activationFunction, std::function<double(double)> dActivationFunction, std::pair<unsigned short int, unsigned short int> inputSize, unsigned short int inputDepth = 3);
            ~Activation_Layer() = default;

            std::vector<Math::Matrix> FeedForward(const std::vector<Math::Matrix>& previousLayer) override;
            std::vector<Math::Matrix> BackPropagation(const std::vector<Math::Matrix>& previousLayer) override;

        private:
            std::array<unsigned short int, 3> _inputSize, _outputSize;
            std::function<double(double)> _activationFunction, _dActivationFunction; 
    };
}