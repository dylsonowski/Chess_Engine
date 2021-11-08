#include "palepch.h"
#include "Automation_Tests.h"

namespace testing {
	Automation_Tests::Automation_Tests(std::string fileName) {
		std::string filePath = "test_scenarios/" + fileName + ".test";
		_inputFilePath = filePath;
		ProcessInputFile();
	}

	void Automation_Tests::ProcessInputFile() {
		try {
			unsigned int boardDimentionY, boardDimentionX;
			boardDimentionY = _inputFilePath[_inputFilePath.find("(") + 1] - '0';
			boardDimentionX = _inputFilePath[_inputFilePath.find(")") - 1] - '0';

			std::ifstream testFile(_inputFilePath);
			if (!testFile.good())
				throw PaleEngineException("Exception happened!", 'e', "Automation_Tests.cpp", 20, "ProcessInputFile", READ_FILE_ERROR);

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
						Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>> newTempTestScenario = Pale::Chess_Logic::ConvertBoard<std::shared_ptr<Pale::Chess_Logic::Pieces>>(tempTestScenario);
						_testsList.emplace_back(std::make_pair(newTempTestScenario, tempTestCaseList));

						if (_testsList.back().first.GetRowNumber() != boardDimentionY || _testsList.back().first.GetColumnNumber() != boardDimentionX)
							throw PaleEngineException("Exception happened!", 'e', "Automation_Tests.cpp", 399, "ProcessInputFile", "One of dimentions of the board in test scenario is different than that is define in test file name. Dimentions from file name: " + std::to_string(boardDimentionY) + "x" + std::to_string(boardDimentionX) + ".Dimentions from test file : " + std::to_string(_testsList.back().first.GetRowNumber()) + "x" + std::to_string(_testsList.back().first.GetColumnNumber()) + ".");
					}
					else if (line.find("@") != std::string::npos) {
						Test_Case tempTestCase;
						tempTestCase.m_boardDimentionX = boardDimentionX;
						tempTestCase.m_boardDimentionY = boardDimentionY;

						if (line.find("@PieceMove") != std::string::npos) {
							tempTestCase.m_type = TEST_TYPE::PIECE_MOVE;
							while (line.find("(END)") == std::string::npos) {
								getline(testFile, line);
								tempTestCase.m_arguments.clear();
								if (line.find("|") != std::string::npos) {
									tempTestCase.m_arguments.emplace_back(line.substr(line.find("{") + 1, line.find(",") - 5));
									tempTestCase.m_arguments.emplace_back(line.substr(line.find(",") + 2, 1));
									tempTestCase.m_expecterResult = line.substr(line.find("|") + 2);
									_testsList.back().second.emplace_back(tempTestCase);
								}
							}
						}
						else if (line.find("@MoveGeneration") != std::string::npos) {
							tempTestCase.m_type = TEST_TYPE::MOVE_GENERATE;
							while (line.find("(END)") == std::string::npos) {
								getline(testFile, line);
								tempTestCase.m_arguments.clear();
								if (line.find("|") != std::string::npos) {
									tempTestCase.m_arguments.emplace_back(line.substr(line.find("{") + 1, 1));
									tempTestCase.m_arguments.emplace_back(line.substr(line.find(",") + 2, 1));
									tempTestCase.m_expecterResult = line.substr(line.find("|") + 2);
									_testsList.back().second.emplace_back(tempTestCase);
								}
							}
						}
						else
							throw PaleEngineException("Exception happened!", 'w', "Automation_Tests.cpp", 399, "ProcessInputFile", "The given test type '" + line + "' does not match the list of test types.");
					}
				}
				testFile.close();
			}
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_TESTS_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_TESTS_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	void Automation_Tests::ExecuteTestCases(std::vector<TEST_TYPE> testsToExecute) const {
		try {
			unsigned int passedTests = 0, failedTests = 0, skippedTests = 0;
			for (const auto& mainIterator : _testsList) {
				Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>> testScenario = mainIterator.first;

				for (const auto& casesIterator : mainIterator.second) {
					if (testsToExecute.size() != 0) {
						if (std::find(testsToExecute.begin(), testsToExecute.end(), casesIterator.m_type) == testsToExecute.end())
							throw PaleEngineException("Exception happened!", 'e', "Automation_Tests.cpp", 20, "ExecuteTestCases", TEST_CASE_SEARCH_FAIL);
					}

					if (casesIterator.m_type == TEST_TYPE::PIECE_MOVE) {
						OWNERS whichTurn;
						bool expectedResult;
						if (casesIterator.m_arguments.at(1) == "b")
							whichTurn = OWNERS::BLACK;
						else
							whichTurn = OWNERS::WHITE;

						if (casesIterator.m_expecterResult == "true")
							expectedResult = true;
						else
							expectedResult = false;

						Move_Command processedMove = Pale::Chess_Logic::ProcessMoveCommand(casesIterator.m_arguments.at(0), whichTurn, testScenario);
						const bool result = testScenario.MovePiece(processedMove, *testScenario.GetPlateValue(processedMove.m_startPos.first, processedMove.m_startPos.second));

						if (result == expectedResult) {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
							std::cout << "Test -> P\n";
							passedTests++;
						}
						else {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
							std::cout << "Test -> F\n";
							failedTests++;
						}
					}
					else if (casesIterator.m_type == TEST_TYPE::MOVE_GENERATE) {
						const std::vector<Move_Command> result = testScenario.GetPlateValue(std::stoi(casesIterator.m_arguments.at(0)), std::stoi(casesIterator.m_arguments.at(1)))->GenerateLegitMoves(testScenario.GetBoardRef());

						if (result.size() == std::stoi(casesIterator.m_expecterResult)) {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
							std::cout << "Test -> P\n";
							passedTests++;
						}
						else {
							SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
							std::cout << "Test -> F\n";
							failedTests++;
						}
					}
				}
			}

			PrintTestReport(passedTests, failedTests, skippedTests);
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_TESTS_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_TESTS_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
		}
	}

	void Automation_Tests::PrintTestReport(unsigned int passedTests, unsigned int failedTests, unsigned int skippedTests) const {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		std::cout << "\nTests result:\n";
		std::cout << "Passed tests: " << passedTests << ",\n";
		std::cout << "Failed tests: " << failedTests << ",\n";
		std::cout << "Skipped tests: " << skippedTests << ".\n";
	}
}