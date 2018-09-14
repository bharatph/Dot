#ifndef _DOT_OPERATION_H
#define _DOT_OPERATION_H

#include <Dot/DotOperationEventManager.hpp>

namespace dot {
class DotOperation : public DotOperationEventManager {
    public:
    DotOperation();
    ~DotOperation();
    virtual void notify(std::string);
};
}

#endif
