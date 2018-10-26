#ifndef _DOT_EVENT_H
#define _DOT_EVENT_H
namespace dot {
  //Dot level event
	enum class DotEvent
	{
		INITIALIZED,
		TIMEOUT,
		CONNECT,
		CONNECTED,
		RESUME,
		PAUSE,
		DISCONNECT,
		DISCONNECTED,
		FAILED
	};
}
#endif
