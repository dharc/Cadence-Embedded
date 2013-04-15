/*
 * include/cadence-embedded/test/test.h
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

#ifndef _CADENCE_TEST_TEST_H_
#define _CADENCE_TEST_TEST_H_

/* Preprocessor macros for Cadence test assertions */
//#define CHECK_EQUAL(A,B) if (A != B) { test.checkFailed(__LINE__.__func__,__FILE__); } else { test.checkPassed(); }
#define CHECK(A) if (!(A)) { test.checkFailed(__LINE__,__func__,"__FILE__"); } else { test.checkPassed(); }
#define DONE test.testDone(__func__);

namespace cadence {
	class Test {
		public:

		enum testtype_t { TEST_UNIT, TEST_SYSTEM, TEST_PERFORMANCE, TEST_SECURITY, TEST_OTHER };

		Test(int argc, char *argv[]);
		~Test();

		bool run(testtype_t, void (*)(Test&));

		void checkFailed(int line, const char *function, const char *file);
		void checkPassed();
		void testDone(const char *function);

		private:
		bool m_intest;
		bool m_failed;
		int m_checkcount;
		int m_testcount;
		int m_curcheckcount;
		int m_checkfailcount;
		int m_testfailcount;
		bool m_pretty;
	};
};

#endif /* _CADENCE_TEST_TEST_H_ */

