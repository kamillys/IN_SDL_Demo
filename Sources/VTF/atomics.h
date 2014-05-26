#pragma once

#include <util.h>
//#include <boost/atomic.hpp>
#include <atomic>

namespace VTF {

class AtomicCounter
{
    NONCOPYABLE(AtomicCounter)

    //mutable boost::atomic<int> m_counter;
    mutable std::atomic<int> m_counter;
public:
    AtomicCounter();
    int incrementAndGet() const;
    int decrementAndGet() const;
    int get() const;
};

}
