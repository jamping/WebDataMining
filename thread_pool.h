/**
 * \file		thread_pool.h
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

#pragma once

#if (_WIN32_WINNT < 0x0500)
#error Requires Windows Server 2003, Windows 2000 Server, or Windows NT Server 3.5 and later.
#endif //_WIN32_WINNT < 0x0500

/*
**  Includes
*/  

#pragma warning( disable: 4702) //unreachable code
#include <list>
#include <vector>
#pragma warning( default: 4702) //unreachable code
#include <crtdbg.h>
#include "utility.h"

/*
**  Declarations
*/

namespace plugware {
namespace core 
{
    /**
    * \class        thread_pool
    * \author       Plugware Solutions.com, Ltd.
    * \date         
    * \brief        Context to execute stateful work in a controllable, concurrent fashion.
    *               
    *               On win32 platforms implemented using iocp.  Work may be queued against the
    *               thread pool and will execute when a thread is free.  The thread pool provides
    *               fined grained concurrency control: number of threads running, total threads and a
    *               global initialization object available per thread pool made available to worker threads
    *               and work units.
    *
    *               Two related classes interact with the thread pool: work units worker threads.
    *               Work units are stateful objects encapsulating work to be done; worker thread 
    *               encapsulate the process by which work units are executed.
    */
    template <class worker>
    class thread_pool : public singleton<thread_pool<worker>>
    {
    public:  // interface

        typedef typename worker::request_type request_type;
        typedef std::vector<HANDLE> thread_vector;
        thread_pool();
        ~thread_pool();
    
        enum { _shutdown_flag_ = -1 };    
        void shutdown(const DWORD dw_max_wait = INFINITE);
        void initialize(size_t n_running_threads = 0, size_t n_thread_count = 0, const DWORD dw_stack_size = 0);
        void associate_device(HANDLE h_device);
        void queue_request(request_type* p_request);
        BOOL get_queued_status(LPDWORD pdw_bytes, request_type** pp_request, LPOVERLAPPED* pp_overlapped, DWORD dw_wait);

        // accessors
        HANDLE get_completion_port() { return m_h_iocp; }
        
        // const accessors
        const size_t get_size() const;
        int get_current_working_count() const { return m_nCurWorkingCounts; }
        void addto_working_count() { m_nCurWorkingCounts++; }
	    void delfrom_working_count() { m_nCurWorkingCounts--; }
    private:    // implementation

        // modifiers
        void create_iocp(const size_t n_running_threads);
        void create_threads(size_t n_thread_count, const size_t dw_stack_size);
        void flag_shutdown();
        void wait_for_threads_to_die(DWORD dw_max_wait);
   
        // accessors
        thread_vector& get_thread_vector() { return m_c_thread_vector; }
                 
        // const accessors
        const bool           shutdown_initiated() const { return m_b_shutdown_flag; }
        const thread_vector& get_thread_vector()  const { return m_c_thread_vector; }
        const bool           is_initialized()     const { return 0     != m_h_iocp &&
                                                                 false == get_thread_vector().empty(); }
        // static helpers
        static const size_t get_cpu_count();
        
    private:    // data
    
        bool          m_b_shutdown_flag;
        thread_vector m_c_thread_vector;
        HANDLE        m_h_iocp;
        int           m_nCurWorkingCounts;
    }; // class thread_pool
    
    /**
    * \class        work_unit
    * \author       Plugware Solutions.com, Ltd.
    * \date         
    * \brief        Interface for stateful work executed by a worker thread within a thread pool.
    *
    *               The work unit encapsulates all state pertinent to a task, as well as the process
    *               that operates on that state.  work_unit is explicitly bound to worker_thread
    *               as request_type; work_unit is specialized for worker_thread.
    */
    struct work_unit : reference_counted
    {
        virtual void process() throw() = 0;    
    };

    /**
    * \class        chain
    * \author       Plugware Solutions.com, Ltd.
    * \date         
    * \brief        [brief here]
    *
    *               [details here]
    */
    struct chain 
    {
        struct data : work_unit, std::list<smart_pointer<work_unit> >
        {
            void process() throw();
        };
        
        chain();
        chain& operator,(work_unit* p_work);
        operator work_unit*() { return m_work.get(); }

        smart_pointer<data> m_work;
    }; // struct chain

    /**
    * \class        worker_thread 
    * \author       Plugware Solutions.com, Ltd.
    * \date         
    * \brief        Encapsulation of thread running in thread pool.
    *
    *               work_unit serves as default request_type.
    */
    struct worker_thread
    {
        typedef work_unit request_type;
        typedef thread_pool<worker_thread> thread_pool;
        void execute(request_type* p_request) const throw();
        static DWORD WINAPI thread_proc(LPVOID pv) throw();
        const DWORD thread_proc() const;
        static void prepare_request(request_type* p_request) { p_request->add_ref(); }
    
    private:    // implementation
        
        worker_thread();
    }; // struct worker_thread

    /*
    **  Implementation
    */

    ////////////////////////////////////////////////////////////////////////////////
    // thread_pool
    ////////////////////////////////////////////////////////////////////////////////
    template <class worker>
    thread_pool<worker>::thread_pool()
    : m_b_shutdown_flag(false), m_h_iocp(0),m_nCurWorkingCounts(0)
    {
    }
    
    template <class worker>
    thread_pool<worker>::~thread_pool()
    {
#ifdef _DEBUG
        ::OutputDebugStringA("~thread_pool\n");
#endif //_DEBUG

        shutdown();
    }
    
    template <class worker>
    void thread_pool<worker>::initialize(size_t n_running_threads, size_t n_thread_count, const DWORD dw_stack_size)
    {
        // validate and assign defaults:
        // n_running_threads: cpu count
        // n_thread_count:    2 * n_running_threads
        if (true == is_initialized()) return;
        if (0 >= n_running_threads) n_running_threads = get_cpu_count();
        if (0 >= n_thread_count) n_thread_count = n_running_threads << 1;
        if (n_running_threads > n_thread_count) n_running_threads = n_thread_count;
        
        // store param, create iocp and threads
        create_iocp(n_running_threads);
        create_threads(n_thread_count, dw_stack_size);
    }
    
    template <class worker>
    void thread_pool<worker>::create_iocp(const size_t n_running_threads)
    {
        // validate, create and validate
        if (true == is_initialized()) return;
        
        //@@todo: migrate to thread traits
        m_h_iocp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, (DWORD)n_running_threads);
        if (0 == get_completion_port()) throw std::exception("CreateIoCompletionPort failed");
    }
    
    template <class worker>
    void thread_pool<worker>::create_threads(size_t n_thread_count, const size_t dw_stack_size)
    {
        // validate, reserve, create and store
        if (true == is_initialized()) return;
        get_thread_vector().reserve(n_thread_count);
        
        while (n_thread_count--)
        {
            //@@todo: migrate to thread traits
            //@@review: security, dw_thread_id, etc
            // create thread, validate and store
            DWORD  dw_thread_id = 0;
            HANDLE h_thread     = ::CreateThread(0, 
                                                 dw_stack_size, 
                                                 worker::thread_proc, 
                                                 0,
                                                 0,
                                                 &dw_thread_id);
                                                 
            if (0 == h_thread) throw std::exception("CreateThread failed");
            get_thread_vector().push_back(h_thread);
        }
    }
    
    template <class worker>
    void thread_pool<worker>::associate_device(HANDLE h_device)
    {
        // validate, associate and validate
        if (false == is_initialized()) throw std::exception("uninitialized");
        if (0 == ::CreateIoCompletionPort(h_device, get_completion_port(), 0, 0))
        {
            throw std::exception("CreateIoCompletionPort failed");
        }   
    }
    
    template <class worker>
    void thread_pool<worker>::queue_request(request_type* p_request)
    {
        // validate, if we are shutting down, bail
        _ASSERT(p_request);
        if (true == shutdown_initiated()) return;        
        worker::prepare_request(p_request);
        
        //@@todo: migrate to thread traits
        if (FALSE == ::PostQueuedCompletionStatus(get_completion_port(), 0, (ULONG_PTR)p_request, 0))
        {
            throw std::exception("PostQueuedCompletionStatus failed");
        }
        addto_working_count();
    }

    template <class worker>
    BOOL thread_pool<worker>::get_queued_status(LPDWORD pdw_bytes, request_type** pp_request, LPOVERLAPPED* pp_overlapped, DWORD dw_wait)
    {
        if (false == is_initialized()) throw std::exception("uninitialized");
        return ::GetQueuedCompletionStatus(get_completion_port(), pdw_bytes, (PULONG_PTR)pp_request, pp_overlapped, dw_wait);
    }

    template <class worker>
    void thread_pool<worker>::shutdown(const DWORD dw_max_wait)
    {
        // validate
        if (false == is_initialized()) return;
        if (true == shutdown_initiated()) return;
        
        // flag, wait
        flag_shutdown();
        wait_for_threads_to_die(dw_max_wait);
        
        // close handles and clear
        for (thread_vector::iterator handle(get_thread_vector().begin());
             handle != get_thread_vector().end();
             *handle && ::CloseHandle(*handle), ++handle) {}
        
        get_thread_vector().clear();
        ::CloseHandle(m_h_iocp);
        m_h_iocp = 0;
        m_nCurWorkingCounts=0;
    }
    
    template <class worker>
    void thread_pool<worker>::flag_shutdown()
    {
        // flag and queue
        m_b_shutdown_flag = true;
        size_t n_thread_count = get_thread_vector().size();
        
        //@@todo: migrate thread traits
        while (n_thread_count--) ::PostQueuedCompletionStatus(get_completion_port(),
                                                              0,
                                                              0,
                                                              (LPOVERLAPPED)_shutdown_flag_);
    }
    
    template <class worker>
    void thread_pool<worker>::wait_for_threads_to_die(const DWORD dw_max_wait)
    {
        ::WaitForMultipleObjects((DWORD)get_thread_vector().size(), 
                                 &get_thread_vector().at(0), 
                                 TRUE, 
                                 dw_max_wait);
    }
    
    template <class worker>
    const size_t thread_pool<worker>::get_cpu_count()
    {
        SYSTEM_INFO c_si = {0};
        ::GetSystemInfo(&c_si);
        return c_si.dwNumberOfProcessors;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    //  chain
    ///////////////////////////////////////////////////////////////////////////////
    inline chain::chain() 
    : m_work(new(data)) 
    {
    }
    
    inline chain& chain::operator,(work_unit* p_work)
    {
        _ASSERT(0 != p_work);
        _ASSERT(0 != m_work.get());
        m_work.get()->push_back(p_work);
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //  chain::data
    ///////////////////////////////////////////////////////////////////////////////
    inline void chain::data::process()
    {
        _ASSERT(size());
        _ASSERT(front().get());
        
        front().get()->process();
        pop_front();
        
        // if not empty, requeue
        if (true == empty()) return;
        thread_pool<worker_thread>::instance().queue_request(this);
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    // worker_thread
    ////////////////////////////////////////////////////////////////////////////////
    inline worker_thread::worker_thread()
    {
    }
    
    inline DWORD WINAPI worker_thread::thread_proc(LPVOID) throw()
    {
        try
        {
            // return values:
            //  0: success
            // -1: failure
            return worker_thread().thread_proc();
        }
        catch (...) {}            
        return DWORD(-1);
    }
    
    inline const DWORD worker_thread::thread_proc() const
    {
        // declare
        DWORD         dw_bytes_transferred = 0;
        OVERLAPPED*   p_overlapped         = 0;
        request_type* p_request            = 0;
        
        // main loop
        for (;;)
        { 
            // get next packet, on failure assert and and continue
            if (FALSE == 
                thread_pool::instance().get_queued_status(
                    &dw_bytes_transferred,
                    &p_request,
                    &p_overlapped,
                    INFINITE))
            {
                // nothing to see, move along ...
                continue;
            } 

            // shutdown or process
            if (thread_pool::_shutdown_flag_ == (UINT_PTR)p_overlapped) break;

            // no io requests, please
            _ASSERT(0 == p_overlapped);
            execute(p_request);
            core::thread_pool<core::worker_thread>::instance().delfrom_working_count();
        }
        
#ifdef _DEBUG    
        ::OutputDebugStringA("worker_thread: terminated\n");
#endif //_DEBUG

        return 0;
    }
    
    inline void worker_thread::execute(request_type* p_request) const throw()
    {
        // validate and process (io or logic)
        if (0 == p_request) return;
        p_request->process();
        p_request->release();
    }
} // namespace core    

    typedef core::thread_pool<core::worker_thread> thread_pool;
} // namespace plugware
typedef plugware::thread_pool thread_pool;