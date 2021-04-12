#include <Pale.h>

int main(int argc, char** argv) {

	LOG_SYSTEM_INIT;
	PALE_ENGINE_ERROR("Initialize core logger!");
	PALE_INFO("Initialize client logger!");
	Pale::Application app;
	app.Run();
	return 0;
}