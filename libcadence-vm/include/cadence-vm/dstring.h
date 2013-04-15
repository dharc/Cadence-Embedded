/*
 * includes/doste/dstring.h
 * 
 * C++ wrapper for DOSTE strings
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

#ifndef _CADENCE_DSTRING_H_
#define _CADENCE_DSTRING_H_

#include <cadence-vm/core/oid.h>
#include <cadence-vm/core/type_traits.h>

#include <cadence-embedded/dll.h>

namespace cadence {
	/**
	 * A class to convert c-strings to doste strings and vice-versa.
	 */
	class XARAIMPORT DString {
		public:
		DString();
		DString(const char *str, bool async=false);
		DString(core::OID o);
		~DString();
		
		void toString(char *str, int max);

		int size() const { return m_obj[core::Size]; };
		
		operator core::OID() const { return m_obj; };
		operator const char*();
		
		template <typename T>
		friend DString operator+(DString m, T v) { return m + cadence::core::OID(v); }
		XARAIMPORT friend DString operator+(DString m, const char *str);
		XARAIMPORT friend DString operator+(DString m, const cadence::core::OID &o);
		XARAIMPORT friend DString operator+(DString m, const DString &str);
		//friend bool operator==(const char *str);
		//friend bool operator==(const DString &str);
		
		private:
		core::OID m_obj;
		char *m_buffer;
		//static char s_unsafe[1024];
	};

	typedef DString dstring;

	namespace core {
		template <> class is_pod<DString> : public true_type {};
	};
};

#endif 
