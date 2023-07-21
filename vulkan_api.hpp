#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

#include "reel_err.hpp"

namespace reel
{
	class VulkanApi
	{
	private:
		static constexpr uint32_t WIDTH = 800;
		static constexpr uint32_t HEIGHT = 600;

		GLFWwindow *m_window;

		VkInstance m_instance;
	public:
		void run();
	private:
		void initWindow();

		void initVulkan();
		void createInstance();

		void mainLoop();

		void cleanup();
	};

}
