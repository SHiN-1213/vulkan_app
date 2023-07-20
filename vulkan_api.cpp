#include "vulkan_api.hpp"

namespace reel
{
	void VulkanApi::run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

	void VulkanApi::initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	}

	void VulkanApi::initVulkan()
	{

	}

	void VulkanApi::mainLoop()
	{
		while (!glfwWindowShouldClose(m_window)) {
			glfwPollEvents();
		}

	}

	void VulkanApi::cleanup()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}