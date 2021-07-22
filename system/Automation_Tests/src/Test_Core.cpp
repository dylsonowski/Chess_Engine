#include "palepch.h"
#include "Automation_Tests.h"

int main(int argv, char** argc) {
	LOG_SYSTEM_INIT;
	Automation_Tests newAutomationTest("Test_File1(8x8)");
	newAutomationTest.ProcessInputFile();

	std::cin.get();
	return 0;
}