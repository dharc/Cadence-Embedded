#include <cadence-embedded/cadence.h>
#include <cadence-embedded/file.h>
#include <cadence-embedded/core/type_traits.h>
#include <cadence-embedded/memory.h>

#ifdef QT
#include <QtGui>
#endif

using namespace cadence;

#ifdef QT
QApplication *qtapp=0;
int myargc = 0;
char *myargv[] = { "", "-qws" };
extern bool interactive;
#endif

void cadence_callback() {
	#ifdef QT
	if (interactive) {
		if (qtapp) qtapp->processEvents();
	} else {
		if (qtapp) qtapp->processEvents(QEventLoop::WaitForMoreEvents);
	}
	#endif
}

int main(int argc, char *argv[]) {
	#ifdef QT
	#ifdef ARM
	myargc = 2;
	qtapp = new QApplication(myargc,myargv);
	#else
	qtapp = new QApplication(myargc,0);
	#endif
	#endif

	cadence::initialise(argc, argv);

	core::root["notations"]["dasm"]["run"] = NEW LocalFile("linux.dasm");
	core::root["notations"]["dasm"]["run"] = NEW LocalFile("config.dasm");
	
	cadence::run(cadence_callback);
	cadence::finalise();

	#ifdef QT
	delete qtapp;
	#endif
}
