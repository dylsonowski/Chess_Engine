#include "palepch.h"
#include "Layer.h"

namespace Pale::AI_Module {
    Layer::Layer(unsigned short int layerId, unsigned short int neuronsNumber, double biasValue, bool outputLayer, std::optional<unsigned short int> previousLayerSize) : _layerId(layerId), _outputLayer(outputLayer) {
        for (int neuronsIterator = 0; neuronsIterator < neuronsNumber; neuronsIterator++) {
            if (previousLayerSize.has_value()) {
                _inputLayer = false;
                _layer.emplace_back(std::make_shared<Neuron>(neuronsIterator, layerId, 0, false, previousLayerSize.value()));
                _biases.emplace_back(biasValue);
            }
            else {
                _inputLayer = true;
                _layer.emplace_back(std::make_shared<Neuron>(neuronsIterator, layerId, 0, true, 0));
            }
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

    void Layer::UpdateLayerWeights(const Math::Matrix& deltaWeightsMatrix) {
        try {
            Math::Matrix tempWeightsMatrix = GenerateWeightsMatrix();
            if(deltaWeightsMatrix.GetRowsNumber() != tempWeightsMatrix.GetRowsNumber() || deltaWeightsMatrix.GetColumnsNumber() != tempWeightsMatrix.GetColumnsNumber())
                throw PaleEngineException("Exception happened!", 'e', "Layer.cpp", 49, "UpdateLayerWeights()", NN__DELTA_WEIGHTS_MATRIX_INVALID_SIZE);

            unsigned int updatedWeightsNumber = 0;
            for (int neuronIterator = 0; neuronIterator < _layer.size(); neuronIterator++) {
                for (int neuronWeightIterator = 0; neuronWeightIterator < _layer.at(neuronIterator)->GetInputWeightsNumber(); neuronWeightIterator++) {
                    _layer.at(neuronIterator)->SetInputWeight(neuronWeightIterator, _layer.at(neuronIterator)->GetWeight(neuronWeightIterator) + deltaWeightsMatrix.GetValue(neuronIterator, neuronWeightIterator));
                    updatedWeightsNumber++;
                }
            }

            PALE_ENGINE_INFO("Layer.cpp->UpdateLayerWeights() [59]: Weights values for layer {0} has been updated! Number of updated weights: {1}/{2}.", _layerId, updatedWeightsNumber, deltaWeightsMatrix.GetRowsNumber() * deltaWeightsMatrix.GetColumnsNumber());
        }
        catch (PaleEngineException& exception) {
            if (exception.GetType() == 'e')
                PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
            else if (exception.GetType() == 'w')
                PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
        }
    }

    void Layer::UpdateLayerBiases(const Math::Matrix& deltaBiasesMatrix) {
        if (deltaBiasesMatrix.GetRowsNumber() != _biases.size() || deltaBiasesMatrix.GetColumnsNumber() != 1)
            throw PaleEngineException("Exception happened!", 'e', "Layer.cpp", 49, "UpdateLayerBiases()", NN__DELTA_BIASES_MATRIX_INVALID_SIZE);

        unsigned int updatedBiasesNumber = 0;
        for (int biasIterator = 0; biasIterator < _biases.size(); biasIterator++) {
            double tempBiasValue = _biases.at(biasIterator) + deltaBiasesMatrix.GetValue(biasIterator, 0);
            _biases.at(biasIterator) += deltaBiasesMatrix.GetValue(biasIterator, 0);
            assert(AssertionHandling(_biases.at(biasIterator) == tempBiasValue, "Layer.cpp->UpdateLayerBiases() [77]: Assertion failed! Bias value = " + std::to_string(_biases.at(biasIterator)) + " should be equal: " + std::to_string(tempBiasValue) + "!"));
            updatedBiasesNumber++;
        }

        PALE_ENGINE_INFO("Layer.cpp->UpdateLayerBiases() [81]: Biases values for layer {0} has been updated! Number of updated biases: {1}/{2}.", _layerId, updatedBiasesNumber, deltaBiasesMatrix.GetRowsNumber() * deltaBiasesMatrix.GetColumnsNumber());
    }

    void Layer::LoadNeuronInputWeights(unsigned short int neuronId, const std::vector<double>& weightsValues) {
        try {
            if(_inputLayer)
                throw PaleEngineException("Exception happened!", 'e', "Layer.cpp", 86, "LoadNeuronInputWeights()", NN__INVALID_LAYER);
            
            if (neuronId >= _layer.size())
                throw PaleEngineException("Exception happened!", 'e', "Layer.cpp", 89, "LoadNeuronInputWeights()", VEC_OUT_OF_RANGE);

            if(weightsValues.size() != _layer.at(neuronId)->GetInputWeightsNumber())
                throw PaleEngineException("Exception happened!", 'e', "Layer.cpp", 92, "LoadNeuronInputWeights()", NN__INVALID_DATA_SIZE);

            for (int neuronInputWeightsIterator = 0; neuronInputWeightsIterator < _layer.at(neuronId)->GetInputWeightsNumber(); neuronInputWeightsIterator++) {
                _layer.at(neuronId)->SetInputWeight(neuronInputWeightsIterator, weightsValues.at(neuronInputWeightsIterator));

                double actualWeightValue = _layer.at(neuronId)->GetWeight(neuronInputWeightsIterator), expectedWeightValue = weightsValues.at(neuronInputWeightsIterator);

                assert(AssertionHandling(_layer.at(neuronId)->GetWeight(neuronInputWeightsIterator) == weightsValues.at(neuronInputWeightsIterator), "Artificial_Neural_Net.cpp->LoadNeuronInputWeights() [99]: Assertion failed! Value = " + std::to_string(actualWeightValue) + " should be equal: " + std::to_string(expectedWeightValue) + "!"));
            }

            PALE_ENGINE_INFO("Artificial_Neural_Net.cpp->LoadNeuronInputWeights() [102]: Input weights loading process for neuron {0} in layer {1} ended up successfully!", neuronId, _layerId);
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