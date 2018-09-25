#ifndef _DOT_EVENT_H
#define _DOT_EVENT_H
namespace dot {
  //Dot level event
	enum class DotEvent
	{
		INITIALIZED,
		TIMEOUT,
		CONNECTED,
		RESUME,
		PAUSE,
		DISCONNECTED
	};
}
#endif
