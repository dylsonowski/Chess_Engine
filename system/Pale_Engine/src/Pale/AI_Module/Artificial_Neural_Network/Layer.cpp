#include "palepch.h"
#include "Layer.h"

namespace Pale::AI_Module {
    Layer::Layer(unsigned short int layerId, unsigned short int neuronsNumber, double biasValue, bool outputLayer, std::optional<unsigned short int> previousLayerSize) : _layerId(layerId), _outputLayer(outputLayer) {
        for (int neuronsIterator = 0; neuronsIterator < neuronsNumber; neuronsIterator++) {
            if (previousLayerSize.has_value()) {
                _inputLayer = false;
                _layer.emplace_back(std::make_shared<Neuron>(neuronsIterator, layerId, 0, false, previousLayerSize.value()));
            }
            else {
                _inputLayer = true;
                _layer.emplace_back(std::make_shared<Neuron>(neuronsIterator, layerId, 0, true, 0));
            }

            _biases.emplace_back(biasValue);
        }

        PALE_ENGINE_INFO("Layer.cpp->Layer constructor [17]: New layer has been created! Layer ID: {0}. Number of neurons: {1}. Input layer: {2}. Output layer: {3}.", layerId, _layer.size(), _inputLayer, _outputLayer);
    }

    void Layer::RecalculateLayerValues(std::optional<Math::Matrix> previousLayer) {
        try {
            if (previousLayer.has_value()) {
                if(_layer.size() != _biases.size())
                    throw PaleEngineException("Exception happened!", 'e', "Layer.cpp", 24, "RecalculateLayerValues()", NN__INCOMPATIBLE_BIASES_NUMBER);

                for (int layerIterator = 0; layerIterator < _layer.size(); layerIterator++) {
                    _layer.at(layerIterator)->CalculateNeuronValue(previousLayer.value(), _biases.at(layerIterator));
                }

                PALE_ENGINE_INFO("Layer.cpp->RecalculateLayerValues() [25]: Layer: {0} has been recalculated! Value of first neuron in the layer: {1}.", _layerId, _layer.at(0)->GetNeuronValue());
            }
            else
                PALE_ENGINE_INFO("Layer.cpp->RecalculateLayerValues() [29]: Layer: {0} recalculation didn't happened!.", _layerId);
        }
        catch (PaleEngineException& exception) {
            if (exception.GetType() == 'e')
                PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
            else if (exception.GetType() == 'w')
                PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
        }
    }

    Math::Matrix Layer::ConvertToMatrix() const {
        std::vector<double> tempLayerVector;
        for (const auto layerIterator : _layer) {
            tempLayerVector.push_back(layerIterator->GetNeuronValue());
        }

        Math::Matrix tempLayerMatrix(_layer.size(), 1, tempLayerVector);

        PALE_ENGINE_INFO("Layer.cpp->ConvertToMatrix() [40]: Layer has been converted into Matrix! Matrix dimensions: {0}x{1}.", tempLayerMatrix.GetRowsNumber(), tempLayerMatrix.GetColumnsNumber());
        return tempLayerMatrix;
    }
    
    std::string Layer::ToString(bool horizontalView) const {
        std::stringstream ss;
        for (int layerIterator = 0; layerIterator < _layer.size(); layerIterator++) {
            if (layerIterator == _layer.size() - 1)
                ss << _layer.at(layerIterator)->GetNeuronValue() << ".";
            else
                ss << _layer.at(layerIterator)->GetNeuronValue() << ", ";
        }

        return ss.str();
    }

    Math::Matrix Layer::GenerateWeightsMatrix() const {
        std::vector<double> tempWeightsVector;
        for (const auto layerIterator : _layer) {
            for (int neuronWeightsIterator = 0; neuronWeightsIterator < layerIterator->GetInputWeightsNumber(); neuronWeightsIterator++) {
                tempWeightsVector.emplace_back(layerIterator->GetWeight(neuronWeightsIterator));
            }
        }

        Math::Matrix tempWeightsMatrix(_layer.size(), _layer.at(0)->GetInputWeightsNumber(), tempWeightsVector);

        PALE_ENGINE_INFO("Layer.cpp->GenerateWeightsMatrix() [79]: Weights matrix of layer {0} has been generated! Matrix dimensions: {1}x{2}. First matrix value: {3}.", _layerId, tempWeightsMatrix.GetRowsNumber(), tempWeightsMatrix.GetColumnsNumber(), tempWeightsMatrix.GetValue(0, 0));

        return tempWeightsMatrix;
    }
}