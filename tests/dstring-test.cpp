#include <cadence-vm/dstring.h>
#include <cadence-vm/test/test.h>
#include <cadence-vm/cadence.h>
#include <string.h>

using namespace cadence;
using namespace cadence::core;

void test_construct(Test &test) {
	dstring str("hello world");
	OID obj = str;
	CHECK((char)obj.get(0) == 'h');
	CHECK((int)obj.get(Size) == 11);
	DONE	
}

void test_tostring_normal(Test &test) {
	dstring str("the matrix has you");
	char buf[100];
	str.toString(buf,100);
	CHECK(strcmp(buf,"the matrix has you") == 0);
	DONE
}

void test_tostring_overflow(Test &test) {
	dstring str("war of the worlds");
	char buf[100];
	str.toString(buf,5);
	CHECK(strcmp(buf,"war o") == 0);
	DONE
}

void test_tostring_massive(Test &test) {
	dstring str("I have tried to figure out the screen resolution thing. I found that it seems the height and width is\
not propotional to the screen resolution and the default resolution now  is 1600*900, and all the\
settings work fine with high resolutions. For lower resolution, it does not look good with smaller\
size windows, because a lot of information will not be displayed.  I am not sure if I should change\
it or leave it as it is now...");
	char buf[2000];
	str.toString(buf,2000);
	CHECK(strcmp(buf,"I have tried to figure out the screen resolution thing. I found that it seems the height and width is\
not propotional to the screen resolution and the default resolution now  is 1600*900, and all the\
settings work fine with high resolutions. For lower resolution, it does not look good with smaller\
size windows, because a lot of information will not be displayed.  I am not sure if I should change\
it or leave it as it is now...") == 0);
	DONE
}

void test_tostring_nest(Test &test) {
	dstring str("nest _ hohoho");
	OID strobj = str;
	strobj.set(5, 66666);
	char buf[100];
	str.toString(buf,100);
	CHECK(strcmp(buf,"nest 66666 hohoho") == 0);
	DONE
}

void test_addstr(Test &test) {
	dstring str("part1");
	dstring str2 = str + " part2";
	char buf[100];
	str2.toString(buf,100);
	CHECK(strcmp(buf,"part1 part2") == 0);
	DONE
}

void test_addint(Test &test) {
	dstring str("part3");
	dstring str2 = str + 555;
	char buf[100];
	str2.toString(buf,100);
	CHECK(strcmp(buf,"part3555") == 0);
	DONE
}

int main(int argc, char *argv[]) {
	Test test(argc,argv);

	//Cadence vm;
	//vm.initialise();
	test.run(Test::TEST_SYSTEM, test_construct);
	test.run(Test::TEST_SYSTEM, test_tostring_normal);
	test.run(Test::TEST_SYSTEM, test_tostring_overflow);
	test.run(Test::TEST_SYSTEM, test_tostring_massive);
	test.run(Test::TEST_SYSTEM, test_tostring_nest);
	test.run(Test::TEST_SYSTEM, test_addstr);
	test.run(Test::TEST_SYSTEM, test_addint);
	return test.status();
}

