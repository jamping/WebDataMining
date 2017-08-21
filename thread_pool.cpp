/**
 * \file		thread_pool.cpp
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
 
/*
**  Includes
*/

#define _WIN32_WINNT 0x0500
#include <conio.h>
#include <iostream>
#include "thread_pool.h"
using namespace plugware;

/*
**  Implementation
*/

#define WORK( label )                                           \
struct work_##label : core::work_unit                           \
{                                                               \
    void process() throw()                                      \
    {                                                           \
        std::cout << "processing work " << #label << std::endl; \
    }                                                           \
};

WORK( 1 ); 
WORK( 2 ); 
WORK( 3 );

int main(int, char**)
{
    // initialize the thread pool
    global::thread_pool::instance().initialize();    
    
    //@@todo: demonstrate asynchronous notification
    std::cout << "Press any key to die ..." << std::endl;       

    // demonstrate chaining
    global::thread_pool::instance().queue_request(
        (core::chain(), new work_1, new work_2, new work_3));

    _getch();
    return 0;
}
