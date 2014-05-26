#pragma once

#include <util.h>

namespace VTF {

template <class T>
class OwnPointer
{
private:
    T* m_ptr;

    OwnPointer(const OwnPointer<T>& ptr);
public:

    /**
     * Construction & management methods
     */
    OwnPointer() : m_ptr(0) {}
    ~OwnPointer() { release(); }

    void release() {
        if (!m_ptr)
            return;
        delete m_ptr;
        m_ptr = 0;
    }

    OwnPointer(T* ptr) : m_ptr(ptr) { }

    OwnPointer<T>& operator =(const T* ptr)
    {
        release();
        m_ptr = const_cast<T*>(ptr);
        return *this;
    }

    OwnPointer<T>& operator =(const OwnPointer<T>& ptr)
    {
        if (&ptr != this)
        {
            release();
            m_ptr = ptr.m_ptr;
        }
        return *this;
    }

    /**
     * Access methods
     */

    const T* get() const { return m_ptr; }
    T* get() { return m_ptr; }

    operator T* ()
    {
        return m_ptr;
    }

    T* operator -> ()
    {
        return m_ptr;
    }

    const T* operator -> () const
    {
        return m_ptr;
    }
};

}
