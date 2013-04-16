#include <cadence-vm/core/event.h>
#include <cadence-vm/test/test.h>
#include <cadence-vm/cadence.h>
#include <string.h>

using namespace cadence;
using namespace cadence::core;

void test_construct(Test &test) {
	Event evt(Event::SET, OID(1,1,1,1));
	CHECK(evt.dest() == OID(1,1,1,1));
	CHECK(evt.type() == Event::SET);
	DONE
}

void test_params(Test &test) {
	Event evt(Event::SET, OID(1,1,1,1));
	evt.param<0>(55);
	evt.param<1>(66);
	evt.param<2>(77);
	evt.param<3>(88);
	CHECK((int)evt.param<0>() == 55);
	CHECK((int)evt.param<1>() == 66);
	CHECK((int)evt.param<2>() == 77);
	CHECK((int)evt.param<3>() == 88);
	DONE
}

void test_excessparams(Test &test) {
	Event evt(Event::SET, OID(1,1,1,1));
	evt.param<0>(55);
	evt.param<1>(66);
	evt.param<2>(77);
	evt.param<3>(88);
	evt.param<4>(99);
	evt.param<5>(100);
	evt.param<6>(110);
	evt.param<7>(120);
	CHECK((int)evt.param<3>() == 88);
	CHECK(evt.param<6>() == Null);
	DONE
}

void test_allocate(Test &test) {
	Event **evtarray = new Event*[10000];
	for (int i=0; i<10000; i++) {
		evtarray[i] = new Event(0,i);
	}
	CHECK((int)(evtarray[6700]->dest()) == 6700);
	for (int i=0; i<10000; i++) {
		delete evtarray[i];
	}
	delete [] evtarray;
	DONE
}

void test_excessallocate(Test &test) {
	Event **evtarray = new Event*[100001];
	for (int i=0; i<100001; i++) {
		evtarray[i] = new Event(0,i);
	}
	CHECK(evtarray[100001] != 0);
	for (int i=0; i<100001; i++) {
		delete evtarray[i];
	}
	delete [] evtarray;
	DONE
}

int main(int argc, char *argv[]) {
	Test test(argc,argv);

	Event::initialise();

	test.run(Test::TEST_UNIT, test_construct);
	test.run(Test::TEST_UNIT, test_params);
	test.run(Test::TEST_UNIT, test_excessparams);
	test.run(Test::TEST_PERFORMANCE, test_allocate);
	test.run(Test::TEST_UNIT, test_excessallocate);

	Event::finalise();

	//Cadence vm;
	//vm.initialise();
	//test.run(Test::TEST_UNIT, test_construct);
	return test.status();
}
