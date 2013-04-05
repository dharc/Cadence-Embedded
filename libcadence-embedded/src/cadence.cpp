/*
 * src/doste.cpp
 * 
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

#include <cadence-embedded/cadence.h>
#include <cadence-embedded/core/processor.h>
#include <cadence-embedded/object.h>
#include <cadence-embedded/agenthandler.h>
#include <cadence-embedded/notation.h>
#include <cadence-embedded/dstring.h>
#include <cadence-embedded/memory.h>
#include "agents/module.h"
#include "agents/softagent.h"
#include "notations/dasm.h"
#include <cadence-embedded/file.h>
#include <cadence-embedded/directory.h>
#include <stdlib.h>
#include <string.h>

#ifdef LINUX
#include <unistd.h>
#include <sys/time.h>
#include <dlfcn.h>
#include <fcntl.h>
#endif

#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#include <conio.h>
#endif

//#ifndef WIN32
//#include <errno.h>

//#include <unistd.h>
//#undef ERROR

using namespace cadence;
using namespace cadence::core;

//DObject static data
cadence::Map Object::s_map;
cadence::Map Object::s_reg;
cadence::List<Object*> Object::s_list;

OID dasm;

Cadence *Cadence::s_instance = 0;

//DUMMY Used by configure scripts.
extern "C" {
int cadence_check(int v) {
	return v*v;
}
}

class XAgent : public Agent {
	public:
	XAgent(const OID &obj) : Agent(obj) { registerEvents(); };
	~XAgent() {};
		
	BEGIN_EVENTS(Agent);
	EVENT(evt_cwd, (*this)("cwd"));
	EVENT(evt_softagent, (*this)("agents")(cadence::core::All));
	EVENT(evt_notation, (*this)("notations")(cadence::core::All));
	EVENT(evt_modules, (*this)("modules")(cadence::core::All));
	EVENT(evt_cout, (*this)("cout"));
	EVENT(evt_error, (*this)("error"));
	EVENT(evt_warning, (*this)("warning"));
	EVENT(evt_debug, (*this)("debug"));
	EVENT(evt_info, (*this)("info"));
	//Notation event
	END_EVENTS;
};

void Cadence::makePath(char *buffer, int path, const char *filename) {
	switch(path) {
		case PATH_SCRIPTS:	strcpy(buffer, getenv("CADENCE_SCRIPTS")); break;
		case PATH_MODULES:	strcpy(buffer, getenv("CADENCE_MODULES")); break;
		default:		strcpy(buffer, ".");
	}
	strcat(buffer, "/");
	strcat(buffer, filename);
}

OnEvent(XAgent, evt_cwd) {
	dstring s((OID)((*this)["cwd"]));
	char buffer[400];
	s.toString(buffer,400);
	chdir(buffer);
}
	
OnEvent(XAgent, evt_softagent) {
	//Make all softagents
	OID ag = (*this)["agents"];
	for (OID::iterator i=ag.begin(); i!=ag.end(); i++) {
		(Object*)(ag.get(*i));
		//std::cout << "Found agent\n";
	}
}
	
OnEvent(XAgent, evt_notation) {
	//Make all softagents
	OID ag = (*this)["notations"];
	for (OID::iterator i=ag.begin(); i!=ag.end(); i++) {
		(Notation*)(ag.get(*i));
		//std::cout << "Found notation\n";
	}
}

OnEvent(XAgent, evt_modules) {
	//Make all softagents
	OID ag = (*this)["modules"];
	for (OID::iterator i=ag.begin(); i!=ag.end(); i++) {
		if (*i != This)
		(Module*)(ag.get(*i));
		//std::cout << "Found module\n";
	}
}

OnEvent(XAgent, evt_cout) {
	//DEPENDENCY(*this, "cout2");
	//std::cout << (const char*)dstring(get("cout"));
}

OnEvent(XAgent, evt_error) {
	if (get("error") == Null) return;

	#ifdef LINUX
	std::cout << "\e[31;1m";
	#else
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	#endif

	std::cout << (const char*)dstring(get("error").get("message")) << "\n";;

	#ifdef LINUX
	std::cout << "\e[0m";
	#else
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	#endif

	std::cout.flush();
}

OnEvent(XAgent, evt_warning) {
	if (get("warning") == Null) return;

	#ifdef LINUX
	std::cout << "\e[33m";
	#else
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	#endif

	std::cout << (const char*)dstring(get("warning").get("message")) << "\n";;

	#ifdef LINUX
	std::cout << "\e[0m";
	#else
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	#endif

	std::cout.flush();
}

OnEvent(XAgent, evt_debug) {
	if (get("debug") == Null) return;

	#ifdef LINUX
	std::cout << "\e[34;1m";
	#else
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	#endif

	std::cout << (const char*)dstring(get("debug").get("message")) << "\n";;

	#ifdef LINUX
	std::cout << "\e[0m";
	#else
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	#endif

	std::cout.flush();
}

OnEvent(XAgent, evt_info) {
	if (get("info") == Null) return;
	std::cout << (const char*)dstring(get("info").get("message")) << "\n";;
	std::cout.flush();
}
	
IMPLEMENT_EVENTS(XAgent, Agent);

long long Cadence::getTicks() {
	#ifdef LINUX
	unsigned long long ticks;
	struct timeval now;
	gettimeofday(&now, NULL);
	ticks = ((unsigned long long)now.tv_sec) * (unsigned long long)1000000 + ((unsigned long long)now.tv_usec);
	return ticks;
	#endif

	#ifdef WIN32
	LARGE_INTEGER tks;
	QueryPerformanceCounter(&tks);
	return (((unsigned long long)tks.HighPart << 32) + (unsigned long long)tks.LowPart);
	#endif
}

void Cadence::initialise() {
	//int n = 1;
	//m_base = OID(1,0,0,0);
	//int ai = 0;
	//int arg_exec[20]; //NOTE: do not hard code.
	//int port = 9001;
	
	m_startticks = getTicks();

	cadence::core::initialise(m_base, m_n);
	cadence::LocalFile::initialise();
	
	new AgentHandler();
	root["running"] = true;
	char *buf = new char[1000];
	getcwd(buf,1000);
	root["cwd"] = dstring(buf);
	delete [] buf;
	root["agents"] = OID::create();
	OID notations = OID::create();
	root["notations"] = notations;
	root["workdir"] = OID(0,101,0,0);
	dasm = OID::create();
	dasm["type"] = OID("DASM");
	dasm["root"] = root;
	notations["dasm"] = dasm;
	Processor::processAll();

	new Directory();

	Object::registerType<SoftAgent>();
	Object::registerType<Notation>();
	Object::registerType<DASM>();
	Object::registerType<File>();
	Object::registerType<LocalFile>();
	Object::registerType<Module>();
	new XAgent(root);

	//if (ai > 0) {
		//DASM dasm(doste::dvm::root);
	//	for (int i=0; i<ai; i++) {
	//		dasm["execute"] = dstring(argv[arg_exec[i]]);
	//		Processor::processAll();
	//	}
	//}
	
	#ifdef win32
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
	#endif
}

Cadence::Cadence()
 : m_base(1,0,0,0),
   m_n(1),
   m_interactive(false),
   m_debug(0),
   m_settime(true) {
	m_includeix = 0;
	m_toinclude = new const char*[MAX_INCLUDES];
	s_instance = this;
}

Cadence::~Cadence() {
	//#ifndef WIN32
	//close(service_sock);
	//#else
	//closesocket(service_sock);
	//#endif

	Object::destroyAll();
	cadence::core::finalise();
	
	//if (interactive) {
		//endwin();
	//}
	
	#ifdef DEBUG
	DisplayLeaks();
	#endif
}

void Cadence::include(const char *inc) {
	if (m_includeix == MAX_INCLUDES) return;
	m_toinclude[m_includeix++] = inc;
}

void Cadence::run(void (*callback)()) {
	char *ibuf = NEW char[10000];
	int pos = 0;
	int count;
	OID res;

	initialise();
	
	#ifdef LINUX
	fcntl(0, F_SETFL, O_NONBLOCK);
	#endif
	if (m_interactive) {
		std::cout << "dasm> ";
		std::cout.flush();
	}
	
	dasm.set("root", root);

	//For all things to be included...
	for (int i=0; i<m_includeix; ++i) {
		core::root["notations"]["dasm"]["run"] = NEW LocalFile(m_toinclude[i]);
	}

	while (root["running"] == true) {
		
		#ifdef LINUX
		m_ttime = ((double) getTicks() - (double) m_startticks) / 1000000.0;
		#endif
		#ifdef WIN32
		LARGE_INTEGER fq;
		QueryPerformanceFrequency(&fq);
		m_ttime = ((double)getTicks() - (double)m_startticks) / (double)(((unsigned long long)fq.HighPart << 32) + (unsigned long long)fq.LowPart);
		#endif
		
		//Put time into graph if requested.
		if (m_settime) root["time"] = m_ttime;

		m_ttime_draw += (m_ttime - m_ttime_last);
		m_dtime = (m_ttime - m_ttime_last);
		m_ttime_last = m_ttime;

		
		if (m_interactive) {

			//Read a console character from stdin.
			#ifdef LINUX
			count = read(0, &ibuf[pos], 10000-pos);
			#else
			if(_kbhit()) {
				ReadConsoleA(GetStdHandle(STD_INPUT_HANDLE), &ibuf[pos], 10000-pos, (unsigned long *)&count, 0);
			} else {
				count = 0;
			}
			#endif

			//If a character was read
			if (count > 0) {
				pos += count;
				ibuf[pos] = 0;
					
				//If the character was a carrage return
				if (ibuf[pos-1] == '\n') {
					//We have reached the end of an input statement
					pos = 0;

					//Execute the entered statement
					((DASM*)dasm)->execute(ibuf);
					DMsg msg(DMsg::INFO);
					res = dasm.get("result");

					//Check the type of the result and display it
					if (!res.isReserved() && res.get(Size) != Null && res.get(0).isChar()) {
						dstring(res).toString(ibuf,1000);
						std::cout << "  \"" << ibuf << "\"\n";
					} else {
						res.toString(ibuf, 1000);
						std::cout << "  " << ibuf << "\n";
					}

					//Display a new prompt on a new line.
					std::cout << "dasm> ";
					std::cout.flush();
				}
			}
		}

		Module::updateAll(m_dtime);
		if (callback) callback();
	}

	delete [] ibuf;
}
