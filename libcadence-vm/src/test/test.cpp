/*
 * src/test/test.cpp
 * 
 * Test harness for Cadence
 * Copyright (C) 2013 Nicolas Pope
 *
 * Author(s) : Nicolas Pope
 * Date      : 2013
 * Home Page : http://www.dharc.co.uk
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <cadence-vm/test/test.h>
#include <cadence-vm/cadence.h>
#include <iostream>
#include <string.h>

using namespace cadence;
using namespace std;

cadence::Test::Test(int argc, char *argv[]) {
	m_pretty = false;
	for (int i=0; i<argc; i++) {
		if (strcmp(argv[i],"--pretty") == 0) {
			m_pretty = true;
		}
	}

	if (m_pretty) {
		//std::cout << "\n====================================\n";
		std::cout << "==== " << argv[0] << " ====\n";
		//std::cout << "====================================\n\n";
	} else {
		std::cout << "<testapp name=\"" << argv[0] << "\">\n";
	}

	m_testcount = 0;
	m_testfailcount = 0;
	m_checkfailcount = 0;
	m_checkcount = 0;

	m_actualcout = cout.rdbuf();
}

cadence::Test::~Test() {
	if (m_pretty) {
		std::cout << "[Tests: " << m_testcount << ", failed " << m_testfailcount;
		std::cout << ", Checks: " << m_checkcount << ", failed " << m_checkfailcount << "]\n";
	} else {
		std::cout << "\t<testcount>" << m_testcount << "</testcount>\n";
		std::cout << "\t<failedtests>" << m_testfailcount << "</failedtests>\n";
		std::cout << "\t<checkcount>" << m_checkcount << "</checkcount>\n";
		std::cout << "\t<failedchecks>" << m_checkfailcount << "</failedchecks>\n";
		std::cout << "</testapp>\n";
	}
}

void cadence::Test::checkPassed() {
	m_checkcount++;
}

void cadence::Test::captureCOUT() {
	m_actualcout = cout.rdbuf();
	cout.flush();
	cout.rdbuf(m_cout.rdbuf());
}

void cadence::Test::uncaptureCOUT() {
	cout.rdbuf(m_actualcout);
}

bool cadence::Test::checkCOUT(const char *str) {
	bool result = (m_cout.str() == str);
	m_cout.str("");
	return result;
}

void cadence::Test::checkFailed(int line, const char *function, const char *file) {
	m_failed = true;
	m_checkfailcount++;

	uncaptureCOUT();

	if (m_pretty) {
		std::cout << (&function[5]) << ":" << line;

		#ifdef LINUX
		std::cout << "\e[31;1m";
		#else
		HANDLE  hConsole;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		#endif

		std::cout << " CHECK FAILED\n";

		#ifdef LINUX
		std::cout << "\e[0m";
		#else
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		#endif

		std::cout.flush();
	} else {
		std::cout << "\t<check name=\"" << (&function[5]) << "\" line=\"" << line << "\">failed</check>\n";
	}

	captureCOUT();
}

void cadence::Test::testDone(const char *function) {
	uncaptureCOUT();

	if (m_pretty) {
		std::cout << (&function[5]);
	}

	if (m_failed) {
		if (m_pretty) {
			#ifdef LINUX
			std::cout << "\e[31;1m";
			#else
			HANDLE  hConsole;
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
			#endif

			std::cout << " FAILED\n";

			#ifdef LINUX
			std::cout << "\e[0m";
			#else
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			#endif
		} else {
			std::cout << "\t<test name=\"" << (&function[5]) << "\">failed</test>\n";
		}
	} else {
		if (m_pretty) {
			#ifdef LINUX
			std::cout << "\e[32;1m";
			#else
			HANDLE  hConsole;
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
			#endif

			std::cout << " PASSED\n";

			#ifdef LINUX
			std::cout << "\e[0m";
			#else
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			#endif
		} else {
			std::cout << "\t<test name=\"" << (&function[5]) << "\">passed</test>\n";
		}
	}

	std::cout.flush();
	captureCOUT();
}

bool cadence::Test::run(testtype_t type, void (*func)(Test &)) {
	m_failed = false;
	m_curcheckcount = 0;
	m_testcount++;

	captureCOUT();
	m_cout.str("");

	long long tickdiff = 0;

	if ((type == TEST_SYSTEM) || type == TEST_PERFORMANCE) {
		Cadence vm;
		vm.initialise();
		long long startticks = vm.getTicks();
		func(*this);
		tickdiff = vm.getTicks() - startticks;
	} else {
		func(*this);
	}

	uncaptureCOUT();

	if (type == TEST_PERFORMANCE) {
		std::cout << " in " << ((double)tickdiff / 1000000.0) << "s --\n";		
	}

	if (m_failed) {
		m_testfailcount++;
		return false;
	} else {
		return true;
	}
}

