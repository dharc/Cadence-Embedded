#include <cadence-embedded/cadence.h>
#include <cadence-embedded/file.h>
#include <cadence-embedded/core/type_traits.h>
#include <cadence-embedded/memory.h>

using namespace cadence;

int main(int argc, char *argv[]) {
	cadence::initialise(argc, argv);

	core::root["notations"]["dasm"]["run"] = NEW LocalFile("linux.dasm");
	core::root["notations"]["dasm"]["run"] = NEW LocalFile("config.dasm");
	
	cadence::run();
	cadence::finalise();
}
