/**
 * \file		utility.h
 * \author		Plugware Solutions.com, Ltd.
 * \date		
 * \brief		[brief here]
 *
 *              Copyright (c) 2003, 2004
 *              Plugware Solutions.com, Ltd.
 *
 *              Permission to use, copy, modify, distribute and sell 
 *              this software and its documentation for any purpose 
 *              is hereby granted without fee, provided that the above 
 *              copyright notice appear in all copies and the both that 
 *              copyright notice and this permission notice appear 
 *              in supporting documentation.  Plugware Solutions.com, Ltd. 
 *              makes no reresentations about the suitability of this 
 *              software for any purpose.  It is provided "as is" 
 *              without express or implied warranty.
 */

#ifndef _plugware_utility_h
#define _plugware_utility_h

/*
**  Includes
*/

#include <windows.h>

/*
**  Declarations
*/

namespace plugware
{
    /**
    * \class        singleton
    * \author       Plugware Solutions.com, Ltd.
    * \date         
    * \brief        Quick and easy singleton.
    *
    *               [details here]
    */
    template <typename T>
    struct singleton
    {
        static T& instance();
    };

    /**
    * \class        reference_counted
    * \author       Plugware Solutions.com, Ltd.
    * \date         
    * \brief        Quick and easy reference counting.
    *
    *               [details here]
    */
    struct reference_counted
    {
        LONG m_count;
        void release();
        void add_ref();
        reference_counted();
        virtual ~reference_counted();
    };

    /**
    * \class        smart_pointer
    * \author       Plugware Solutions.com, Ltd.
    * \date         
    * \brief        Quick and easy smart pointer.
    *
    *               [details here]
    */
    template <typename T>
    struct smart_pointer
    {
        smart_pointer(T* p) : m_p(0) { *this = p; }
        smart_pointer(const smart_pointer& that) : m_p(0) { *this = that.m_p; }
        ~smart_pointer() { if (m_p) { m_p->release(); m_p = 0; }}
        
        const smart_pointer& operator=(T* p);
        T* get() { return m_p; }
        T*const get() const { return m_p; }
        
        // data
        mutable T* m_p; 
    }; 

    /**
    * \class        critical_section
    * \author       Plugware Solutions.com, Ltd.
    * \date            
    * \brief        [brief here]
    *
    *               [details here]
    */
    class critical_section
    {
    public:     // interface

        critical_section() throw();
        ~critical_section();
        
        // modifiers
        void       unlock();
        const bool acquire_lock();

    private:    // data

        CRITICAL_SECTION m_h_cs;
    
    }; // class critical_section

    /**
    * \class        auto_lock
    * \author       Plugware Solutions.com, Ltd.
    * \date        
    * \brief        scoped locking mechanism
    *
    *               [details here]
    */
    struct auto_lock
    {
        ~auto_lock();
        auto_lock(critical_section&);
        const auto_lock& operator=(const auto_lock&);
        critical_section& m_c_lock;
    }; 
    
    /*
    **  Implementation
    */

    ////////////////////////////////////////////////////////////////////////////////
    // singleton
    ////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    T& singleton<T>::instance()
    {
        static T t_;
        return t_;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // reference_counted
    ////////////////////////////////////////////////////////////////////////////////
    inline reference_counted::reference_counted() 
    : m_count(0) 
    {
    }

    inline reference_counted::~reference_counted() 
    {
    }
    
    inline void reference_counted::release() 
    { 
        if (0 == ::InterlockedDecrement(&m_count))
        {
            delete this; 
        }            
    }
    
    inline void reference_counted::add_ref() 
    { 
        ::InterlockedIncrement(&m_count); 
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    // smart_pointer
    ////////////////////////////////////////////////////////////////////////////////
    template <class T>
    const smart_pointer<T>& smart_pointer<T>::operator=(T* p)
    {
        if (p == m_p) return *this;
        if (m_p) m_p->release();
        m_p = p;
        if (m_p) m_p->add_ref();
        return *this;
    }

    /////////////////////////////////////////////////////////////////////
    // critical_section
    /////////////////////////////////////////////////////////////////////
    inline critical_section::critical_section()
    {
        ::InitializeCriticalSection(&m_h_cs);
    }

    inline critical_section::~critical_section()
    {
        ::DeleteCriticalSection(&m_h_cs);
    }

    inline const bool critical_section::acquire_lock()
    {
    #if(_WIN32_WINNT >= 0x0400)
        return 1L & ::TryEnterCriticalSection(&m_h_cs);
    #else
        ::EnterCriticalSection(&m_h_cs);
        return true;
    #endif //_WIN32_WINNT >= 0x0400
    }

    inline void critical_section::unlock()
    {
        ::LeaveCriticalSection(&m_h_cs);
    }

    ///////////////////////////////////////////////////////////////////////
    // auto_lock
    ///////////////////////////////////////////////////////////////////////
    inline auto_lock::auto_lock(critical_section& c_lock) 
    : m_c_lock(c_lock) 
    { 
        while (false == m_c_lock.acquire_lock()) {}
    }        

    inline auto_lock::~auto_lock() 
    { 
        m_c_lock.unlock(); 
    }
} // namespace plugware
#endif //_plugware_utility_h