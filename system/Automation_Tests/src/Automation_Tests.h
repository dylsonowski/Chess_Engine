#pragma once
#include <Pale.h>

typedef void(*t_voidFunctionPointer)(void);

enum class TEST_TYPE {
	NONE = 0,
	PIECE_MOVE,
	MOVE_GENERATE
};

struct Test_Case {
	TEST_TYPE m_type;
	unsigned int m_boardDimentionY, m_boardDimentionX;
	t_voidFunctionPointer m_functionPointer;
	std::vector<std::string> m_arguments;
	std::string m_expecterResult;
};

class Automation_Tests {

public:
	Automation_Tests() = delete;
	Automation_Tests(std::string fileName);
	~Automation_Tests() = default;

	void ProcessInputFile();

private:
	unsigned int _numberOfTests;
	std::string _inputFilePath;
	std::vector<std::pair<Pale::Chess_Logic::Board_Representation<std::shared_ptr<Pale::Chess_Logic::Pieces>>, std::vector<Test_Case>>> _testsList;
};

