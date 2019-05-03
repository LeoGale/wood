//
//  CurrentThread.hpp
//  
//
//  Created by leo on 2019/4/30.
//

#ifndef CurrentThread_hh
#define CurrentThread_hh

namespace tornado
{
namespace CurrentThread
{
    extern __thread int t_tid_;
    extern __thread const char* t_name_;
    static_assert(std::is_same<pid_t, int>::value, "pid_t should equal int");
    void cacheTid();
    
    pid_t tid()
    {
        if(tid == 0)
        {
            cacheTid();
        }
        return t_tid_;
    }
    
    const char* name();
}
}

#endif /* CurrentThread_hh */
