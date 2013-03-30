/*
 * src/dvm/doste.cpp
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

#include <cadence-embedded/core/core.h>
#include <cadence-embedded/core/processor.h>
#include <cadence-embedded/core/event.h>
#include <cadence-embedded/dll.h>
#include <cadence-embedded/map.h>
#include <cadence-embedded/memory.h>
#include <cadence-embedded/core/buffer.h>
#include "../handlers/local.h"
#include "../handlers/number.h"

cadence::core::OID DVMIMPORT cadence::core::root;
unsigned int DVMIMPORT cadence::core::debug = 0;
cadence::core::Processor *proc;
bool running = true;

#include "names.h"
extern cadence::core::Names *names;

void cadence::core::initialise(const OID &base, int n) {
	//The name to OID mapping
        names = NEW Names;

	//Our local OID address space
	cadence::core::OID rbase(base.a(),base.b(),0,0);
	cadence::core::OID::local(rbase);

	//Initialise event cache and processor queues
	Event::initialise();
	Processor::init(n);	
	new cadence::core::Processor();

	//Create the key internal handlers
	new handlers::VStore();
	new handlers::Number();

	//Make the root OID
	root = OID::create();

	//Mark root as in use and not to be garbage collected.
	Event *evt = NEW Event(Event::ADD_REF, root);
	evt->send(Event::FLAG_FREE);
	
	//Now make the math functions
	root["math"] = OID::create();
	root["math"]["tan"] = OID(0,50,0,1);
	root["math"]["atan"] = OID(0,50,0,2);
	root["math"]["atan2"] = OID(0,50,0,3);
	root["math"]["sqrt"] = OID(0,50,0,4);
	root["math"]["sin"] = OID(0,50,0,5);
	root["math"]["cos"] = OID(0,50,0,6);
	root["math"]["_random"] = OID(0,50,0,7);
}

void cadence::core::finalise() {
	running = false;

	//Clean up memory in the various sub systems
	Processor::final();
	Event::finalise();
	Buffer::finalise();
	
	delete names;
}
