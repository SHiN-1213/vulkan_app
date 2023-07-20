#include <iostream>
#include "reel.hpp"

int main()
{
	reel::VulkanApi api;
	try
	{
		api.run();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;

	}
	return EXIT_SUCCESS;
}
