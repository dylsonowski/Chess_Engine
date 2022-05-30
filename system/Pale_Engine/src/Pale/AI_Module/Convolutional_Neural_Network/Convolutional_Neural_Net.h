#pragma once
#include "Layer/Layer.h"
#include "../Data_Set.h"

namespace Pale::AI_Module {
    class Convolutional_Neural_Net {

        public:
            Convolutional_Neural_Net() = delete;
            Convolutional_Neural_Net(unsigned short int inputSize, unsigned short int outputSize, std::string networkName, double learningRate = 0.01);
            ~Convolutional_Neural_Net() = default;

            //--- Void functions ---//
            void AddOutputLayer();

        private:
            std::string _networkName;
            std::vector<std::string> _topology;
            std::vector<std::shared_ptr<Layer>> _network;
            double _learningRate;
    };
} // namespace Pale::AI_Module