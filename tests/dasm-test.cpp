#include <cadence-vm/core/event.h>
#include <cadence-vm/test/test.h>
#include <cadence-vm/cadence.h>
#include <string.h>

using namespace cadence;
using namespace cadence::core;

void test_simplecorrect(Test &test) {
	//simple assignment
	_DASM2(this a = 5;);
	CHECK((int)root.get("a") == 5);

	//simple brackets
	_DASM2(this b = (6););
	CHECK((int)root.get("b") == 6);

	//Check double brackets
	_DASM2(this c = ((7)););
	CHECK((int)root.get("c") == 7);

	_DASM2(this(y)=4);
	CHECK((int)root.get("y") == 4);

	DONE
}

void test_whitespace(Test &test) {
	_DASM2(  this     b
		= (  5););
	CHECK((int)root.get("b") == 5);

	_DASM2(this b=7);
	CHECK((int)root.get("b") == 7);

	_DASM2(this b=(8););
	CHECK((int)root.get("b") == 8);

	DONE
}

void test_structures(Test &test) {
	_DASM2(this x = (new x=9););
	CHECK((int)(root.get("x").get("x")) == 9);

	_DASM2(this f is { this x x };);
	CHECK((int)root.get("f") == 9);

	_DASM2(this g is { this x(x) };);
	CHECK((int)root.get("g") == 9);

	_DASM2(this x y = (new z=77); this h is { this x(y)(z) };);
	CHECK((int)root.get("h") == 77);
	DONE
}

void test_is(Test &test) {
	_DASM2(this a = 6; this b is { this a };);
	CHECK((int)root.get("b") == 6);

	_DASM2(this c is { this (a) };);
	CHECK((int)root.get("c") == 6);

	_DASM2(this d is { this ((a)) };);
	CHECK((int)root.get("d") == 6);

	DONE
}

void test_if(Test &test) {
	_DASM2(
		this iftest1 = 66;
		this iftest3 is {
			if (this iftest1 == 66) {
				50
			} else {
				60
			}
		};
	);
	CHECK((int)root.get("iftest3") == 50);
	root.set("iftest1", 54);
	CHECK((int)root.get("iftest3") == 60);

	_DASM2(
		this iftest4 = 66;
		this iftest5 is {
			if (this iftest4 == 66)	50
			else 60
		};
	);
	CHECK((int)root.get("iftest5") == 50);

	_DASM2(
		this iftest6 = 66;
		this iftest7 is {
			if (this iftest6 == 66)	50
		};
	);
	CHECK((int)root.get("iftest7") == 50);
	root.set("iftest6", 54);
	CHECK(root.get("iftest7") == Null);
	DONE
}

void test_select(Test &test) {

}

void test_brokensimply(Test &test) {
	root.get("error").set("number",0);
	_DASM2(this.);
	CHECK((int)(root.get("error").get("number")) == 1);

	root.get("error").set("number",0);
	_DASM2(this.a is);
	CHECK((int)(root.get("error").get("number")) == 1);

	root.get("error").set("number",0);
	_DASM2(this.a is { b);
	CHECK((int)(root.get("error").get("number")) == 1);
	DONE
}

void test_context(Test &test) {
	root.get("error").set("number",0);
	_DASM2(@hh g);
	CHECK((int)(root.get("error").get("number")) == 1);

	DONE
}

void test_generics(Test &test) {

}

void test_numeric(Test &test) {

}

int main(int argc, char *argv[]) {
	Test test(argc,argv);

	//Cadence vm;
	//vm.initialise();
	test.run(Test::TEST_SYSTEM, test_simplecorrect);
	test.run(Test::TEST_SYSTEM, test_whitespace);
	test.run(Test::TEST_SYSTEM, test_is);
	test.run(Test::TEST_SYSTEM, test_structures);
	test.run(Test::TEST_SYSTEM, test_if);
	test.run(Test::TEST_SYSTEM, test_brokensimply);
	test.run(Test::TEST_SYSTEM, test_context);
	return test.status();
}

