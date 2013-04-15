/*
 * src/dvm/local.h
 * 
 * Internal RAM storage for DOSTE database objects.
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

#ifndef _doste_DVM_VSTORE_H_
#define _doste_DVM_VSTORE_H_

#include <cadence-vm/core/handler.h>
#include <cadence-vm/core/oid.h>
#include <cadence-vm/list.h>
#include <cadence-vm/vector.h>
#include <cadence-vm/core/definition.h>
#include <cadence-vm/core/processor.h>
//#include <cadence/spinlock.h>

class OIDPair {
	public:
	friend inline bool operator==(const OIDPair &p1, const OIDPair &p2);

	cadence::core::OID obj;
	cadence::core::OID key;
};

class Dependency : public OIDPair {
	public:
	bool isdep;
};

namespace cadence {
	namespace handlers {
		class VStore : public core::Handler {
			public:
			VStore();
			~VStore();
			
			bool handle(core::Event &evt);
			
			private:
			
			static core::OID s_alloc;
			
			class Attribute;
			
			Attribute *findAttribute(const cadence::core::OID &d, const cadence::core::OID &p);

			class Object {
				public:
				Object();
				Object(const core::OID &o);
				~Object();
				
				const core::OID &getOID() const { return m_oid; };
				core::OID getKeysBuffer();
				core::OID getRangeBuffer(int a, int b);
				void add(const core::OID &key) { m_keys.add(key); };
				bool contains(const core::OID &key) const { return m_keys.contains(key); };
				void all(Attribute *a) { m_all = a; };
				Attribute *all() const { return m_all; };	

				core::OID specialGet(const core::OID &k);

				void copy(const core::OID &nobj);
				core::OID keys();
				core::OID size();

				void addReference() { m_refcount++; };
				void removeReference() {
					m_refcount--;
					if (m_refcount <= 0) {
						//std::cout << "Deleting Object: <" << m_oid.a() << ":" << m_oid.b() << ":" << m_oid.c() << ":" << m_oid.d() << ">\n";
						//delete this;
					}
				}

				static Object *get(const core::OID &o);

				static void initialise();
				static void finalise();
				
				private:
				//doste::ReadWriteLock m_rwlock;	//Lock for key modifications.
				core::OID m_oid;
				Object *next;
				Attribute *m_all;
				int m_refcount;

				static const unsigned int OBJECT_HASH_SIZE = 10000;
				
				Vector<core::OID> m_keys;
				static Object *s_hash[OBJECT_HASH_SIZE];
				//static doste::ReadWriteLock s_hlock;
				
				static int hashOID(const core::OID &o) { return o.d() % OBJECT_HASH_SIZE; };
			};
			
			class Attribute {
				public:
				Attribute(const core::OID &obj, const core::OID &key);
				~Attribute();
				
				const core::OID &getKey() const { return m_key; };
				const core::OID &getValue() {
					//if ((m_flags & OID::FLAG_FUNCTION) != 0) {
					//	m_value = Definition(m_def).evaluate(m_obj, m_key, true);
					//}
					return m_value;
				}

				const core::OID &getValue(const core::OID &key) {
					//if ((m_flags & OID::FLAG_FUNCTION) != 0) {
					//	if (evaluating) {
							//ERROR, cyclic
					//		Error(Error::CYCLIC, "Cyclic 'is' definition detected");
					//		m_value = Null;
					//	} else {
					//		evaluating = true;
					//		m_value = Definition(m_def).evaluate(m_obj, key, true);
					//		evaluating = false;
					//	}
					//}
					return m_value;
				}

				const core::OID &getObject() const { return m_obj; };

				bool setValue(const core::OID &value);

				void setDefinition(const core::OID &def, int flags, const core::OID &init);
				const core::OID &getDefinition() const { return m_def; };
				int getFlags() const { return m_flags; };
				void setFlags(int flag);
				//bool isAgent() const { return (m_flags & OID::FLAG_AGENT) != 0; };
				bool isFunction() const { return (m_flags & core::OID::FLAG_FUNCTION) != 0; };
				bool isDefinition() const { return m_def != core::Null; };
				void notify();
				void addDependent(const core::OID &obj, const core::OID &key, bool is=false);

				void notifyAll();
				void notifyAll(const core::OID &key, const core::OID &value);
			
				static Attribute *get(const core::OID &obj, const core::OID &key);
					
				static void initialise();
				static void finalise();

				bool evaluating;

				private:
				core::OID m_obj;
				core::OID m_key;
				core::OID m_value;
				unsigned char m_instant;

				List<Dependency> m_deps;
				core::OID m_def;
				void *m_compdef;
				unsigned char m_flags;
				Attribute *next;
				//SpinLock m_lock;

				static const unsigned int ATTRIB_HASH_SIZE = 200000;

				void evaluate(const core::OID &me);
				
				static Attribute *s_hash[ATTRIB_HASH_SIZE];
				static int hashOIDS(const core::OID &o, const core::OID &k) {
					return ((k.d() << 16)+o.d()) % ATTRIB_HASH_SIZE;
				};
			};
		};
	};
};

bool operator==(const OIDPair &p1, const OIDPair &p2) {
	return (p1.obj == p2.obj && p1.key == p2.key);
}

#endif 
