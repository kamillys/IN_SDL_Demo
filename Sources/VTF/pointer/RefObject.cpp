#include "RefObject.h"

using namespace VTF;

RefObject::RefObject()
{
}

void RefObject::ref()
{
    m_counter.incrementAndGet();
}

bool RefObject::deref()
{
    return 0 == m_counter.decrementAndGet();
}

bool RefObject::isRef() const
{
    return 0 != m_counter.get();
}
