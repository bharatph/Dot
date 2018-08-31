#ifndef _DOT_OP_EVENT
#define _DOT_OP_EVENT

namespace dot {
//Events have states
//success
//failed - decribe failure (timeout? no network? network unreachable?)
//undergoing //can be polled by the user
enum class DotOperationEvent {
    SUCCESS,
    FAILED,
    PROCESSING
};
}

#endif
