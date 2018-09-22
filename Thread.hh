#include <pthread.h>

namespace Wood 
{
class Thread {
	Thread() = default;
	~Thread() = default;
	Thread(const Thread& thread) = delete;
	Thread & operator=(const Thread& thread) = delete;
	//Thread(Thread && thread);
};

}