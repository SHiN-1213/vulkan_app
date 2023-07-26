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
#include <fstream>


#include "reel_err.hpp"

namespace reel
{
	class VulkanApi
	{
	private:
		static constexpr uint32_t m_WIDTH = 800;
		static constexpr uint32_t m_HEIGHT = 600;

		static constexpr int m_MAX_FRAMES_IN_FLIGHT = 2;


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

		VkRenderPass m_render_pass;

		VkPipelineLayout m_pipeline_layout;

		VkPipeline m_graphics_pipeline;

		std::vector<VkFramebuffer> m_swap_chain_frame_buffers;

		VkCommandPool m_command_pool;

		uint32_t m_current_frame = 0;

		std::vector<VkCommandBuffer> m_command_buffers;

		std::vector<VkSemaphore> m_image_available_semaphores;
		std::vector<VkSemaphore> m_render_finished_semaphores;
		std::vector<VkFence> m_in_flight_fences;

		bool framebufferResized = false;

	public:
		void run();

	private:
		static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

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

		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

		void createSwapChain();

		void cleanupSwapChain();

		void recreateSwapChain();

		void createImageViews();

		void createRenderPass();

		VkShaderModule createShaderModule(const std::vector<char> &code);

		void createGraphicsPipeline();

		void createFramebuffers();

		void createCommandPool();

		void createCommandBuffers();

		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

		void createSyncObjects();

		void drawFrame();

		void mainLoop();

		void cleanup();
	};

}
