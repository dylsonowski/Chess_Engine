#include "palepch.h"

int main(int argc, char** argv) {
	bool consoleUIVersion = true;
	unsigned short int numberOfAIInstances = 0;
	if (argc > 1) {
		for (int iterator = 0; iterator < argc; iterator++) {
			if (argv[iterator] == "-g")
				consoleUIVersion = false;

			if (argv[iterator] == "-ai")
				numberOfAIInstances = std::stoi(argv[iterator + 1]);
		}
	}

	Pale::Application app(consoleUIVersion, numberOfAIInstances);
	app.Run();
	return 0;
}