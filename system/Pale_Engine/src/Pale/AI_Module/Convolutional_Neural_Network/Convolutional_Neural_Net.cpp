#include "palepch.h"
#include "Convolutional_Neural_Net.h"
#include "Layer/Convolutional_Layer.h"
#include "Layer/Pooling_Layer.h"
#include "Layer/Activation_Layer.h"
#include "Layer/Fully_Connected_Layer.h"

namespace Pale::AI_Module {
    Convolutional_Neural_Net::Convolutional_Neural_Net(unsigned short int inputSize, unsigned short int outputSize, std::string networkName, double learningRate) 
        : _networkName(networkName), 
          _learningRate(learningRate) {
        _network.emplace_back(std::make_shared<Fully_Connected_Layer>(0, inputSize, 0, nullptr, nullptr, true, false));
        _topology.emplace_back("Full(" + std::to_string(inputSize) + ")");
    }

    void Convolutional_Neural_Net::AddOutputLayer() {
        _network.emplace_back(std::make_shared<Fully_Connected_Layer>(_network.size(), ))
    }
}