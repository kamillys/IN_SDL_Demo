#include "atomics.h"


VTF::AtomicCounter::AtomicCounter()
{
    m_counter = 0;
}

int VTF::AtomicCounter::incrementAndGet() const
{
    return m_counter++;
}

int VTF::AtomicCounter::decrementAndGet() const
{
    return m_counter--;
}

int VTF::AtomicCounter::get() const
{
    return m_counter;
}
