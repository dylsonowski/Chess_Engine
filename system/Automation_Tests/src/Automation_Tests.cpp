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
		Pale::Chess_Logic::Board_Representation<std::string> tempTestScenario(boardDimentionY, boardDimentionX, true);
		std::vector<Test_Case> tempTestCaseList;
		while (getline(testFile, line)) {
			if (line.find("<---") != std::string::npos && line.find("--->") != std::string::npos) {
				for (int rowIt = 0; rowIt < boardDimentionY; rowIt++) {
					getline(testFile, line);
					for (int columnIt = 0; columnIt < boardDimentionX; columnIt++) {
						std::string tempPiece = line.substr(1, 2);
						tempTestScenario.SetPlateValue(rowIt, columnIt, tempPiece);
						line.erase(0, 3);
					}
				}
				Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>> tempTestScenario = Pale::Chess_Logic::ConvertBoard<std::shared_ptr<Pale::Chess_Logic::Pieces>>(tempTestScenario); //TODO: Copple of converting function missing! Add it!
				_testsList.emplace_back(std::make_pair(tempTestScenario, tempTestCaseList));

				if (_testsList.back().first.GetRowNumber() != boardDimentionY || _testsList.back().first.GetColumnNumber() != boardDimentionX) {
					PALE_TESTS_ERROR("One of dimentions of the board in test scenario is different than that is define in test file name. Dimentions from file name: {0}x{1}. Dimentions from test file: {2}x{3}.", boardDimentionY, boardDimentionX, _testsList.back().first.GetRowNumber(), _testsList.back().first.GetColumnNumber());
					break;
				}
			}
			else if (line.find("@") != std::string::npos) {
				Test_Case tempTestCase;
				tempTestCase.m_boardDimentionX = boardDimentionX;
				tempTestCase.m_boardDimentionY = boardDimentionY;
				if (line.find("@PieceMove") != std::string::npos) {
					tempTestCase.m_type = TEST_TYPE::PIECE_MOVE;
					while (line.find("(END)") == std::string::npos) {
						getline(testFile, line);
						if (line.find("|") != std::string::npos) {
							tempTestCase.m_arguments.emplace_back(line.substr(line.find("{") + 1, line.find("}") - 5));
							tempTestCase.m_expecterResult = line.substr(line.find("|") + 2);
							_testsList.back().second.emplace_back(tempTestCase);
						}
					}
				}
				else if (line.find("@MoveGeneration") != std::string::npos) {
					tempTestCase.m_type = TEST_TYPE::MOVE_GENERATE;
					while (line.find("(END)") == std::string::npos) {
						getline(testFile, line);
						if (line.find("|") != std::string::npos) {
							tempTestCase.m_arguments.emplace_back(line.substr(line.find("{") + 1, line.find(",") - 5));
							tempTestCase.m_arguments.emplace_back(line.substr(line.find(",") + 2, line.find("}") - 5));
							tempTestCase.m_expecterResult = line.substr(line.find("|") + 1);
							_testsList.back().second.emplace_back(tempTestCase);
						}
					}
				}
				else {
					PALE_TESTS_WARN("The given test type '{0}' does not match the list of test types.", line);
					break;
				}

				/*while (line != "(END)") {
					getline(testFile, line);
				}*/
			}
		}
		testFile.close();
	}
}
