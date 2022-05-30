#pragma once

namespace Pale::AI_Module {
    struct Data_Set {
		std::vector<std::pair<std::vector<double>, std::vector<double>>> m_data;
		bool m_testSet;
		unsigned short int m_dataSetSize;

		Data_Set() = delete;
		Data_Set(const std::vector<std::vector<double>>& inputData, const std::vector<std::vector<double>>& targetData, bool testSet = false) : m_testSet(testSet) {
            assert(AssertionHandling(inputData.size() == targetData.size(), "Data_Set.h->Data_Set constructor [11]: Assertion failed! " NN__INVALID_DATA_SIZE));

            for (int dataIterator = 0; dataIterator < inputData.size(); dataIterator++) {
                m_data.emplace_back(std::make_pair(inputData.at(dataIterator), targetData.at(dataIterator)));
            }
            m_dataSetSize = inputData.size();

            PALE_ENGINE_INFO("Artificial_Neural_Net.h->Data_Set constructor [18]: New Data_Set object has been created! Data set size: {0}. Test set: {1}.", m_dataSetSize, m_testSet);
		}

		Data_Set ShuffleDataSet() {
			std::random_device randomDevice;
			std::mt19937 generator(randomDevice());
			std::shuffle(m_data.begin(), m_data.end(), generator);

			PALE_ENGINE_TRACE("Artificial_Neural_Net.h->ShuffleDataSet() [36]: Data set has been shuffled!");

			return *this;
		}
	};

    inline std::pair<Data_Set, Data_Set> SplitDataSet(const Data_Set& dataSet, float splitDistribution) {
		std::vector<std::vector<double>> inputDataSet1, targetDataSet1, inputDataSet2, targetDataSet2;

		const unsigned short int dataSet1Size = Math::ProportionCalculation(splitDistribution * 100, dataSet.m_dataSetSize);
		for (int dataSetIterator = 0; dataSetIterator < dataSet.m_dataSetSize; dataSetIterator++) {
			if (dataSetIterator < dataSet1Size) {
				inputDataSet1.emplace_back(dataSet.m_data.at(dataSetIterator).first);
				targetDataSet1.emplace_back(dataSet.m_data.at(dataSetIterator).second);
			}
			else {
				inputDataSet2.emplace_back(dataSet.m_data.at(dataSetIterator).first);
				targetDataSet2.emplace_back(dataSet.m_data.at(dataSetIterator).second);
			}
		}

		PALE_ENGINE_INFO("Artificial_Neural_Net.h->SplitDataSet() [83]: Data set has been splinted! Data set distribution: {0}/{1}. Data sets sizes: {2}/{3}.", splitDistribution, 1 - splitDistribution, inputDataSet1.size(), inputDataSet2.size());

		return std::make_pair(Data_Set(inputDataSet1, targetDataSet1), Data_Set(inputDataSet2, targetDataSet2, true));
	}
}