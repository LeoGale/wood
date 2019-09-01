//
//  CurrentThread.hpp
//
//
//  Created by leo on 2019/4/30.
//

#ifndef CurrentThread_hh
#define CurrentThread_hh

namespace wood 
{
namespace CurrentThread
{
    extern __thread int t_tid_;
    extern __thread const char* t_name_;

    void cacheTid();

    inline pid_t tid()
    {
        if(t_tid_ == 0)
        {
            cacheTid();
        }
        return t_tid_;
    }

    inline const char* name();
    inline bool isMainThread();
}
}

#endif /* CurrentThread_hh */
