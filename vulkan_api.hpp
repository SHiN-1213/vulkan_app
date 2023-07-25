#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <optional>

#include "reel_err.hpp"

namespace reel
{
	class VulkanApi
	{
	private:
		static constexpr uint32_t m_WIDTH = 800;
		static constexpr uint32_t m_HEIGHT = 600;

		const std::vector<const char *> m_validation_layers = {
				"VK_LAYER_KHRONOS_validation"
		};
#ifdef NDEBUG
		static constexpr bool m_enable_validation_layers = false;
#else
		static constexpr bool m_enable_validation_layers = true;
#endif
		GLFWwindow *m_window;

		VkInstance m_instance;

		VkDebugUtilsMessengerEXT m_debug_messenger;

		VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;

		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;

			bool isComplete() {
				return graphicsFamily.has_value();
			}
		};


	public:
		void run();

	private:

		void initWindow();

		void initVulkan();

		bool checkValidationLayerSupport();

		std::vector<const char*> getRequiredExtensions();

		void createInstance();

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		void setupDebugMessenger();

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		bool isDeviceSuitable(VkPhysicalDevice device);

		void pickPhysicalDevice();

		void mainLoop();

		void cleanup();
	};

}
