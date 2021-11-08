#include "palepch.h"
#include "Command_Line_UI.h"

void Pale::UI::CommandLine::Command_Line_UI::RenderLogo() const {
	std::cout << "															\n";
	std::cout << "		 .d8888b.  888										\n";
	std::cout << "		d88P  Y88b 888										\n";
	std::cout << "		888    888 888										\n";
	std::cout << "		888        88888b.   .d88b.  .d8888b  .d8888b		\n";
	std::cout << "		888        888 \"88b d8P  Y8b 88K      88K			\n";
	std::cout << "		888    888 888  888 88888888 \"Y8888b. \"Y8888b.	\n";
	std::cout << "		Y88b  d88P 888  888 Y8b.          X88      X88		\n";
	std::cout << "		 \"Y8888P\"  888  888  \"Y8888   88888P\'  88888P\'	\n";
	std::cout << "															\n";
}

void Pale::UI::CommandLine::Command_Line_UI::RenderBoard() const {
	std::cout << "Captured pieces (WHITE): ";
	for (const auto& iterator : Chess_Logic::s_deathList) {
		switch (iterator) {
		case 7: std::cout << "K "; break;
		case 5: std::cout << "Q "; break;
		case 4: std::cout << "B "; break;
		case 3: std::cout << "N "; break;
		case 2: std::cout << "R "; break;
		case 1: std::cout << "P "; break;
		}
	}

	std::cout << "\n\n";
	std::cout << "Captured pieces (BLACK): ";
	for (const auto& iterator : Chess_Logic::s_deathList) {
		switch (iterator) {
		case -7: std::cout << "K "; break;
		case -5: std::cout << "Q "; break;
		case -4: std::cout << "B "; break;
		case -3: std::cout << "N "; break;
		case -2: std::cout << "R "; break;
		case -1: std::cout << "P "; break;
		}
	}

	std::cout << "\n\n" << _boardRepresentation << "\n\n";
}

void Pale::UI::CommandLine::Command_Line_UI::RenderMainMenu() const {
	RenderLogo();
	std::cout << "Welcome in Chess AI program!\n\n How deep should the AI look for moves?\n Warning: values above 3 will be very slow. [n]? --> ";
}

void Pale::UI::CommandLine::Command_Line_UI::RenderGameMenu(unsigned int turnCounter) const {
	try {
		switch (_numberOfAIInstances) {
		case 0:
			if (turnCounter % 2 == 0)
				std::cout << "Turn " << turnCounter << "(BLACK)\n";
			else
				std::cout << "Turn " << turnCounter << "(WHITE)\n";

			std::cout << "Commands: (N)ew game\t (M)ove\t (Q)uit\n\n";
			std::cout << "To see move command help, type '?'. Insert command here --> ";
			break;

		case 1:
			if (turnCounter % 2 == 0)
				std::cout << "Turn " << turnCounter << "(BLACK)\n";
			else {
				std::cout << "Turn " << turnCounter << "(WHITE)\n";

				std::cout << "Commands: (N)ew game\t (M)ove\t (Q)uit\n\n";
				std::cout << "To see move command help, type '?'. Insert command here --> ";
			}
			break;

		case 2:
			if (turnCounter % 2 == 0)
				std::cout << "Turn " << turnCounter << "(BLACK)\n";
			else
				std::cout << "Turn " << turnCounter << "(WHITE)\n";

			if (FULLY_AUTOMATED)
				std::cout << "Continue?: ";			
			break;

		default: throw PaleEngineException("Exception happened!", 'e', "Command_Line_UI.cpp", 20, "DrawGameMenu", UI_INVALID_AI_NUMBER); break;
		}
	}
	catch (PaleEngineException& exception) {
		if (exception.GetType() == 'e')
			PALE_ENGINE_ERROR("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo())
		else if (exception.GetType() == 'w')
			PALE_ENGINE_WARN("{0} [{1}]: {2}", exception.GetFile(), exception.GetLine(), exception.GetInfo());
	}
}
