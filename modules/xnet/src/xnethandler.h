#ifndef _cadence_XNETHANDLER_H_
#define _cadence_XNETHANDLER_H_

#include <cadence-embedded/core/handler.h>
#include <cadence-embedded/core/event.h>
#include <cadence-embedded/list.h>
#include <cadence-embedded/vector.h>

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
