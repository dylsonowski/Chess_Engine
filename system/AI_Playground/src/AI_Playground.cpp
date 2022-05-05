#include "palepch.h"

int main(int argc, char** argv) {
	try {
		LOG_SYSTEM_INIT(LOGGING_METHOD::BASIC);
		//--- Json handling testing ---//
		/*nlohmann::json file1 = {
			{"name", "NN 1"},
			{"layer1", {
				{"inputWeights", {0.1, 0.4}},
				{"inputWeights", {0.2, 0.2}}
			}},
			{"layer2", {
				{"inputWeights", {0.5}}
			}}
		};

		std::cout << file1.dump(4) << "\n\n\n";

		std::ofstream outputJsonFile("json/json testing/testFile1.json");
		if (!outputJsonFile.good())
			throw PaleEngineException("Exception happened!", 'e', "AI_Playground.h", 22, "main()", FILE_OPEN_ERROR);

		outputJsonFile << std::setw(4) << file1;
		outputJsonFile.close();

		std::ifstream inputJsonFile("json/json testing/testFile1.json");
		if (!inputJsonFile.good())
			throw PaleEngineException("Exception happened!", 'e', "AI_Playground.h", 29, "main()", FILE_OPEN_ERROR);

		nlohmann::json readJsonFile;
		readJsonFile << inputJsonFile;
		std::vector<float> inputWeights = readJsonFile["layer1"]["inputWeights"];*/

		//--- Assertion testing ---//
		/*assert(AssertionHandling(1 == 1, "AI_Playground.cpp->main() [6]: Assertion failed! Value = " + std::to_string(1) + " should be equal: " + std::to_string(1) + "!"));
		assert(AssertionHandling(1 == 0, "AI_Playground.cpp->main() [6]: Assertion failed! Value = " + std::to_string(1) + " should be equal: " + std::to_string(0) + "!"));*/

		//--- Matrix testing ---//
		/*Pale::Math::Matrix firstMatrix(2, 1, {2,1,2,1}), secondMatrix(2, 1, {2,2,2,2});
		std::cout << "A:\n" << firstMatrix << "\nB:\n" << secondMatrix << "\n\n";
		std::cout << "Map function on matrix result:\n" << firstMatrix.Map(Pale::Math::DSigmoigFunction) << "\n\n\n";
		std::cout << "Elemental matrix multiply result (without summing):\n" << Pale::Math::Matrix::HadamardMultiplication(firstMatrix, secondMatrix, false) << "\n\n\n";
		std::cout << "Elemental matrix multiply result (with summing):\n" << Pale::Math::Matrix::HadamardMultiplication(firstMatrix, secondMatrix, true) << "\n\n\n";
		std::cout << "Add matrices result:\n" << firstMatrix + secondMatrix << "\n\n\n";
		std::cout << "Subtract matrices result:\n" << firstMatrix - secondMatrix << "\n\n\n";
		std::cout << "Multiply matrices result:\n" << firstMatrix * secondMatrix << "\n\n\n";
		std::cout << "Add matrix and scalar result:\n" << firstMatrix + 2 << "\n\n\n";
		std::cout << "Subtract matrix and scalar result:\n" << firstMatrix - 2 << "\n\n\n";
		std::cout << "Multiply matrix and scalar result:\n" << firstMatrix * 2 << "\n\n\n";
		std::cout << "Transpose matrix result:\n" << ~firstMatrix << "\n\n\n";*/

		//--- AI testing ---//
		std::vector<std::pair<std::vector<double>, std::vector<double>>> xor = {
			{{0, 0}, {0}},
			{{0, 1}, {1}},
			{{1, 0}, {1}},
			{{1, 1}, {0}}
		};

		std::vector<std::vector<double>> inputData, targetData;
		for (int dataSetIterator = 0; dataSetIterator < 1000; dataSetIterator++) {
			auto chosenEntry = xor.at(Pale::Math::GenerateRandomNumber<int>(0, xor.size() - 1));
			inputData.emplace_back(chosenEntry.first);
			targetData.emplace_back(chosenEntry.second);
		}

		Pale::AI_Module::Data_Set xorDataSet(inputData, targetData);
		std::pair<Pale::AI_Module::Data_Set, Pale::AI_Module::Data_Set> splitedDataSet = Pale::AI_Module::SplitDataSet(xorDataSet, 0.7f);
		Pale::AI_Module::Artificial_Neural_Net newNeuralNetwork({ 2,2,1 }, "XOR Recognizing Neural Net", 0.1f);
		newNeuralNetwork.Train(splitedDataSet.first.ShuffleDataSet(), splitedDataSet.second, 50, false, 0.2f);

		/*Pale::AI_Module::Artificial_Neural_Net freshNeuralNetwork({ 2,2,1 }, "Newly created XOR Neural Net");
		freshNeuralNetwork.LoadWeights("SaveWeightsTestFile", "./weights/testing", true);*/

		std::cout << newNeuralNetwork << "\n";
		//std::cout << freshNeuralNetwork << "\n";
		std::cin.get();
	}
	catch (PaleEngineException& exception) {
		if (exception.GetType() == 'e')
			PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
		else if (exception.GetType() == 'w')
			PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
	}
	return 0;
}