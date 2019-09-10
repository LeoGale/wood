#include <iostream>

#include "wood/base/ThreadPool.hh"


int main(int argc, char* argv[])
{
	wood::ThreadPool threadPool(3);
	auto future = threadPool.deliver([](){return 1+3});
	std::cout <<"result " << future.get() << std::endl;
}
