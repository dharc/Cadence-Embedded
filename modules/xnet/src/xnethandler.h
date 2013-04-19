#ifndef _cadence_XNETHANDLER_H_
#define _cadence_XNETHANDLER_H_

#include <cadence-vm/core/handler.h>
#include <cadence-vm/core/event.h>
#include <cadence-vm/list.h>
#include <cadence-vm/vector.h>

class XNetConnection;
class XNetCache;

class XNetHandler : public cadence::core::Handler {
	public:
	XNetHandler(XNetConnection *conn, const cadence::core::OID &base);
	~XNetHandler();

	bool handle(cadence::core::Event &evt);
	
	private:
		
	XNetConnection *m_conn;
	XNetCache *m_cache;
};

#endif
