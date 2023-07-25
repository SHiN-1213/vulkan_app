#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3native.h>

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <optional>
#include <set>
#include <cstdint>
#include <limits>
#include <algorithm>

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

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;


			bool isComplete()
			{
				return graphicsFamily.has_value() &&
				       presentFamily.has_value();
			}
		};

		const std::vector<const char *> m_device_extensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		VkDevice m_device;

		VkQueue m_graphics_queue;

		VkQueue m_present_queue;

		VkSurfaceKHR m_surface;

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		VkSwapchainKHR m_swap_chain;
		std::vector<VkImage> m_swap_chain_images;
		VkFormat m_swap_chain_image_format;
		VkExtent2D m_swap_chain_extent;

		std::vector<VkImageView> m_swap_chain_image_views;


	public:
		void run();

	private:

		void initWindow();

		void initVulkan();

		bool checkValidationLayerSupport();

		std::vector<const char *> getRequiredExtensions();

		void createInstance();

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

		void setupDebugMessenger();

		void createSurface();

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		bool isDeviceSuitable(VkPhysicalDevice device);

		void pickPhysicalDevice();

		void createLogicalDevice();

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);

		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);

		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		void createSwapChain();

		void createImageViews();

		void mainLoop();

		void cleanup();
	};

}
