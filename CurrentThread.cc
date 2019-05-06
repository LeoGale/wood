//
//  CurrentThread.cpp
//
//
//  Created by leo on 2019/4/30.
//

#include <type_traits>

#include <stdlib.h>

#include "CurrentThread.hh"

namespace tornado
{
namespace CurrentThread
{
    static_assert(std::is_same<pid_t, int>::value, "pid_t should be equal to int");
    __thread int t_tid_ = 0;
    __thread const char* t_name_ = "unknown";
}
}
