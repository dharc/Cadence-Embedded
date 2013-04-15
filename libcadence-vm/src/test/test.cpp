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
#include <iostream>
#include <string.h>

using namespace cadence;

cadence::Test::Test(int argc, char *argv[]) {
	m_pretty = false;
	for (int i=0; i<argc; i++) {
		if (strcmp(argv[i],"--pretty") == 0) {
			m_pretty = true;
		}
	}

	if (m_pretty) {
		std::cout << "\n====================================\n";
		std::cout << "  -- " << argv[0] << " --\n";
		std::cout << "====================================\n\n";
	} else {
		std::cout << "<testapp name=\"" << argv[0] << "\">\n";
	}

	m_testcount = 0;
	m_testfailcount = 0;
	m_checkfailcount = 0;
	m_checkcount = 0;
}

cadence::Test::~Test() {
	if (m_pretty) {
		std::cout << "Finished.\n";
		std::cout << "    Tests: " << m_testcount << ", failed " << m_testfailcount << "\n";
		std::cout << "    Checks: " << m_checkcount << ", failed " << m_checkfailcount << "\n";
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

void cadence::Test::checkFailed(int line, const char *function, const char *file) {
	m_failed = true;
	m_checkfailcount++;

	if (m_pretty) {
		std::cout << function << ":" << line;

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
		std::cout << "\t<check name=\"" << function << "\" line=\"" << line << "\">failed</check>\n";
	}
}

void cadence::Test::testDone(const char *function) {
	if (m_pretty) {
		std::cout << function;
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
			std::cout << "\t<test name=\"" << function << "\">failed</test>\n";
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
			std::cout << "\t<test name=\"" << function << "\">passed</test>\n";
		}
	}

	std::cout.flush();
}

bool cadence::Test::run(testtype_t type, void (*func)(Test &)) {
	m_failed = false;
	m_curcheckcount = 0;
	m_testcount++;
	func(*this);
	if (m_failed) {
		m_testfailcount++;
		return false;
	} else {
		return true;
	}
}

