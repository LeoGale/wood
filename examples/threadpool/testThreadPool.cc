#include <iostream>

#include "wood/base/ThreadPool.hh"


int main(int argc, char* argv[])
{
	wood::ThreadPool threadPool(3);
	auto future2 = threadPool.deliver([](){std::cout <<"Just say hello" << std::endl;throw std::runtime_error("test Exception");});

	auto future = threadPool.deliver(0, [](){return 1+3;});
	std::cout <<"result " << future.get() << std::endl;

	auto future1 = threadPool.deliver([](double a, double b){return a+b;}, 3.01, 4.09);
	std::cout <<"result1 " << future1.get() << std::endl;
    

    	try
    	{
		future2.get();
    	}
    	catch(std::exception& e)
    	{
    		std::cout <<"catch exception " << e.what() << std::endl;
    	}
}
