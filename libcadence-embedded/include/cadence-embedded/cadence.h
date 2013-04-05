/*
 * includes/doste/doste.h
 * 
 * Global common doste header.
 * Copyright (C) 2008 Nicolas Pope
 *
 * Author(s) : Nicolas Pope
 * Date      : 2008
 * Home Page : http://www.doste.co.uk
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

#ifndef _CADENCE_H_
#define _CADENCE_H_

#include <cadence-embedded/core/core.h>
#include <cadence-embedded/agent.h>
#include <cadence-embedded/notation.h>

namespace cadence {
	/**
	 * The master application object that must be created before anything
	 * in the Cadence library is used. After construction the run method
	 * is used to start up the virtual machine, but the VM should be
	 * configured first.
	 * @author Nicolas Pope
	 */
	class Cadence {
		public:
		Cadence();
		~Cadence();

		/**
		 * Initialises the VM and begins the main run loop. This
		 * function will then block until the VM is signalled to
		 * terminate.
		 * @param cb Callback function, called each pass through the loop.
		 */
		void run(void (*)());

		/**
		 * Enable command line interaction mode. This does increase
		 * CPU usage to maximum so is only recommended as a debug
		 * mechanism.
		 * @param enabled Defaults to false.
		 */
		void setInteractive(bool b) { m_interactive = b; }

		/**
		 * Change the number of threads to be created by the virtual
		 * machine for event processing. Typically this should be one
		 * per core available.
		 * @param n Number of threads. Defaults to 1.
		 */
		void setProcessorCount(int n) { m_n = n; }

		/**
		 * Each VM has a base Object IDentifier that specifies an OID
		 * address space that it is responsible for and which it can
		 * use to allocate new OIDs.
		 * @param base Defaults to 1:0:0:0
		 */
		void setBaseOID(const core::OID &base) { m_base = base; }

		/**
		 * Change the debug output level. Only works when the VM is
		 * compiled in DEBUG mode. Increasing the level number will
		 * increase the amount of debug information displayed.
		 * @param level Debug level between 0 and 5. Default is 0.
		 */
		void setDebug(int debug) { m_debug = debug; }

		void setSetTime(bool b) { m_settime = b; }

		bool isInteractive() { return m_interactive; }

		double getTime() { return m_ttime; }

		void include(const char*);

		/**
		 * Used to generate a full file path using a particular search
		 * order and environment variables. Checks local directory first
		 * and then global paths.
		 * @param buffer Resulting path is put into this buffer.
		 * @param path Select type of file to look for.
		 * @param filename The short path to be searched for.
		 */
		void makePath(char *buffer, int path, const char *filename);

		static const int PATH_SCRIPTS = 1;
		static const int PATH_MODULES = 2;

		static Cadence *instance() { return s_instance; }

		void initialise();
		void finalise();

		private:
		long long getTicks();

		core::OID m_base;
		int m_debug;
		bool m_interactive;
		bool m_settime;
		int m_n;
		long long m_startticks;
		double m_ttime;
		double m_ttime_last;
		double m_dtime;
		double m_ttime_draw;
		const char **m_toinclude;
		int m_includeix;
		static Cadence *s_instance;

		static const unsigned int MAX_INCLUDES = 20;
	};	
};

#endif
