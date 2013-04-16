#include <cadence-vm/core/oid.h>
#include <cadence-vm/test/test.h>
#include <cadence-vm/cadence.h>
#include <string.h>

using namespace cadence;
using namespace cadence::core;

void test_double(Test &test) {
	OID val(4.555673);
	CHECK(val.isDouble());
	CHECK((double)val == 4.555673);
	CHECK((int)val == 4);
	DONE	
}

void test_comparators(Test &test) {
	OID vala(55);
	OID valb(55);
	CHECK(vala == valb);
	valb = (56);
	CHECK(vala != valb);
	CHECK(!(vala == valb));
	CHECK(valb > vala);
	CHECK(vala < valb);
	DONE
}

void test_oidarithmetic(Test &test) {
	OID vala(50);
	vala++;
	CHECK((int)vala == 51);
	OID valb(0,0,0,55);
	valb = valb + vala;
	CHECK((int)valb == 106);
	DONE
}

void test_string(Test &test) {
	OID val(56,34,23,12);
	char buf[100];
	val.toString(buf,100);
	CHECK(strcmp(buf,"<56:34:23:12>") == 0);
	val = 57788;
	val.toString(buf,100);
	CHECK(strcmp(buf,"57788") == 0);
	val = true;
	val.toString(buf,100);
	CHECK(strcmp(buf,"true") == 0);
	DONE
}

void test_getset(Test &test) {
	True.set(0,55);
	CHECK(int(True.get(0)) == 55);
	True.set(0,False);
	CHECK(True.get(0) == False);
	DONE
}

void test_iterator(Test &test) {
	DONE
}

int main(int argc, char *argv[]) {
	Test test(argc,argv);
	test.run(Test::TEST_UNIT, test_double);
	test.run(Test::TEST_UNIT, test_comparators);
	test.run(Test::TEST_UNIT, test_oidarithmetic);
	test.run(Test::TEST_UNIT, test_string);

	Cadence vm;
	vm.initialise();
	test.run(Test::TEST_SYSTEM, test_getset);
	test.run(Test::TEST_SYSTEM, test_iterator);
}
