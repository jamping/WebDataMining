#pragma once

#pragma warning(disable : 4786)
#pragma warning(disable : 4275)
#pragma warning(disable : 4251)

#include <memory>
#include <algorithm>
#include <sstream>
#include <list>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/detail/lightweight_mutex.hpp>
#include <iostream>

#undef DEFAULT_OSTREAM_PTR
#define DEFAULT_OSTREAM_PTR  &std::cout

#define LOG_DEBUG(message) \
do{\
    if( robert::LogLoger::instance().isDebugging() )\
	{\
	   std::stringstream _log_debug_buf; \
       _log_debug_buf << message;\
       robert::LogLoger::instance().Debug(DEFAULT_OSTREAM_PTR,_log_debug_buf.str().c_str() );\
	}\
}while(0);

namespace robert
{	
	//////////////////////////////////
	//singleton
	//
	//////////////////////////////////
	template <typename T>
	class singleton
    {
	public:
		typedef boost::mutex          mutex_t;			
	    typedef mutex_t::scoped_lock  scoped_lock_t;
     public:		
        static T& instance();
	 private:		
		static T * instance_;
		static mutex_t lock_;
    };
	
	//////////////////////////////////
	//delay
	//
	//////////////////////////////////
	static boost::xtime delay(int secs, int msecs=0, int nsecs=0)
	{
		const int MILLISECONDS_PER_SECOND = 1000;
		const int NANOSECONDS_PER_SECOND = 1000000000;
		const int NANOSECONDS_PER_MILLISECOND = 1000000;
		
		boost::xtime xt;
		boost::xtime_get (&xt, boost::TIME_UTC);			
		
		nsecs += xt.nsec;
		msecs += nsecs / NANOSECONDS_PER_MILLISECOND;
		secs += msecs / MILLISECONDS_PER_SECOND;
		nsecs += (msecs % MILLISECONDS_PER_SECOND) * NANOSECONDS_PER_MILLISECOND;
		xt.nsec = nsecs % NANOSECONDS_PER_SECOND;
		xt.sec += secs + (nsecs / NANOSECONDS_PER_SECOND);		
		return xt;
	}
    
	////////////////////////////////////////////
	//LogLoger
	//
	////////////////////////////////////////////
	struct LogLoger	: singleton<LogLoger>
	{
         typedef boost::mutex          mutex_t;			
	     typedef mutex_t::scoped_lock  scoped_lock_t;
		 
		 void setInternalDebugging(bool bDebug)
		 {
			 debugging_ = bDebug;
		 }
		 bool isDebugging()const{return debugging_;}

		 void Debug(std::ostream * out,const char * message )
		 {
			 if( debugging_ )
			 {
				 scoped_lock_t guard( lock_ );
				 *out << message << std::endl;
			 }
		 }
	   private:
		   bool debugging_;
		   mutex_t lock_;
	};
	
	namespace core
	{
		////////////////////////////////////////
		//前置声明
		template<class worker> class thread_pool;

		////////////////////////////////////////////
		//thread_pool_func
		//线程池监控线程函数对象
		////////////////////////////////////////////
		template<class worker>
		struct thread_pool_func
		{					
			thread_pool_func(thread_pool<worker> * pool):pool_(pool){}
			void operator()();
		private:			
			thread_pool<worker> * pool_;
		};

		////////////////////////////////////
		//线程池管理策略类
		//thread_pool_strategy
		////////////////////////////////////
		class thread_pool_strategy
		{
		public:
			virtual ~thread_pool_strategy(){}
			virtual unsigned int get_delete_count(unsigned int idles,unsigned int max_count) = 0;
		};

		class default_thread_pool_strategy : public thread_pool_strategy
		{
		public:
			unsigned int get_delete_count(unsigned int idles,unsigned int max_count)
			{
				if( idles > max_count / 2 )
				{
					return  idles / 10;
				}
				return 0;
			}
		};

		////////////////////////////////////////////
		//thread_pool
		//
		////////////////////////////////////////////
		template<class worker>
		class thread_pool : boost::noncopyable, public singleton<thread_pool<worker> >
		{				
 			friend struct thread_pool_func<worker>;			
	    public:
		      typedef typename worker::request_type        request_type;
		      typedef typename worker::request_ptr         request_ptr;			  
			  typedef std::auto_ptr<worker>                sp_worker_ptr;
			  typedef worker*                              worker_pointer;
			  typedef std::queue<request_ptr>              request_queue_t;
			  typedef std::list<worker_pointer>            thread_list_t;	
			  typedef typename thread_list_t::iterator     worker_iterator;
			  typedef boost::mutex                         mutex_t;	
			  typedef boost::detail::lightweight_mutex     lw_mutex_t;
			  typedef lw_mutex_t::scoped_lock              lw_scoped_lock_t; 
			  typedef mutex_t::scoped_lock                 scoped_lock_t;
			  typedef boost::condition                     condition_t;	  
			  typedef std::auto_ptr<thread_pool_strategy>  sp_strategy_ptr;
		      			  
			  //////////////////////
			  //ctor/dtor
			  thread_pool();
              ~thread_pool();
			  
			  /////////////////////////////////////////////
			  //public method			  
              void shutdown();
              void initialize(size_t init_threads = 0, size_t max_thread_count = 0,
				               sp_strategy_ptr strategy = sp_strategy_ptr(new default_thread_pool_strategy() ) );
		      void add_request(request_ptr request);   
			  
			  bool is_shutdowned()const{ return shutdowned_; }
			  void add_to_free_list(worker_pointer worker_thread);
			  void remove_from_working_list(worker_pointer worker_thread);
			  void setInternalDebugging(bool bDebug);
			  int get_current_working_count() { return get_current_count(); }
	    private:		
			  /////////////////////////////
	          //管理方法   	
			  request_ptr get_request(size_t millisecs);
			  sp_worker_ptr get_from_free_list(size_t millisecs);
			  void add_to_working_list(sp_worker_ptr worker_thread);			  
			  void release_free_thread(unsigned int count);
			  //属性
			  unsigned int get_max_count()const;
			  void set_max_count(unsigned int count);
			  unsigned int get_current_count()const;
			  unsigned int get_idle_count()const;
			  bool is_initialized()const{ return (monitor_thread_ !=0 );}
			  void change_strategy( sp_strategy_ptr s );
              thread_pool_strategy * get_strategy()const;
			  ///////////////////////////
			  //监控函数
			  void run();
			  /////////////////////////////
			  //资源释放
			  void shutdown_all_free_threads();
			  void shutdown_all_working_threads();
			  void remove_all_requests();			  
		private:
			  bool               shutdowned_;			 
			  mutable lw_mutex_t max_threads_lock_;
			  unsigned int       max_threads_;

			  mutable lw_mutex_t strategy_lock_;
			  sp_strategy_ptr    strategy_;
			  boost::thread  * monitor_thread_;
			  mutex_t          request_q_lock_;
			  condition_t      request_q_cond_; 
			  request_queue_t  request_queue_;
              
			  mutable mutex_t  working_list_lock_;			 
			  thread_list_t    working_thread_list_;

			  mutable mutex_t  free_list_lock_;	
			  condition_t      free_list_cond_;
			  thread_list_t    free_thread_list_;			  
		};
		
		/////////////////////////////////
		//work_unit
		//
		/////////////////////////////////
		struct work_unit
		{			
			virtual ~work_unit(){}
			virtual void process() throw() = 0;    
		};
		
		/////////////////////////////////
		//worker_thread
		//
		/////////////////////////////////
		struct worker_thread
		{
			typedef work_unit request_type;
			typedef thread_pool<worker_thread>       thread_pool_t;			
			typedef boost::shared_ptr<request_type>  request_ptr;
			typedef boost::mutex                     mutex_t;			
			typedef mutex_t::scoped_lock             scoped_lock_t;
			typedef boost::condition                 condition_t;
			
			static const int DEFAULT_WAIT_REQUEST_TIMEOUT;
			static const int DEFAULT_WAIT_THREAD_TIMEOUT;

			struct wroker_thread_func;
			friend struct wroker_thread_func;
			struct wroker_thread_func
			{
				wroker_thread_func(worker_thread * worker):worker_(worker){}
				void operator()()
				{
					if( worker_ )
                       worker_->run();
				}
			private:
				worker_thread * worker_;
			};

			worker_thread():shutdowned_(false),activityed_(false),thd_( 0 )
			{
				thd_ = new boost::thread( wroker_thread_func(this));
			}
			~worker_thread()
			{
				shutdown();
			}
			void shutdown();
			void activity(bool bActive);
			void wait_activitied();
			void assign(request_ptr request)throw();			
		private:
			void executeRequest();
			void run();
		private:
			bool            shutdowned_;
			mutex_t         active_lock_;
			condition_t     active_cond_;
			bool            activityed_;
            request_ptr     request_;
			boost::thread * thd_;
		};
		
		
		/////////////////////////////////
		//thread_pool implement
		//
		/////////////////////////////////
		template<class worker>
		thread_pool<worker>::thread_pool()
		  :shutdowned_(false),max_threads_(0),monitor_thread_(0)
		{
			setInternalDebugging(false);
		}
		
		template<class worker>
		thread_pool<worker>::~thread_pool()
		{
			shutdown();
		}
		
		template<class worker>
		void thread_pool<worker>::shutdown()
		{
			shutdowned_ = true;
			if( monitor_thread_ )
               monitor_thread_->join();
			delete monitor_thread_;
			monitor_thread_ = 0;

			remove_all_requests();			
			shutdown_all_free_threads();
			shutdown_all_working_threads();            
			LOG_DEBUG("thread_pool is shutdowned!");
		}
		
		//////////////////////////////////////////////////
		//名称: initialize
		//参数：
		// init_threads 初始启动的空闲线程数
		// max_thread_count 最大允许线程数
		// strategy 空闲线程删除策略
		// 
		//////////////////////////////////////////////////
		template<class worker>
		void thread_pool<worker>::initialize(size_t init_threads, 
		          size_t max_thread_count,sp_strategy_ptr strategy)
		{				            
			change_strategy( strategy );			
			if( !is_initialized() )
			{				
				max_threads_ = max_thread_count;
				monitor_thread_ = new boost::thread(thread_pool_func<worker>(this));
				
				for(size_t i = 0; i < init_threads; i++)
				{
					try
					{
                    free_thread_list_.push_back( new worker() ); 
					}
					catch(boost::thread_resource_error & e)
					{
						LOG_DEBUG("EXCEPTION->>>>>>>>>>>>>>>>>>>>" << e.what() <<" count="<< i );
					}
				}
			}		
			else
			{	 	 
				 int tmp = 0;
				 tmp = max_thread_count- max_threads_;
				 
				 unsigned int cur = get_current_count();
				 unsigned int idles = get_idle_count();

                 if( tmp > 0 )
				 {
                     set_max_count( max_thread_count );
					 release_free_thread( idles );
				 }	
				 else
				 {
					 tmp = max_threads_-max_thread_count ;
                     /////////////////////////////////////
					 //如果活动线程数大于新的最大线程数，
					 //则设置当前活动的线程数为最大线程数					 
					 if( cur>max_thread_count ) //(idles <= tmp  )
					 {
						 set_max_count( cur );
                         release_free_thread( idles );
					 }
					 else
					 {
						 ///////////////////////////////////						 				 
						 set_max_count( max_thread_count );
                         release_free_thread( (idles <= tmp )?idles:tmp );
					 }
				 }
			}
		}
		        
		template<class worker>
		void thread_pool<worker>::change_strategy( sp_strategy_ptr s )
		{
			lw_scoped_lock_t guard( strategy_lock_);
			strategy_ = s;
		}

		template<class worker>
		thread_pool_strategy * thread_pool<worker>::get_strategy()const
		{
			thread_pool_strategy * result = 0;
			{
			lw_scoped_lock_t guard( strategy_lock_);
			result = strategy_.get();			
			}
			return result;
		}

		//////////////////////////////////////////////////
		//名称: add_request		
		//将请求添加到请求队列中
		//////////////////////////////////////////////////
		template<class worker>
		void thread_pool<worker>::add_request(request_ptr request)
		{
			scoped_lock_t guard( request_q_lock_ );
			request_queue_.push( request );
			request_q_cond_.notify_one();
		}
		//////////////////////////////////////////////////
		//名称: get_request
		//参数：
		// millisecs 等待超时的毫秒数		
		// 
		//////////////////////////////////////////////////
		template<class worker>
		typename thread_pool<worker>::request_ptr thread_pool<worker>::get_request(size_t millisecs)
		{
			scoped_lock_t guard( request_q_lock_ );

			boost::xtime xt = delay(0,millisecs);
            
            if( request_queue_.empty() )
				request_q_cond_.timed_wait(guard,xt);			
            
			request_ptr request;
			if( !request_queue_.empty() )
			{
			   request = request_queue_.front();
			   BOOST_ASSERT( request.get() != 0 );
			   request_queue_.pop();
			}
			return request;
		}

        template<class worker>
		void thread_pool<worker>::setInternalDebugging(bool bDebug)
		{
		   robert::LogLoger::instance().setInternalDebugging(bDebug);
		}
		///////////////////////////////////
		//私有属性
		template<class worker>
		unsigned int thread_pool<worker>::get_max_count()const
		{
			unsigned int result = 0;			
			{
			 lw_scoped_lock_t guard(max_threads_lock_);
			 result = max_threads_;
			}
			return result;
		}
		template<class worker>
		void thread_pool<worker>::set_max_count(unsigned int count)
		{
			lw_scoped_lock_t guard(max_threads_lock_);
            max_threads_ = count;
		}

		template<class worker>
		unsigned int thread_pool<worker>::get_current_count()const
		{
			unsigned int result = 0;
			{
			scoped_lock_t guard( working_list_lock_ );
			result = working_thread_list_.size();            
			}			
			return result;
		}
		template<class worker>
		unsigned int thread_pool<worker>::get_idle_count()const
		{
			unsigned int result = 0;
			{
			scoped_lock_t guard( free_list_lock_ );
			result = free_thread_list_.size();   			
			}
			return result;
		}
        
		////////////////////////////////////////////////
		//add_to_free_list
		//将执行完工作后的线程放入空闲线程连表
		////////////////////////////////////////////////
		template<class worker>
		void thread_pool<worker>::add_to_free_list(worker_pointer worker_thread)
		{
			if( !worker_thread )return;
			scoped_lock_t guard( free_list_lock_ );
            free_thread_list_.push_back( worker_thread );	
			free_list_cond_.notify_one();
		}		

		////////////////////////////////////////////////
		//release_free_thread
		//从空闲线程连表中释放指定(count)数量的线程
		////////////////////////////////////////////////
		template<class worker>
		void thread_pool<worker>::release_free_thread(unsigned int count)
		{
             if( count == 0 ) return;
			 scoped_lock_t guard( free_list_lock_ );  
			 
			 for(unsigned int i = 0; i < count; ++i)
			 {
				 worker_pointer pointer = free_thread_list_.back();
				 delete pointer;
				 free_thread_list_.pop_back();
			 }
		}

		////////////////////////////////////////////////
		//get_from_free_list
		//从空闲线程连表中取得线程。
		////////////////////////////////////////////////
		template<class worker>
		typename thread_pool<worker>::sp_worker_ptr thread_pool<worker>::get_from_free_list(size_t millisecs)
		{			
			sp_worker_ptr result;
			{
				scoped_lock_t guard( free_list_lock_ );			
				
				boost::xtime xt = delay(0,millisecs);
				
				if( free_thread_list_.empty() )
					free_list_cond_.timed_wait(guard,xt);			
				if( !free_thread_list_.empty() )
				{
					result = sp_worker_ptr( free_thread_list_.front() ); 
					free_thread_list_.pop_front();
				}
			}
			return result;
		}

		////////////////////////////////////////////////
		//remove_from_working_list
		//
		////////////////////////////////////////////////
		template<class worker>
		void thread_pool<worker>::remove_from_working_list(worker_pointer worker_thread)
		{			
			if( !worker_thread ) return;
			scoped_lock_t guard( working_list_lock_ );
			worker_iterator it = std::find(working_thread_list_.begin(),working_thread_list_.end(),worker_thread);
			if( it != working_thread_list_.end() )
			{
                working_thread_list_.erase( it );
			}
		}

		////////////////////////////////////////////////
		//remove_from_working_list
		//
		////////////////////////////////////////////////
		template<class worker>
	    void thread_pool<worker>::add_to_working_list(sp_worker_ptr worker_thread)
		{
			if( !worker_thread.get() ) return;
			scoped_lock_t guard( working_list_lock_ );	
			working_thread_list_.push_back( worker_thread.release() );
		}

		////////////////////////////////////////////////
		//shutdown_all_free_threads
		//删除所有空闲线程
		////////////////////////////////////////////////
		template<class worker>
		void thread_pool<worker>::shutdown_all_free_threads()
		{
             scoped_lock_t guard( free_list_lock_ );
			 
			 for(worker_iterator it = free_thread_list_.begin(); it != free_thread_list_.end();)
			 {
				 delete *it;
				 it = free_thread_list_.erase( it );
			 }			 
		}

		////////////////////////////////////////////////
		//shutdown_all_working_threads
		//删除所有工作线程
		////////////////////////////////////////////////
		template<class worker>
		void thread_pool<worker>::shutdown_all_working_threads()
		{
            scoped_lock_t guard( working_list_lock_ );
			for(worker_iterator it = working_thread_list_.begin(); it != working_thread_list_.end();)
			{
				delete *it;
				it = working_thread_list_.erase( it );
			}			 
		}
       
		////////////////////////////////////////////////
		//remove_all_requests
		//
		////////////////////////////////////////////////
		template<class worker>
		void thread_pool<worker>::remove_all_requests()
		{
             scoped_lock_t guard( request_q_lock_ );
             while( !request_queue_.empty() )
                request_queue_.pop();
		}		
		
		////////////////////////////////////////////////
		//run
		//
		////////////////////////////////////////////////
		template<class worker>
		void thread_pool<worker>::run()
		{
			while(!shutdowned_ )
			{					 
				try
				{
					request_ptr request = get_request( worker::DEFAULT_WAIT_REQUEST_TIMEOUT );				 
					if( request.get() )
					{								
						sp_worker_ptr worker_thd = get_from_free_list(worker::DEFAULT_WAIT_THREAD_TIMEOUT);					 
						if( !worker_thd.get() )
						{				
							try
							{
								if( get_current_count() < get_max_count() )
								{							
									worker_thd = sp_worker_ptr( new worker() );									
								}						 
								else
								{										
									add_request( request );															
									continue;
								}
							}
							catch(boost::thread_resource_error & e)
							{
								e;
								add_request( request );
								LOG_DEBUG("EXCEPTION Reason{ " << e.what() << " } retry request" );
								continue;
							}							
							catch(std::bad_alloc & e)
							{
								e;
								add_request( request );
								LOG_DEBUG("EXCEPTION Reason{ " << e.what() << " } retry request" );
								continue;

							}
							catch(...)
							{									
								add_request( request );
								LOG_DEBUG("EXCEPTION Reason{ unknown retry request}" );
								continue;
							}
						}							
											 
						worker_thd->assign( request );
						worker_thd->activity( true );
						add_to_working_list( worker_thd );
						
						thread_pool_strategy * strategy = 0;
						strategy = get_strategy();
						
						unsigned int dels = 0;
						
						if( strategy && (dels = strategy->get_delete_count( get_idle_count(),get_max_count()) ) >0 )
						{
							LOG_DEBUG("execute delete thread strategy the count of deleted is " << dels );					     
							release_free_thread( dels );
						}					 
					}//	request.get()		
					
				}//try						
				catch(...)
				{					
                    LOG_DEBUG("EXCEPTION Reason { pool occur unknown exception }");
				}
			}		
		}

		/////////////////////////////////
		//worker_thread implement
		//
		/////////////////////////////////.
		void worker_thread::run()
		{
			thread_pool<worker_thread> & pool = thread_pool<worker_thread>::instance();
			while(!shutdowned_)
			{					
				try
				{
					wait_activitied();				
					
					if( shutdowned_ )
					{
						LOG_DEBUG("worker_thread is shutdowned!");
						break;
					}							
					if( pool.is_shutdowned() )
					{
						LOG_DEBUG("pool is shutdowned!");
						break;
					}
					
					executeRequest();
					
					activity(false);				
					pool.remove_from_working_list(this);
					pool.add_to_free_list(this);
				}				
				catch(...)
				{
					LOG_DEBUG("EXCEPTION Reason { worker_thread occur unknown exception }");
				}
			}
		}
		void worker_thread::assign(request_ptr request)
		{
			this->request_ = request;
		}
		void worker_thread::executeRequest()
		{		
			if( request_.get() )
			{
				try
				{					
                    request_->process();  
				}
				catch(...)
				{
					LOG_DEBUG("EXCEPTION Reason { process request exception!}");
				}

				request_.reset();
			}
		}

		void worker_thread::shutdown()
		{
             shutdowned_ = true;
			 activity(true);
			 if( thd_ )
				 thd_->join();
			 delete thd_;
			 thd_ = 0;
		}
		void worker_thread::activity(bool bActive)
		{
             scoped_lock_t guard( active_lock_ );
			 activityed_ = bActive;           
             active_cond_.notify_one();	 
		}

		void worker_thread::wait_activitied()
		{
             scoped_lock_t guard( active_lock_ );
			 while( !activityed_ )		
			    active_cond_.wait( guard );			 
		}
        
		///////////////////////////////////////
		//常量声明
		const int worker_thread::DEFAULT_WAIT_REQUEST_TIMEOUT = 100;
		const int worker_thread::DEFAULT_WAIT_THREAD_TIMEOUT = 10;

		/////////////////////////////////
		//thread_pool_func implement
		//
		/////////////////////////////////
		template<class worker>
		void thread_pool_func<worker>::operator()()
		{
			assert( pool_ != 0 );
			pool_->run();
		}
	}

	///////////////////////////////////////////
	//singleton
	//
	///////////////////////////////////////////
	template <typename T>
    T& singleton<T>::instance()
    {
		if( !instance_ )
		{ 
			scoped_lock_t guard(lock_);
			if( !instance_ )
				instance_ = new T();
		}        		
        return *instance_;
    }		
	
    template <typename T> T * singleton<T>::instance_ = 0;
    template <typename T> typename singleton<T>::mutex_t singleton<T>::lock_;

    /////////////////////////////////////////
	//thread_pool	
	typedef core::thread_pool<core::worker_thread> thread_pool;

} //robert
typedef robert::thread_pool thread_pool;
typedef thread_pool::request_ptr request_ptr; 