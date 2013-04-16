#include <cadence-vm/cadence.h>
#include <cadence-vm/file.h>
#include <cadence-vm/core/type_traits.h>
#include <cadence-vm/memory.h>

#ifdef QT
#include <QtGui>
#endif

using namespace cadence;

Cadence vm;

#ifdef QT
QApplication *qtapp=0;
int myargc = 0;
char *myargv[] = { "", "-qws" };
double qtlastpoll = 0.0;

//40 fps
static const double QT_POLL_INTERVAL = 0.025;

#endif

void cadence_callback() {
	#ifdef QT
	//if (vm.isInteractive()) {

	//Limit Qt to 40fps.
	if (vm.getTime() - qtlastpoll > QT_POLL_INTERVAL)
	{
		if (qtapp) qtapp->processEvents();
		qtlastpoll = vm.getTime();
	}


	//} else {
	//	if (qtapp) qtapp->processEvents(QEventLoop::WaitForMoreEvents);
	//}
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
