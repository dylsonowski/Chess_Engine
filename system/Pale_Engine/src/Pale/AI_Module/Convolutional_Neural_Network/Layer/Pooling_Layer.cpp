#include "palepch.h"
#include "Pooling_Layer.h"

namespace Pale::AI_Module {
	Pooling_Layer::Pooling_Layer(unsigned short int layerId, std::pair<unsigned short int, unsigned short int> inputSize, unsigned short int inputDepth, unsigned short int windowSize, POOLING_METHOD poolingMethods, unsigned short int stride)
		: Layer(layerId, false, false),
		  _poolingWindowSize(windowSize),
		  _poolingMethod(poolingMethods),
		  _stride(stride) {
		unsigned short int outputYSize = floor((inputSize.first - windowSize) / stride) + 1;
		unsigned short int outputXSize = floor((inputSize.second - windowSize) / stride) + 1;

		_inputSize = { inputSize.first, inputSize.second, inputDepth };
		_outputSize = { outputYSize, outputXSize, inputDepth };

		PALE_ENGINE_INFO("Pooling_Layer.cpp->Pooling_Layer constructor [13]: New pooling layer of id {0} has been created! Input size: {1}x{2}x{3}. Pooling window size: {4}. Stride: {5}. Pooling method: MAX_POOLING. Output size: {6}x{7}x{8}.");
	}

	std::vector<Math::Matrix> Pooling_Layer::FeedForward(const std::vector<Math::Matrix>& previousLayer) {
		try {
			assert(AssertionHandling(previousLayer.size() == _inputSize[2], "Pooling_Layer.cpp->FeedForward() [17]: Assertion failed! Value = " + std::to_string(previousLayer.size()) + " should be equal: " + std::to_string(_inputSize[2]) + "!"));

			assert(AssertionHandling(previousLayer.at(0).GetMatrixSize() == std::make_pair(_inputSize[0], _inputSize[1]), "Pooling_Layer.cpp->FeedForward() [19]: Assertion failed! Input of the pooling layer of id " + std::to_string(_layerId) + " is incompatible!"));

			std::vector<Math::Matrix> tempOutput, tempInputMask;
			if (_poolingMethod == POOLING_METHOD::MAX_POOLING) {
				for (const auto inputIterator : previousLayer) {
					std::pair<Math::Matrix, Math::Matrix> tempMaxPoolingOutput = Math::Matrix::MaxPoolingOperation(inputIterator, _poolingWindowSize, _stride);

					assert(AssertionHandling(tempMaxPoolingOutput.first.GetMatrixSize() == std::make_pair(_outputSize[0], _outputSize[1]), "Pooling_Layer.cpp->FeedForward() [30]: Max pooling output. " MATH__MATRICES_DIMENSIONS_INCORRECT));

					assert(AssertionHandling(tempMaxPoolingOutput.second.GetMatrixSize() == std::make_pair(_inputSize[0], _inputSize[1]), "Pooling_Layer.cpp->FeedForward() [32]: Max pooling input mask. " MATH__MATRICES_DIMENSIONS_INCORRECT));

					tempOutput.emplace_back(tempMaxPoolingOutput.first);
					tempInputMask.emplace_back(tempMaxPoolingOutput.second);
				}
			} else
				throw PaleEngineException("Exception happened!", 'e', "Pooling_Layer.cpp", 34, "FeedForward()", NN__INVALID_POOLING_METHOD);

			PALE_ENGINE_INFO("Pooling_Layer.cpp->FeedForward() [40]: Feed forward process for pooling layer {0} has been performed successfully! Output dimensions: {1}x{2}x{3}. Input mask dimensions: {4}x{5}x{6}.", _layerId, tempOutput.at(0).GetRowsNumber(), tempOutput.at(0).GetColumnsNumber(), tempOutput.size(), tempInputMask.at(0).GetRowsNumber(), tempInputMask.at(0).GetColumnsNumber(), tempInputMask.size());

			_inputPoolingMask = tempInputMask;
			return tempOutput;
		} catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
		}
	}

	std::vector<Math::Matrix> Pooling_Layer::BackPropagation(const std::vector<Math::Matrix>& previousLayer) {
		assert(AssertionHandling(previousLayer.size() == _outputSize[2], "Pooling_Layer.cpp->BackPropagation() [51]: Assertion failed! Value = " + std::to_string(previousLayer.size()) + " should be equal: " + std::to_string(_inputSize[2]) + "!"));

		assert(AssertionHandling(previousLayer.at(0).GetMatrixSize() == std::make_pair(_outputSize[0], _outputSize[1]), "Pooling_Layer.cpp->BackPropagation() [53]: Assertion failed! Previous layer size given into pooling layer of id " + std::to_string(_layerId) + " is incompatible!"));

		std::vector<Math::Matrix> tempOutput;
		for (int previousLayerDepthIterator = 0; previousLayerDepthIterator < previousLayer.size(); previousLayerDepthIterator++) {
			Math::Matrix tempMatrix(_inputSize[0], _inputSize[1], 0.f);
			std::vector<double> tempPreviousLayerValues = previousLayer.at(previousLayerDepthIterator).ConvertToVector();

			for (const auto previousLayerIterator : tempPreviousLayerValues) {
				for (int inputRowsIterator = 0; inputRowsIterator < _inputSize[0]; inputRowsIterator += _stride) {
					for (int inputColumnIterator = 0; inputColumnIterator < _inputSize[1]; inputColumnIterator += _stride) {
						for (int windowRowIterator = 0; windowRowIterator < _poolingWindowSize; windowRowIterator++) {
							for (int windowColumnIterator = 0; windowColumnIterator < _poolingWindowSize; windowColumnIterator++) {

								if (_inputPoolingMask.at(previousLayerDepthIterator).GetValue(inputRowsIterator + windowRowIterator, inputColumnIterator + windowColumnIterator) == 1) {
									tempMatrix.SetMatrixValue(inputRowsIterator + windowRowIterator, inputColumnIterator + windowColumnIterator, previousLayerIterator);
								}
							}
						}
					}
				}
			}

			tempOutput.emplace_back(tempMatrix);
		}

		PALE_ENGINE_INFO("Pooling_Layer.cpp->BackPropagation() [80]: Back propagation process for pooling layer {0} has been performed successfully! Output dimensions: {1}x{2}x{3}.", _layerId, tempOutput.at(0).GetRowsNumber(), tempOutput.at(0).GetColumnsNumber(), tempOutput.size());

		return tempOutput;
	}
} // namespace Pale::AI_Module