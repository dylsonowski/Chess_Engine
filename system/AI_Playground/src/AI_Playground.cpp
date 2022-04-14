#include "palepch.h"

int main(int argc, char** argv) {
	LOG_SYSTEM_INIT(LOGGING_METHOD::BASIC);
	//--- Matrix testing ---//
	/*Pale::Math::Matrix firstMatrix(2, 1, {2,1,2,1}), secondMatrix(2, 1, {2,2,2,2});
	std::cout << "A:\n" << firstMatrix << "\nB:\n" << secondMatrix << "\n\n";
	std::cout << "Map function on matrix result:\n" << firstMatrix.Map(Pale::Math::DSigmoigFunction) << "\n\n\n";
	std::cout << "Elemental matrix multiply result (without summing):\n" << Pale::Math::Matrix::ElementalSummary(firstMatrix, secondMatrix, false) << "\n\n\n";
	std::cout << "Elemental matrix multiply result (with summing):\n" << Pale::Math::Matrix::ElementalSummary(firstMatrix, secondMatrix, true) << "\n\n\n";
	std::cout << "Add matrices result:\n" << firstMatrix + secondMatrix << "\n\n\n";
	std::cout << "Subtract matrices result:\n" << firstMatrix - secondMatrix << "\n\n\n";
	std::cout << "Multiply matrices result:\n" << firstMatrix * secondMatrix << "\n\n\n";
	std::cout << "Add matrix and scalar result:\n" << firstMatrix + 2 << "\n\n\n";
	std::cout << "Subtract matrix and scalar result:\n" << firstMatrix - 2 << "\n\n\n";
	std::cout << "Multiply matrix and scalar result:\n" << firstMatrix * 2 << "\n\n\n";
	std::cout << "Transpose matrix result:\n" << ~firstMatrix << "\n\n\n";*/

	//--- AI testing ---//
	Pale::AI_Module::Artificial_Neural_Net newNeuralNetwork({ 2,2,1 }, "XOR Recognizing Neural Net");
	newNeuralNetwork.FeedForward({ 1,1 });
	newNeuralNetwork.BackPropagation({ 0 });

	std::cout << newNeuralNetwork << "\n";
	std::cin.get();
	return 0;
}