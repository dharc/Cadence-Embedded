//Get the files contained in a directory

#ifndef _DOSTE_DIRECTORY_H_
#define _DOSTE_DIRECTORY_H_


#include <cadence-vm/core/handler.h>
#include <cadence-vm/dstring.h>
#include <cadence-vm/vector.h>

#undef MAX_PATH //damn windows
#define MAX_PATH	400
#define MAX_ENTRIES	100

namespace cadence {
	struct DirEntry {
		bool available;
		dstring name;
		char path[MAX_PATH];
		core::OID entries[MAX_ENTRIES];
		int size;
	};

	class Directory : public core::Handler {
		public:
		Directory();
		~Directory();

		bool handle(core::Event &evt);

		private:
		Vector<DirEntry*> m_dirs;

		void search(DirEntry *ent);
		core::OID addDirectory(const char *, const char *);
		core::OID addFile(const char *, const char *);
		core::OID lookupEntry(int id, const core::OID &entry);
	};
};

#endif
