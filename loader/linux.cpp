#include <cadence-embedded/cadence.h>
#include <cadence-embedded/file.h>
#include <cadence-embedded/core/type_traits.h>
#include <cadence-embedded/memory.h>

#ifdef QT
#include <QtGui>
#endif

using namespace cadence;

Cadence vm;

#ifdef QT
QApplication *qtapp=0;
int myargc = 0;
char *myargv[] = { "", "-qws" };
#endif

void cadence_callback() {
	#ifdef QT
	if (vm.isInteractive()) {
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

	vm.include("linux.dasm");
	vm.include("config.dasm");
	
	//Process command line arguments here.
	for (int i=1; i<argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'b':	vm.setBaseOID(core::OID(argv[++i]));
					break;
			case 'n':	vm.setProcessorCount(atoi(argv[++i]));
					break;
			case 'i':	vm.setInteractive(true);
					break;
			case 't':	vm.setSetTime(false);
					break;
			case 'v':	vm.setDebug(atoi(argv[++i]));
					break;
			case 'e':	vm.include(argv[++i]);
					break;

			default:	break;
			}
		}
	}	

	vm.run(cadence_callback);

	//Explicit cleanup before Qt cleanup.
	vm.finalise();

	#ifdef QT
	delete qtapp;
	#endif
}
