#ifndef _XNETCACHE_
#define _XNETCACHE_

#include <cadence-vm/core/handler.h>
#include <cadence-vm/core/oid.h>

class XNetConnection;

struct CacheEntry {
	cadence::core::OID n;
	cadence::core::OID e;
	cadence::core::OID value;
	bool out_of_date;
	CacheEntry *next;
};

class XNetCache : public cadence::core::Handler {
	public:
		
	XNetCache(XNetConnection *conn);
	~XNetCache();
	
	bool handle(cadence::core::Event &evt);
	
	const cadence::core::OID &get(const cadence::core::OID &n, const cadence::core::OID &e);
	//const cadence::doste::OID &getkeys(const cadence::doste::OID &n);
	
	private:

	static const unsigned int CACHE_SIZE = 10000;
	static const unsigned int KEY_CACHE_SIZE = 1000;
	CacheEntry *m_cache[CACHE_SIZE];
	//KeyCacheEntry *m_keycache[KEY_CACHE_SIZE];
	XNetConnection *m_conn;
	
	static int hashOIDS(const cadence::core::OID &o, const cadence::core::OID &k) {
		return (o.d()+k.d()) % CACHE_SIZE;
	};
	
	static int hashOID(const cadence::core::OID &o) {
		return o.d() % KEY_CACHE_SIZE;
	};
	
	CacheEntry *lookup(const cadence::core::OID &n, const cadence::core::OID &e);
	//KeyCacheEntry *lookup(const cadence::doste::OID &n);
	int lookupCount(CacheEntry *e, int hash);
};

#endif
