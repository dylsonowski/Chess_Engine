#include "palepch.h"
#include "Automation_Tests.h"

int main(int argv, char** argc) {
	try {
		LOG_SYSTEM_INIT(LOGGING_METHOD::BASIC);
		std::string testFileName = "Test_File1(8x8)";
		std::vector<Testing::TEST_TYPE> testsToExecute;

		if(argv < 3)
			throw PaleEngineException("Exception happened!", 'e', "Test_Core.cpp", 7, "main", TEST_TOO_FEW_ARGUMENTS);

		for (int iterator = 0; iterator < argv; iterator++) {
			if (argc[iterator] == "-f")
				testFileName = argc[iterator + 1];

			if (argc[iterator] == "-t") {
				if (argc[iterator] == "PIECE_MOVE")
					testsToExecute.emplace_back(Testing::TEST_TYPE::PIECE_MOVE);
				else if (argc[iterator] == "MOVE_GENERATE")
					testsToExecute.emplace_back(Testing::TEST_TYPE::MOVE_GENERATE);
			}
		}

		Testing::Automation_Tests newAutomationTest(testFileName);
		newAutomationTest.ExecuteTestCases(testsToExecute);
	}
	catch (PaleEngineException& exception) {
		if (exception.GetType() == 'e')
			PALE_TESTS_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
		else if (exception.GetType() == 'w')
			PALE_TESTS_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
	}

	std::cin.get();
	return 0;
}