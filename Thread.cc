#include <type_traits>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>

#include "CurrentThread.hh"

namespace Wood {

namespace CurrentThread {
__thread int t_tid_ = 0;
__thread char t_tidStr_[32] = "";
__thread int t_tidStrLen_ = 0;
__thread const char* t_threadName_ = "unknown";

static_assert(std::is_same<int, pid_t>::value, "pid_t is not a int type.");
}

namespace Detail {
	pid_t getTid() 
	{
		return static_cast<pid_t>(::syscall(SYS_gettid));
	}
}

void CurrentThread::cacheTid()
{
	if(t_tid_ == 0)
	{
		t_tid_ = Detail::getTid();
	}
}

}