#pragma once

#include <VTF/Pointers>
#include <VTF/atomics.h>

namespace VTF {

class RefObject
{
    NONCOPYABLE(RefObject)

    AtomicCounter m_counter;
public:
    RefObject();
    void ref();
    bool deref();
    bool isRef() const;
};

}
