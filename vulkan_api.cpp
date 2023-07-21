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
		createInstance();
	}

	void VulkanApi::createInstance()
	{
		VkApplicationInfo app_info;
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pApplicationName = "hello vulkan";
		app_info.applicationVersion = VK_MAKE_VERSION(1,0,0);
		app_info.pEngineName = "no engine";
		app_info.engineVersion = VK_MAKE_VERSION(1,0,0);
		app_info.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo create_info;
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.pApplicationInfo = &app_info;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		create_info.enabledExtensionCount = glfwExtensionCount;
		create_info.ppEnabledExtensionNames = glfwExtensions;
		create_info.enabledLayerCount = 0;

		if (vkCreateInstance(&create_info, nullptr, &m_instance) != VK_SUCCESS)
		{
			std::runtime_error("failed to create instance!");
		}
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