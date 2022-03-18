#pragma once

namespace Testing {
	enum class TEST_TYPE {
		NONE = 0,
		PIECE_MOVE,
		MOVE_GENERATE
	};

	struct Test_Case {
		TEST_TYPE m_type;
		unsigned int m_boardDimentionY, m_boardDimentionX;
		std::vector<std::string> m_arguments;
		std::string m_expecterResult;
	};

	template<typename T>
	static void AssertGivenValue(T givenValue, T expectedValue, std::string errorMessage);

	class Automation_Tests {

	public:
		Automation_Tests() = delete;
		Automation_Tests(std::string fileName);
		~Automation_Tests() = default;

		void ProcessInputFile();
		void ExecuteTestCases(std::vector<TEST_TYPE> testsToExecute) const;
		void PrintTestReport(unsigned int passedTests, unsigned int failedTests, unsigned int skippedTests) const;

	private:
		unsigned int _numberOfTests;
		std::string _inputFilePath;
		std::vector<std::pair<Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>>, std::vector<Test_Case>>> _testsList;
	};

	template<typename T>
	void AssertGivenValue(T givenValue, T expectedValue, std::string errorMessage) {
		try {
			if (givenValue == expectedValue)
				PALE_ASSERTION_INFO("Assertion! -> Both values are the same.");
			else
				throw PaleEngineException("Exception happened!", 'e', "Automation_Tests.h", 43, "AssertGivenValue", errorMessage);
		}
		catch (PaleEngineException& exception) {
			if (exception.GetType() == 'e')
				PALE_ASSERTION_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
			else if (exception.GetType() == 'w')
				PALE_ASSERTION_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
		}
	}
}