#pragma once

#include <exception.h>

namespace VTF {

template <class T>
class RefPointer
{
private:
    T* m_ptr;
public:

    /**
     * Construction & management methods
     */
    RefPointer() : m_ptr(0) {}
    ~RefPointer() { release(); }

    void release() {
        if (!m_ptr)
            return;
        if (m_ptr->deref())
            delete m_ptr;
        m_ptr = 0;
    }

    RefPointer(T* ptr) : m_ptr(ptr) { if (m_ptr) m_ptr->ref(); }
    RefPointer(const RefPointer<T>& ptr) : m_ptr(ptr.m_ptr) { if (m_ptr) m_ptr->ref(); }

    RefPointer<T>& operator = (const T* ptr)
    {
        release();
        m_ptr = const_cast<T*>(ptr);
        if (m_ptr)
            m_ptr->ref();
        return *this;
    }

    RefPointer<T>& operator = (const RefPointer<T>& ptr)
    {
        if (&ptr != this)
        {
            release();
            m_ptr = ptr.m_ptr;
            if (m_ptr)
                m_ptr->ref();
        }
        return *this;
    }

    template<class U>
    RefPointer<T>& operator = (const RefPointer<U>& ptr)
    {
        if (ptr.get() != this->m_ptr)
        {
            release();
            m_ptr = static_cast<T*>(const_cast<U*>(ptr.get()));
            if (m_ptr)
                m_ptr->ref();
        }
        return *this;
    }

    /**
     * Access methods
     */

    const T* get() const { return m_ptr; }
    T* get() { return m_ptr; }

    operator const T* ()
    {
        return m_ptr;
    }

    operator T* ()
    {
        return m_ptr;
    }

    operator bool ()
    {
        return m_ptr != 0;
    }

    T* operator -> ()
    {
#ifndef VTF_DISABLE_CHECKS
        if (!m_ptr)
            THROW_EXCEPTION_T("Accessing NULL pointer", VTF::NullPointerException);
#endif
        return m_ptr;
    }

    const T* operator -> () const
    {
        return m_ptr;
    }

    /**
     * Comparison methods
     */
    bool operator==(const RefPointer<T>& ptr )
    { return ptr.m_ptr == m_ptr; }
    bool operator==(const T* ptr )
    { return ptr == m_ptr; }
    bool operator!=(const RefPointer<T>& ptr )
    { return ptr.m_ptr != m_ptr; }
    bool operator!=(const T* ptr )
    { return ptr != m_ptr; }
    bool operator < (const RefPointer<T>& ptr) const
    { return m_ptr < ptr.m_ptr; }
    bool operator > (const RefPointer<T>& ptr) const
    { return m_ptr > ptr.m_ptr; }
    bool operator <= (const RefPointer<T>& ptr) const
    { return m_ptr <= ptr.m_ptr; }
    bool operator >= (const RefPointer<T>& ptr) const
    { return m_ptr >= ptr.m_ptr; }
};

}
