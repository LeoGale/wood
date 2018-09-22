namespace Wood {

namespace CurrentThread {
	extern __thread int t_tid_;
	extern __thread char t_tidStr_[32];
	extern __thread int t_tidStrLen_;
	extern __thread const char* t_threadName_;

	void cacheTid();

	inline int tid() {
		if(t_tid_ == 0)
		{
			cacheTid();
		}
		return t_tid_;
	}

	inline const char* tidInStr() {
		return t_tidStr_;
	}

	inline int tidStrLength() {
		return t_tidStrLen_;
	}

	inline const char* name() {
		return t_threadName_;
	}


}

}