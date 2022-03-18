#include "palepch.h"
#include "Layer.h"

namespace Pale::AI_Module {
    Layer::Layer(unsigned short int layerId, unsigned short int neuronsNumber, double biasValue, bool outputLayer, std::optional<unsigned short int> previousLayerSize) : _layerId(layerId), _outputLayer(outputLayer), _bias(biasValue) {
        for (int neuronsIterator = 0; neuronsIterator < neuronsNumber; neuronsIterator++) {
            if (previousLayerSize.has_value()) {
                _inputLayer = false;
                _layer.emplace_back(std::make_shared<Neuron>(neuronsIterator, layerId, 0, false, previousLayerSize));
            }
            else {
                _inputLayer = true;
                _layer.emplace_back(std::make_shared<Neuron>(neuronsIterator, layerId, 0, true, 0));
            }
        }

        PALE_ENGINE_INFO("Layer.cpp->Layer constructor [17]: New layer has been created! Layer ID: {0}. Number of neurons: {1}. Input layer: {2}. Output layer: {3}.", layerId, _layer.size(), _inputLayer, _outputLayer);
    }

    Math::Matrix& Layer::ConvertToMatrix() const {
        std::vector<double> tempLayerVector;
        for (const auto layerIterator : _layer) {
            tempLayerVector.push_back(layerIterator->GetNeuronValue());
        }

        Math::Matrix tempLayerMatrix(_layer.size(), 1, tempLayerVector);

        PALE_ENGINE_INFO("Layer.cpp->ConvertToMatrix() [28]: Layer has been converted into Matrix! Matrix dimensions: {0}x{1}.", tempLayerMatrix.GetRowsNumber(), tempLayerMatrix.GetColumnsNumber());
        return tempLayerMatrix;
    }
}