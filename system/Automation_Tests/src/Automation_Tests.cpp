#include "palepch.h"
#include "Automation_Tests.h"

using std::placeholders::_1;

Automation_Tests::Automation_Tests(std::string fileName) {
	std::string filePath = "test_scenarios/" + fileName + ".test";
	_inputFilePath = filePath;
	ProcessInputFile();
}

void Automation_Tests::ProcessInputFile() {
	unsigned int boardDimentionY, boardDimentionX;
	boardDimentionY = _inputFilePath[_inputFilePath.find("(") + 1] - '0';
	boardDimentionX = _inputFilePath[_inputFilePath.find(")") - 1] - '0';

	std::ifstream testFile(_inputFilePath);
	if (testFile.is_open()) {
		std::string line;
		std::vector<std::vector<std::string>> tempTestScenario;
		while (getline(testFile, line)) {
			if (line.find("<---") != std::string::npos && line.find("--->") != std::string::npos) {
				for (int rowIt = 0; rowIt < boardDimentionY; rowIt++) {
					getline(testFile, line);
					std::vector<std::string> tempRow;
					for (int columnIt = 0; columnIt < boardDimentionX; columnIt++) {
						if (line.length() > 3) {
							std::string tempPiece = line.substr(1, 2);
							tempRow.emplace_back(tempPiece);
							line.erase(0, 3);
						}
					}
					tempTestScenario.emplace_back(tempRow);
				}

				if (tempTestScenario.size() < boardDimentionY || tempTestScenario.at(0).size() < boardDimentionX) {
					PALE_TESTS_ERROR("One of dimentions of the board in test scenario is different than that is define in test file name. Dimentions from file name: {0}x{1}. Dimentions from test file: {2}x{3}.", boardDimentionY, boardDimentionX, tempTestScenario.size(), tempTestScenario.at(0).size());
					break;
				}
			}
			else if (line.find("@") != std::string::npos) {
				Test_Case tempTestCase;
				Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>> tempTestScenario(boardDimentionY, boardDimentionX, true);
				tempTestCase.m_boardDimentionX = boardDimentionX;
				tempTestCase.m_boardDimentionY = boardDimentionY;
				if (line.find("@PieceMove") != std::string::npos) {
					tempTestCase.m_type = TEST_TYPE::PIECE_MOVE;
					//tempTestCase.m_functionPointer = reinterpret_cast<bool(*)(Move_Command&, Pale::Chess_Logic::Pieces&)>(tempTestCase.m_functionPointer);
					while (line != "(END)") {
						getline(testFile, line);
						if (line.find("|") != std::string::npos) {
							tempTestCase.m_arguments.emplace_back(line.substr(line.find("(") + 1, line.find(")")));
							tempTestCase.m_expecterResult = line.substr(line.find("|") + 1);
						}
					}
				}
				else if (line.find("@MoveGeneration") != std::string::npos) {

				}
				else {
					PALE_TESTS_WARN("The given test type '{0}' does not match the list of test types.", line);
					break;
				}

				while (line != "(END)") {
					getline(testFile, line);
				}
			}
		}
		testFile.close();
	}
}
